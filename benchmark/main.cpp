#include <iostream>
#include <fstream>


#include <string_view>
#include <functional>
#include <map>
#include <vector>

#include "BenchMarkingUtils.h"
#include "../src/instrumented_index.h"

#include "ArgParser.h"
#include "tsc_x86.h"

#include "../graphs/adjacency_graph.h"

#include "../src/forward.h"
#include "../src/forward_hashed.h"

#include "../src/instrumented_index.h"

static constexpr size_t default_num_warmups = 1;
static constexpr size_t default_num_runs = 5;
static constexpr size_t default_num_phases = 5;

template <class Index>
class BenchmarkFunctions {
public:
  void add_functions(std::vector<std::string> names, std::vector<std::pair<std::string, TriangleFunctions<Index>>>& result) {
    std::transform(names.begin(), names.end(), std::back_inserter(result),
        [this](std::string s) -> std::pair<std::string, TriangleFunctions<Index>> {
          auto it = name_to_function.find(s);
          if (it == name_to_function.end()) {
            throw std::invalid_argument("Algorithm does not exist.");
          } else {
            return *it;
          }
      });
  }
private:
  std::map<std::string, TriangleFunctions<Index>> name_to_function = {
      {"edge_iterator", TriangleFunctions(edge_iterator<Index>, edge_iterator_get_dummy_helper)},
      {"forward", TriangleFunctions(forward<Index>, forward_create_neighbor_container)},
      {"forward_hashed", TriangleFunctions(forward_hashed<Index>, forward_hashed_create_neighbor_container)},
  };
};

template<class Index>
BenchParams<Index> parse_arguments(ArgParser& parser) {
  BenchParams<Index> params;
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
  std::vector<std::string> algos = split(std::string (*algos_opt), ',');
  BenchmarkFunctions<Index> benchmarkFunctions;
  benchmarkFunctions.add_functions(algos, params.bench_mark_functions);

  return params;
}

void benchmark(const BenchParams<index_t>& params, std::ofstream& out_file) {
  // TODO: Read this in when we finally have data.
  AdjacencyGraph<index_t>* graph = create_graph_from_file<index_t>(params.graph_file.c_str());

  std::cout << "num_warmups = " << params.num_warmups << std::endl;
  std::cout << "num_phases = " << params.num_phases << std::endl;
  std::cout << "num_runs = " << params.num_runs << std::endl;

  for (const auto&[algo_name, triangle_functions] : params.bench_mark_functions) {

    std::cout << "Benchmarking " << algo_name << std::endl;
    void* helper = triangle_functions.get_helper(graph);

    // Do the warmup runs:
    for (size_t i = 0; i < params.num_warmups; i++) {
      triangle_functions.count(graph, helper);
    }

    for (size_t phase = 0; phase < params.num_phases; phase++) {
      size_t cycles = start_tsc();
      for (size_t run = 0; run < params.num_runs; run++) {
        triangle_functions.count(graph, helper);
      }
      cycles = stop_tsc(cycles);
      out_file << algo_name << "," << cycles / params.num_runs << std::endl;
    }
  }
  std::cout << "All Benchmarking Completed" <<  std::endl;
}

static void run_instrumented(ArgParser& parser) {
  BenchParams<InstrumentedIndex> params;
  params = parse_arguments<InstrumentedIndex>(parser);;

  std::ofstream out_file;
  out_file.open(std::string(params.file_name));
  out_file << "algorithm,op_count_per_run" << std::endl;

  AdjacencyGraph<InstrumentedIndex>* graph = create_graph_from_file<InstrumentedIndex>(params.graph_file.c_str());

  for (const auto&[algo_name, triangle_functions] : params.bench_mark_functions) {
    std::cout << "Instrumented run: " << algo_name << std::endl;
    OpCounter::ResetOpCount();
    void* helper = triangle_functions.get_helper(graph);
    triangle_functions.count(graph, helper);
    out_file << algo_name << "," << OpCounter::GetOpCount() << std::endl;
  }
  std::cout << "All instrumenting Completed" <<  std::endl;
}

static void run_benchmark(ArgParser& parser) {
  BenchParams<index_t> params;
  params = parse_arguments<index_t>(parser);;
  std::ofstream out_file;
  out_file.open(std::string(params.file_name));
  out_file << "algorithm,cycles_per_run" << std::endl;
  benchmark(params, out_file);
  out_file.close();
}



int main(int argc, char * argv[]) {
  ArgParser parser(argc, argv);
  bool run_inst = parser.cmdOptionExists("-instrumented");
  try {
    if(run_inst) {
      run_instrumented(parser);
    } else {
      run_benchmark(parser);
    }
  } catch (const std::invalid_argument& e) {
    std::cout << "Invalid argument: " << e.what() << std::endl;
  }
}
