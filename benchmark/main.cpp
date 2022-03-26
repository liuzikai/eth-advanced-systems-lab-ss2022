#include <iostream>
#include <fstream>

#include "ArgParser.h"
#include <string_view>
#include <functional>
#include <map>
#include <vector>

#include "BenchMarkingUtils.h"

#include "../third_party/tsc_x86.h"

static constexpr size_t default_num_warmups = 1;
static constexpr size_t default_num_runs = 5;
static constexpr size_t default_num_phases = 5;
static std::map<std::string, count_tringangles_fun> name_to_function =  {
    {"edge_iterator", edge_iterator}
};


BenchParams parse_arguments(ArgParser& parser) {
  BenchParams params;
  params.num_warmups = parser.getCmdOptionAsInt("-num_warmups").value_or(default_num_warmups);
  params.num_runs = parser.getCmdOptionAsInt("-num_runs").value_or(default_num_runs);
  params.num_phases = parser.getCmdOptionAsInt("-num_phases").value_or(default_num_phases);
  params.file_name = parser.getCmdOption("-o").value_or("default.csv");

  // TODO: Maybe we want to benchmark time later as well?
  // std::string_view timing_method_string = parser.getCmdOption("-timing").value_or("cycles");

  std::optional<std::string_view> algos_opt = parser.getCmdOption("-algorithm");
  if (!algos_opt) {
    throw std::invalid_argument("No algorithm specified.");
  }
  // Sadly ranges to split the view are not available in gcc yet.
  std::vector<std::string> algos = split(std::string (*algos_opt), ',');
  std::transform(algos.begin(), algos.end(), std::back_inserter(params.bench_mark_functions),
                 [](std::string s) -> std::pair<std::string_view, count_tringangles_fun> {
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
  adjacency_graph_t* data = NULL;

  for (const auto&[algo_name, algo_function] : params.bench_mark_functions) {

    std::cout << "Benchmarking " << algo_name << std::endl;
    // Do the warmup runs:
    for (size_t i = 0; i < params.num_warmups; i++) {
      algo_function(data);
    }
    for (size_t phase = 0; phase < params.num_phases; phase++) {
      size_t cycles = start_tsc();
      for (size_t run = 0; run < params.num_runs; run++) {
        algo_function(data);
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
