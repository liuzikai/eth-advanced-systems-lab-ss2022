#include <iostream>
#include <fstream>
#include <iomanip>

#include <string_view>
#include <functional>
#include <map>
#include <vector>

#include "benchmark_utils.h"
#include "instrumented_index.h"

#include "arg_parser.h"
#include "tsc_x86.h"

#include "adjacency_graph.h"

#include "forward.hpp"
#include "forward_hashed.hpp"
#include "forward_hashed_v1.hpp"

#include "instrumented_index.h"
#include "triangle_lister.h"

static constexpr size_t default_num_warmups = 1;
static constexpr size_t default_num_runs = 5;
static constexpr size_t default_num_phases = 5;

template<class Index, class Counter, class TLR>
static std::map<std::string, TriangleFunctions<Index, Counter, TLR>> name_to_function = {
        {"edge_iterator",  TriangleFunctions(edge_iterator<Index, Counter, TLR>, edge_iterator_get_dummy_helper<Index, Counter>, edge_iterator_free_dummy_helper<Index, Counter>)},
        {"forward",        TriangleFunctions(forward<Index, Counter, TLR>, forward_create_neighbor_container<Index, Counter>, forward_delete_neighbor_container<Index, Counter>)},
        {"forward_hashed", TriangleFunctions(fh0::forward_hashed<Index, Counter, TLR>, fh0::forward_hashed_create_neighbor_container<Index, Counter>, fh0::forward_hashed_delete_neighbor_container<Index, Counter>)},
        {"fh1", TriangleFunctions(fh1::forward_hashed<Index, Counter, TLR>, fh1::forward_hashed_create_neighbor_container<Index, Counter>, fh1::forward_hashed_delete_neighbor_container<Index, Counter>)},
};

BenchParams parse_arguments(arg_parser &parser) {
    BenchParams params;
    params.num_warmups = parser.getCmdOptionAsInt("-num_warmups").value_or(default_num_warmups);
    params.num_runs = parser.getCmdOptionAsInt("-num_runs").value_or(default_num_runs);
    params.num_phases = parser.getCmdOptionAsInt("-num_phases").value_or(default_num_phases);
    params.file_name = parser.getCmdOption("-o").value_or("default.csv");
    std::optional<std::string_view> graph_file = parser.getCmdOption("-graph");
    std::optional<std::string_view> algos_opt = parser.getCmdOption("-algorithm");

    // TODO: Maybe we want to benchmark time later as well?
    // std::string_view timing_method_string = parser.getCmdOption("-timing").value_or("cycles");

    if (!graph_file) {
        throw std::invalid_argument("No algorithm specified.");
    }
    if (!algos_opt) {
        throw std::invalid_argument("No algorithm specified.");
    }

    params.graph_file = graph_file.value();

    // Sadly ranges to split the view are not available in gcc yet.
    params.algos = split(std::string(*algos_opt), ',');
    return params;
}

void run(const BenchParams &params, std::ofstream &out_file) {
    std::cout << "num_warmups = " << params.num_warmups << std::endl;
    std::cout << "num_phases = " << params.num_phases << std::endl;
    std::cout << "num_runs = " << params.num_runs << std::endl;

    std::map<std::string, size_t> op_counts;
    size_t triangle_count = -1;

    // Instrumented runs
    {
        auto *instrumented_graph = create_graph_from_file<InstrumentedIndex>(params.graph_file.c_str());

        static const auto test_translator = name_to_function<InstrumentedIndex, index_t , TriangleListing::Collect<InstrumentedIndex>>;

        TriangleListing::Collect<InstrumentedIndex>::TriangleSet last_result;
        bool has_last_result = false;

        for (const auto &algo_name: params.algos) {

            uintptr_t op_count;
            {
                const auto &functions = test_translator.at(algo_name);  // throw error if no matched name
                void *helper = functions.get_helper(instrumented_graph);

                // List triangles and get op count
                OpCounter::ResetOpCount();
                auto result = functions.count(instrumented_graph, helper);
                op_count = OpCounter::GetOpCount();

                // Compare triangles with the result of the last algorithm (if available)
                if (has_last_result) {
                    if (result.triangles != last_result) {
                        throw std::runtime_error("different triangles");
                    }
                } else {
                    last_result = std::move(result.triangles);
                    has_last_result = true;
                    triangle_count = last_result.size();
                }

                functions.free_helper(helper);
            }
            op_counts[algo_name] = op_count;
            std::cout << algo_name << ": " << op_count << " ops, verified, " << last_result.size() << " triangles" << std::endl;
        }

        free_graph(instrumented_graph);
    }

    // Declare as const to avoid misuse that change the graph
    const auto *benchmark_graph_original = create_graph_from_file<index_t>(params.graph_file.c_str());

    // Make copies of the benchmark graph
    std::vector<AdjacencyGraph<index_t>*> benchmark_graphs;
    benchmark_graphs.reserve(params.num_runs);
    for (size_t i = 0; i < params.num_runs; i++) {
        benchmark_graphs.emplace_back(create_graph_copy(benchmark_graph_original));
    }

    static const auto benchmark_translator = name_to_function<index_t, index_t, TriangleListing::Count<index_t>>;

    for (const auto &algo_name: params.algos) {

        out_file << algo_name << "," << op_counts[algo_name];

        std::cout << "Benchmarking " << algo_name << std::endl;
        {
            const auto &functions = benchmark_translator.at(algo_name);  // throw error if no matched name
            void *helper = functions.get_helper(benchmark_graph_original);

            // Do the warmup runs
            // Use benchmark_graphs[0], which will be restored later anyway
            for (size_t i = 0; i < params.num_warmups; i++) {
                functions.count(benchmark_graphs[0], helper);
            }

            // Do the benchmark runs
            for (size_t phase = 0; phase < params.num_phases; phase++) {

                // Restore the graphs
                for (size_t i = 0; i < params.num_runs; i++) {
                    copy_graph(benchmark_graphs[i], benchmark_graph_original);
                }

                TriangleListing::Count<index_t> result;

                // Start benchmark
                size_t cycles = start_tsc();
                for (size_t run = 0; run < params.num_runs; run++) {
                    result = functions.count(benchmark_graphs[run], helper);
                }
                cycles = stop_tsc(cycles);

                if (result.count != triangle_count) {
                    throw std::runtime_error("count of triangles differs from the instrumented run");
                }
                size_t cycle_per_run = cycles / params.num_runs;
                out_file << "," << cycle_per_run;
                std::cout << "  " << cycle_per_run << " cycles/run, "
                          << (double) op_counts[algo_name] / (double) cycle_per_run << " ops/cycle" << std::endl;
            }

            functions.free_helper(helper);
        }
        out_file << "\n";
    }
    std::cout << "All Benchmarking Completed" << std::endl;

    free_graph(benchmark_graph_original);
    for (size_t i = 0; i < params.num_warmups; i++) {
        free_graph(benchmark_graphs[i]);
    }
}

int main(int argc, char *argv[]) {
    arg_parser parser(argc, argv);
    BenchParams params = parse_arguments(parser);

    std::ofstream out_file;
    out_file.open(std::string(params.file_name));
    out_file << std::fixed;
    out_file << "algorithm,ops";
    for (size_t i = 0; i < params.num_phases; i++) {
        out_file << ",cycles_" << i;
    }
    out_file << std::endl;

    std::cout << std::fixed;
    run(params, out_file);

    out_file.close();
}
