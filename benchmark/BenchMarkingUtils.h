#ifndef ASL_TEAM02_BENCHMARKINGUTILS_H
#define ASL_TEAM02_BENCHMARKINGUTILS_H

#include <sstream>
#include <vector>


#include "../src/adjacency_graph.h"
#include "../src/edge_iterator.h"
#include "../src/common.h"

namespace {
  typedef index_t (*count_triangles_c_style)(adjacency_graph_t*, void*);
  typedef void* (*get_helper_c_style)(adjacency_graph_t*);
}

using count_tringangles_fun = std::function<index_t(adjacency_graph_t*, void*)>;
using get_helper_struct_fun = std::function<void*(adjacency_graph_t*)>;

struct TrinagleFunctions {
  
  template<class HelperStruct>
  TrinagleFunctions(index_t (*c)(adjacency_graph_t*, HelperStruct*), HelperStruct* (*h)(adjacency_graph_t*) ) {
    // Ugh. This is disgusting... 
    // But hey it works and its not undefined behaviour for compatible types which they should be.
    this->count = (count_triangles_c_style)(c);
    this->get_helper = (get_helper_c_style)(h);
  }

  count_tringangles_fun count;
  get_helper_struct_fun get_helper;
  get_helper_struct_fun reset_helper;
};

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
    std::string_view graph_file;
    std::vector<std::pair<std::string_view, TrinagleFunctions>> bench_mark_functions;
};


#endif //ASL_TEAM02_BENCHMARKINGUTILS_H
