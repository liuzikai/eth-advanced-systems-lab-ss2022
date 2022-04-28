#include <iostream>
#include <fstream>


#include <string_view>
#include <functional>
#include <map>
#include <vector>

#include "BenchMarkingUtils.h"

#include "ArgParser.h"
#include "tsc_x86.h"

#include "../src/adjacency_graph.h"

#include "../src/forward.h"
#include "../src/forward_hashed.h"

static constexpr size_t default_num_warmups = 1;
static constexpr size_t default_num_runs = 5;
static constexpr size_t default_num_phases = 5;
static std::map<std::string, TrinagleFunctions> name_to_function =  {
    {"edge_iterator", TrinagleFunctions(edge_iterator, edge_iterator_get_dummy_helper)},
    {"forward", TrinagleFunctions(forward, forward_create_neighbor_container)},
    {"forward_hashed", TrinagleFunctions(forward_hashed, forward_hashed_create_neighbor_container)},
};


BenchParams parse_arguments(ArgParser& parser) {
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
  std::vector<std::string> algos = split(std::string (*algos_opt), ',');
  std::transform(algos.begin(), algos.end(), std::back_inserter(params.bench_mark_functions),
                 [](std::string s) -> std::pair<std::string_view, TrinagleFunctions> {
                   auto it = name_to_function.find(s);
                   if (it == name_to_function.end()) {
                     throw std::invalid_argument("Algorithm does not exist.");
                   } else {
                     return *it;
                   }
                });
  return params;
}

void benchmark(const BenchParams& params, std::ofstream& out_file) {
  // TODO: Read this in when we finally have data.
  adjacency_graph_t* graph = create_graph_from_file(params.graph_file.begin());

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
      out_file << algo_name << ", " << cycles << std::endl;
    }
  }
  std::cout << "All Benchmarking Completed" <<  std::endl;
}

int main(int argc, char * argv[]) {
  ArgParser parser(argc, argv);
  BenchParams params;
  try {
    params = parse_arguments(parser);;
  } catch (const std::invalid_argument& e) {
    std::cout << "Invalid argument: " << e.what() << std::endl;
    return 0;
  }

  std::ofstream out_file;
  out_file.open(std::string(params.file_name));
  out_file << "algorithm, num_runs" << std::endl;
  benchmark(params, out_file);
  out_file.close();
}
