#include <iostream>
#include <fstream>
#include <iomanip>

#define COLLECT_TRIANGLES
// If not defined we will only count.

#include <string_view>
#include <functional>
#include <map>
#include <vector>

#include "benchmark_utils.h"
#include "instrumented_index.h"

#include "arg_parser.h"
#include "tsc_x86.h"

#include "adjacency_graph.h"

#include "instrumented_index.h"
#include "triangle_lister.h"

#include "dummy_helper.hpp"

#include "edge_iterator/edge_iterator_all_versions.h"

#include "forward/forward_all_versions.h"

#include "forward_hashed/forward_hashed_all_versions.h"

#include "merge_sort/merge_sort_all_versions.h"
#include "avx2-quicksort.h"
#include "quick_sort.h"

#include "quick_cut_sort.h"
#include "std_sort.h"

static constexpr size_t default_num_warmups = 1;
static constexpr size_t default_num_runs = 5;
static constexpr size_t default_num_phases = 5;

template<class Index, class Counter, class TLR>
static std::map<std::string, TriangleFunctions<Index, Counter, TLR>> name_to_function = {
    {"ei_base",  TriangleFunctions(ei0::edge_iterator<Index, Counter, TLR>, get_dummy_helper<Index, Counter>, free_dummy_helper<Index, Counter>)},
    {"ei_v1",  TriangleFunctions(ei1::edge_iterator<Index, Counter, TLR>, get_dummy_helper<Index, Counter>, free_dummy_helper<Index, Counter>)},
    {"ei_v2",  TriangleFunctions(ei2::edge_iterator<Index, Counter, TLR>, get_dummy_helper<Index, Counter>, free_dummy_helper<Index, Counter>)},
    {"ei_v3",  TriangleFunctions(ei3::edge_iterator<Index, Counter, TLR>, get_dummy_helper<Index, Counter>, free_dummy_helper<Index, Counter>)},
    {"ei_u4",  TriangleFunctions(eu4::edge_iterator<Index, Counter, TLR>, get_dummy_helper<Index, Counter>, free_dummy_helper<Index, Counter>)},
    {"ei_u5",  TriangleFunctions(eu5::edge_iterator<Index, Counter, TLR>, get_dummy_helper<Index, Counter>, free_dummy_helper<Index, Counter>)},
    {"ei_vec4",  TriangleFunctions(e_vec4::edge_iterator<Index, Counter, TLR>, get_dummy_helper<Index, Counter>, free_dummy_helper<Index, Counter>)},
    {"ei_vec5",  TriangleFunctions(e_vec5::edge_iterator<Index, Counter, TLR>, get_dummy_helper<Index, Counter>, free_dummy_helper<Index, Counter>)},
    {"ei_va",  TriangleFunctions(eia::edge_iterator<Index, Counter, TLR>, get_dummy_helper<Index, Counter>, free_dummy_helper<Index, Counter>)},
    {"f_base",        TriangleFunctions(f0::forward<Index, Counter, TLR>, f0::forward_create_neighbor_container<Index, Counter>, f0::forward_delete_neighbor_container<Index, Counter>)},
    {"f_v1",        TriangleFunctions(f1::forward<Index, Counter, TLR>, f1::forward_create_neighbor_container<Index, Counter>, f1::forward_delete_neighbor_container<Index, Counter>)},
    {"f_v2",        TriangleFunctions(f2::forward<Index, Counter, TLR>, f2::forward_create_neighbor_container<Index, Counter>, f2::forward_delete_neighbor_container<Index, Counter>)},
    {"f_v3",        TriangleFunctions(f3::forward<Index, Counter, TLR>, f3::forward_create_neighbor_container<Index, Counter>, f3::forward_delete_neighbor_container<Index, Counter>)},
    {"f_v4",        TriangleFunctions(f4::forward<Index, Counter, TLR>, f4::forward_create_neighbor_container<Index, Counter>, f4::forward_delete_neighbor_container<Index, Counter>)},
    {"f_v5",        TriangleFunctions(f5::forward<Index, Counter, TLR>, f5::forward_create_neighbor_container<Index, Counter>, f5::forward_delete_neighbor_container<Index, Counter>)},
    {"f_u4",        TriangleFunctions(fu4::forward<Index, Counter, TLR>, fu4::forward_create_neighbor_container<Index, Counter>, fu4::forward_delete_neighbor_container<Index, Counter>)},
    {"f_v6",        TriangleFunctions(f6::forward<Index, Counter, TLR>, f6::forward_create_neighbor_container<Index, Counter>, f6::forward_delete_neighbor_container<Index, Counter>)},
    {"f_va",        TriangleFunctions(fa::forward<Index, Counter, TLR>, fa::forward_create_neighbor_container<Index, Counter>, fa::forward_delete_neighbor_container<Index, Counter>)},
    {"fh_base", TriangleFunctions(fh0::forward_hashed<Index, Counter, TLR>, fh0::forward_hashed_create_neighbor_container<Index, Counter>, fh0::forward_hashed_delete_neighbor_container<Index, Counter>)},
    {"fh_v1", TriangleFunctions(fh1::forward_hashed<Index, Counter, TLR>, fh1::forward_hashed_create_neighbor_container<Index, Counter>, fh1::forward_hashed_delete_neighbor_container<Index, Counter>)},
    {"fh_v2", TriangleFunctions(fh2::forward_hashed<Index, Counter, TLR>, fh2::forward_hashed_create_neighbor_container<Index, Counter>, fh2::forward_hashed_delete_neighbor_container<Index, Counter>)},
    {"fh_v3", TriangleFunctions(fh3::forward_hashed<Index, Counter, TLR>, fh3::forward_hashed_create_neighbor_container<Index, Counter>, fh3::forward_hashed_delete_neighbor_container<Index, Counter>)},
    {"fh_v4", TriangleFunctions(fh4::forward_hashed<Index, Counter, TLR>, fh4::forward_hashed_create_neighbor_container<Index, Counter>, fh4::forward_hashed_delete_neighbor_container<Index, Counter>)},
    {"fh_va", TriangleFunctions(fha::forward_hashed<Index, Counter, TLR>, fha::forward_hashed_create_neighbor_container<Index, Counter>, fha::forward_hashed_delete_neighbor_container<Index, Counter>)},
    // Sorting
    {"quick_sort",  TriangleFunctions(quick_sort_timing<Index, Counter, TLR>, get_dummy_helper<Index, Counter>, free_dummy_helper<Index, Counter>)},
    {"merge_sort_base",  TriangleFunctions(ms0::merge_sort_timing<Index, Counter, TLR>, get_dummy_helper<Index, Counter>, free_dummy_helper<Index, Counter>)},
    {"merge_sort_v2",  TriangleFunctions(ms2::merge_sort_timing<Index, Counter, TLR>, get_dummy_helper<Index, Counter>, free_dummy_helper<Index, Counter>)},
    {"merge_sort_v3",  TriangleFunctions(ms3::merge_sort_timing<Index, Counter, TLR>, get_dummy_helper<Index, Counter>, free_dummy_helper<Index, Counter>)},
    {"merge_sort_v4",  TriangleFunctions(ms4::merge_sort_timing<Index, Counter, TLR>, get_dummy_helper<Index, Counter>, free_dummy_helper<Index, Counter>)},
    {"std_sort",  TriangleFunctions(std_sort_timing<Index, Counter, TLR>, get_dummy_helper<Index, Counter>, free_dummy_helper<Index, Counter>)},
};

