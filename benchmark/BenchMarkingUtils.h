//
// Created by matthiaas on 26.03.22.
//

#ifndef ASL_TEAM02_BENCHMARKINGUTILS_H
#define ASL_TEAM02_BENCHMARKINGUTILS_H

#include <sstream>
#include <vector>

extern "C" {
#include "../src/adjacency_graph.h"
#include "../src/edge_iterator.h"
#include "../src/common.h"
}

using count_tringangles_fun = std::function<index_t(adjacency_graph_t*)>;

std::vector<std::string>  split(const std::string &s, char delim) {
  std::vector<std::string> elems;
  std::istringstream iss(s);
  std::string item;
  while (std::getline(iss, item, delim)) {
    elems.push_back(item);
  }
  return elems;
}


struct BenchParams {
    size_t num_warmups;
    size_t num_runs;
    size_t num_phases;
    std::string_view file_name;
    std::vector<std::pair<std::string_view, count_tringangles_fun>> bench_mark_functions;
};


#endif //ASL_TEAM02_BENCHMARKINGUTILS_H
