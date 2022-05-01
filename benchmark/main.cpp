#include <iostream>
#include <fstream>
#include <iomanip>

#include <string_view>
#include <functional>
#include <map>
#include <vector>

#include "benchmark_utils.h"
#include "../src/instrumented_index.h"

#include "arg_parser.h"
#include "tsc_x86.h"

#include "../graphs/adjacency_graph.h"

#include "../src/forward.h"
#include "../src/forward_hashed.h"

#include "../src/instrumented_index.h"
#include "../src/triangle_lister.h"

static constexpr size_t default_num_warmups = 1;
static constexpr size_t default_num_runs = 5;
static constexpr size_t default_num_phases = 5;

template<class Index, class TLR>
static std::map<std::string, TriangleFunctions<Index, TLR>> name_to_function = {
        {"edge_iterator",  TriangleFunctions(edge_iterator<Index, TLR>, edge_iterator_get_dummy_helper)},
        {"forward",        TriangleFunctions(forward<Index, TLR>, forward_create_neighbor_container)},
        {"forward_hashed", TriangleFunctions(forward_hashed<Index, TLR>, forward_hashed_create_neighbor_container)},
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
    auto *instrumented_graph = create_graph_from_file<InstrumentedIndex>(params.graph_file.c_str());
    auto *benchmark_graph = create_graph_from_file<index_t>(params.graph_file.c_str());

    std::cout << "num_warmups = " << params.num_warmups << std::endl;
    std::cout << "num_phases = " << params.num_phases << std::endl;
    std::cout << "num_runs = " << params.num_runs << std::endl;

    static const auto test_translator = name_to_function<InstrumentedIndex, TriangleListing::Collect>;
    static const auto benchmark_translator = name_to_function<index_t, TriangleListing::Count>;

    TriangleSet last_result;
    bool has_last_result = false;

    for (const auto &algo_name: params.algos) {

        out_file << algo_name << ",";

        std::cout << "Instrumented run: " << algo_name << std::endl;
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
            }
        }
        out_file << op_count;
        std::cout << "  " << op_count << " ops" << std::endl;
        std::cout << "  Result verified" << std::endl;

        std::cout << "Benchmarking " << algo_name << std::endl;
        {
            const auto &functions = benchmark_translator.at(algo_name);  // throw error if no matched name
            void *helper = functions.get_helper(benchmark_graph);

            // Do the warmup runs
            for (size_t i = 0; i < params.num_warmups; i++) {
                functions.count(benchmark_graph, helper);
            }

            // Do the benchmark runs
            for (size_t phase = 0; phase < params.num_phases; phase++) {
                TriangleListing::Count result;
                size_t cycles = start_tsc();
                for (size_t run = 0; run < params.num_runs; run++) {
                    result = functions.count(benchmark_graph, helper);
                }
                cycles = stop_tsc(cycles);
                if (result.count != last_result.size()) {
                    throw std::runtime_error("count of triangles differs from the instrumented run");
                }
                size_t cycle_per_run = cycles / params.num_runs;
                out_file << "," << cycle_per_run;
                std::cout << "  " << cycle_per_run << " cycles/run, "
                          << (double) op_count / (double) cycle_per_run << " ops/cycle" << std::endl;
            }
        }
        out_file << "\n";
    }
    std::cout << "All Benchmarking Completed" << std::endl;
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