template<class Counter, class TLR>
static std::map<std::string, TriangleFunctions<index_t, Counter, TLR>> name_to_function_no_instrumentation = {
   // {"WojciechMula",  TriangleFunctions(WojciechMula_sort_timing<index_t, Counter, TLR>, get_dummy_helper<index_t, Counter>, free_dummy_helper<index_t, Counter>)},

};

//***** sort, cut *****
// sort+cut This is used to presort the edges and remove the unused ones.
template<class Index>
static auto pre_sort_cut_function = quick_cut_sort<Index>;

// sort only
template<class Index>
static auto pre_sort_function = quick_sort<Index>;

// cut only
template<class Index>
static auto pre_cut_function = quick_cut<Index>;

// sort + cut
template<class Index>
static void pre_sort_cut_graph(AdjacencyGraph<Index> *G) {
    for (index_t u = 0; u < G->n; u++) {
        if (G->adjacency[u].count > 0) {
            pre_sort_cut_function<Index>(G->adjacency[u].neighbors, 0, G->adjacency[u].count - 1, (Index) u, &G->adjacency[u].count);
        }
    }
}

// sort only
template<class Index>
static void pre_sort_graph(AdjacencyGraph<Index> *G) {
    for (index_t u = 0; u < G->n; u++) {
        if (G->adjacency[u].count > 0) {
            pre_sort_function<Index>(G->adjacency[u].neighbors, 0, G->adjacency[u].count - 1);
        }
    }
}

// cut only
template<class Index>
static void pre_cut_graph(AdjacencyGraph<Index> *G) {
    for (index_t u = 0; u < G->n; u++) {
        if (G->adjacency[u].count > 0) {
            pre_cut_function<Index>(G->adjacency[u].neighbors, 0, G->adjacency[u].count - 1, (Index) u, &G->adjacency[u].count);
        }
    }
}
//***** sort, cut *****

BenchParams parse_arguments(arg_parser &parser) {
    BenchParams params;
    params.num_warmups = parser.getCmdOptionAsInt("-num_warmups").value_or(default_num_warmups);
    params.num_runs = parser.getCmdOptionAsInt("-num_runs").value_or(default_num_runs);
    params.num_phases = parser.getCmdOptionAsInt("-num_phases").value_or(default_num_phases);
    params.file_name = parser.getCmdOption("-o").value_or("default.csv");
    std::optional<std::string_view> graph_file = parser.getCmdOption("-graph");
    std::optional<std::string_view> algos_opt = parser.getCmdOption("-algorithm");
    params.pre_sort_edge_lists = !parser.cmdOptionExists("-no_pre_sort");
    params.pre_cut_edge_lists = !parser.cmdOptionExists("-no_pre_cut");

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
    std::cout << "pre_cut_edge_lists = " << params.pre_cut_edge_lists << std::endl;
    std::cout << "pre_sort_edge_lists = " << params.pre_sort_edge_lists << std::endl;

    std::map<std::string, size_t> op_counts;
    size_t triangle_count = 20836815;

    // Instrumented runs
    #ifndef NO_INSTRUMENTATION
    {
        auto *instrumented_graph_original = create_graph_from_file<InstrumentedIndex>(params.graph_file.c_str());
        if (params.pre_sort_edge_lists && params.pre_cut_edge_lists) {
            pre_sort_cut_graph<InstrumentedIndex>(instrumented_graph_original);
        } else if (params.pre_cut_edge_lists) {
            pre_cut_graph<InstrumentedIndex>(instrumented_graph_original);
        } else if (params.pre_sort_edge_lists) {
            pre_sort_graph<InstrumentedIndex>(instrumented_graph_original);
        }

        auto instrumented_graph_copy = create_graph_copy(instrumented_graph_original);

        #ifdef COLLECT_TRIANGLES
        auto test_translator = name_to_function<InstrumentedIndex, index_t , TriangleListing::SetCollect<InstrumentedIndex>>;
        #else
        auto test_translator = name_to_function<InstrumentedIndex, index_t , TriangleListing::Count<InstrumentedIndex>>;
        #endif
        #ifdef COLLECT_TRIANGLES
        TriangleListing::SetCollect<InstrumentedIndex>::TriangleSet last_result;
        #else
        size_t last_result;
        #endif

        bool has_last_result = false;
        for (const auto &algo_name: params.algos) {

            uintptr_t op_count;
            {
                auto functions_it = test_translator.find(algo_name);
                if (functions_it == test_translator.end()) {
                    // Skip and report 0 as
                    std::cerr << "Skip instrumentation for: " << algo_name << std::endl;
                    op_count = 0;
                } else {
                    const auto &functions = functions_it->second;
                    void *helper = functions.get_helper(instrumented_graph_copy);

                    // List triangles and get op count
                    #ifdef COLLECT_TRIANGLES
                    TriangleListing::SetCollect<InstrumentedIndex> result;
                    #else
                    TriangleListing::Count<InstrumentedIndex> result(0);
                    #endif
                    OpCounter::ResetOpCount();
                    functions.count(&result, instrumented_graph_copy, helper);
                    op_count = OpCounter::GetOpCount();
                    #ifdef COLLECT_TRIANGLES
                    auto result_set = result.triangles;
                    #else
                    auto result_set = result.count;
                    #endif
                    // Compare triangles with the result of the last algorithm (if available)
                    if (has_last_result) {
                        if (result_set != last_result) {
                            // Convert an int to a string
                            std::stringstream ss;
                            #ifdef COLLECT_TRIANGLES
                            ss << "Different triangles! Count is: " << result_set.size() << " expected: " << last_result.size();
                            #else
                            ss << "Different triangles! Count is: " << result_set << " expected: " << last_result;
                            #endif
                            std::cerr << ss.str() << std::endl;
                            //throw std::runtime_error(ss.str());
                        }
                    } else {
                        last_result = std::move(result_set);
                        has_last_result = true;
                        #ifdef COLLECT_TRIANGLES
                        triangle_count = last_result.size();
                        #else
                        triangle_count = last_result;
                        #endif
                    }

                    copy_graph(instrumented_graph_copy, instrumented_graph_original);
                    functions.free_helper(helper);
                }
            }
            op_counts[algo_name] = op_count;
            #ifdef COLLECT_TRIANGLES
            std::cout << algo_name << ": " << op_count << " ops, verified, " << last_result.size() << " triangles" << std::endl;
            #else
            std::cout << algo_name << ": " << op_count << " ops, verified, " << last_result << " triangles" << std::endl;
            #endif
        
        }
        free_graph(instrumented_graph_original);
        free_graph(instrumented_graph_copy);
    }
    #endif

    // Declare as const to avoid misuse that change the graph
    auto *benchmark_graph_original = create_graph_from_file<index_t>(params.graph_file.c_str());
    if (params.pre_sort_edge_lists && params.pre_cut_edge_lists) {
        pre_sort_cut_graph<index_t>(benchmark_graph_original);
    } else if (params.pre_cut_edge_lists) {
        pre_cut_graph<index_t>(benchmark_graph_original);
    } else if (params.pre_sort_edge_lists) {
        pre_sort_graph<index_t>(benchmark_graph_original);
    }

    // Make copies of the benchmark graph
    std::vector<AdjacencyGraph<index_t>*> benchmark_graphs;
    benchmark_graphs.reserve(params.num_runs);
    if(!params.pre_sort_edge_lists) {
        // We dont need copies if we presort the edge lists.
        for (size_t i = 0; i < params.num_runs; i++) {
            benchmark_graphs.emplace_back(create_graph_copy(benchmark_graph_original));
        }
    }

    AdjacencyGraph<index_t>* warmup_graph = nullptr;
    if (params.num_warmups > 0) {
        warmup_graph = create_graph_copy(benchmark_graph_original);
    }
    // Yes this could be templated.
    #ifdef COLLECT_TRIANGLES
    auto benchmark_translator = name_to_function<index_t, index_t, TriangleListing::Collect<index_t>>;
    auto translator_no_instrumentation = name_to_function_no_instrumentation<index_t, TriangleListing::Collect<index_t>>;
    #else
    auto benchmark_translator = name_to_function<index_t, index_t, TriangleListing::Count<index_t>>;
    auto translator_no_instrumentation = name_to_function_no_instrumentation<index_t, TriangleListing::Count<index_t>>;
    #endif

    benchmark_translator.insert(translator_no_instrumentation.begin(), translator_no_instrumentation.end());



    for (const auto &algo_name: params.algos) {

        out_file << algo_name << "," << op_counts[algo_name];

        std::cout << "Benchmarking " << algo_name << std::endl;
        {
            const auto &functions = benchmark_translator.at(algo_name);  // throw error if no matched name
            void *helper = functions.get_helper(benchmark_graph_original);

            // Do the benchmark runs
            for (size_t phase = 0; phase < params.num_phases; phase++) {

                // Do the warmup runs
                // Use benchmark_graphs[0], which will be restored later anyway
                // We know the triangle count from the instrumented roun.
                #ifdef COLLECT_TRIANGLES
                TriangleListing::Collect<index_t> result(triangle_count);
                #else
                TriangleListing::Count<index_t> result(triangle_count);
                #endif
                for (size_t i = 0; i < params.num_warmups; i++) {
                    functions.count(&result, warmup_graph, helper);
                    result.reset();
                }


                // Start benchmark
                size_t cycles;
                if(params.pre_sort_edge_lists) {
                    cycles = start_tsc();
                    for (size_t run = 0; run < params.num_runs; run++) {
                        result.reset();
                        functions.count(&result, benchmark_graph_original, helper);
                    }
                    cycles = stop_tsc(cycles);
                    std::cout << result.count << std::endl;
                } else {
                    cycles = start_tsc();
                    for (size_t run = 0; run < params.num_runs; run++) {
                        result.reset();
                        functions.count(&result, benchmark_graphs[run], helper);
                    }
                    cycles = stop_tsc(cycles);

                }

                if (result.count != triangle_count) {
                    std::stringstream ss;
                    ss << "Count of triangles differs from the instrumented run! Count is: " << result.count << " expected: " << triangle_count;
                    //throw std::runtime_error(ss.str());
                }
                size_t cycle_per_run = cycles / params.num_runs;
                out_file << "," << cycle_per_run;
                std::cout << "  " << cycle_per_run << " cycles/run, "
                          << (double) op_counts[algo_name] / (double) cycle_per_run << " ops/cycle" << std::endl;

                // Restore the graphs
                if(!params.pre_sort_edge_lists) {
                    for (size_t i = 0; i < params.num_runs; i++) {
                        copy_graph(benchmark_graphs[i], benchmark_graph_original);
                    }
                }
                if (warmup_graph) {
                    copy_graph(warmup_graph, benchmark_graph_original);
                }
            }
            functions.free_helper(helper);
        }
        out_file << "\n";
    }
    std::cout << "All Benchmarking Completed" << std::endl;

    free_graph(benchmark_graph_original);
    if (warmup_graph) {
        free_graph(warmup_graph);
    }
    if(!params.pre_sort_edge_lists) {
        for (size_t i = 0; i < params.num_runs; i++) {
            free_graph(benchmark_graphs[i]);
        }
    }
}

#define RESET   "\033[0m"
#define RED     "\033[31m"

int main(int argc, char *argv[]) {
    try {
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
    } catch (const std::exception &e) {
        std::cerr << RED << "Error: " << e.what() << RESET << std::endl;
        return 1;
    }

}
