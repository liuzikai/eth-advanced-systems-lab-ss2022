#ifndef ASL_TEAM02_BENCHMARKINGUTILS_H
#define ASL_TEAM02_BENCHMARKINGUTILS_H

#include <sstream>
#include <vector>


#include "../graphs/adjacency_graph.h"
#include "../src/edge_iterator.h"
#include "../src/common.h"

/*
namespace {

}
*/


template<class Index>
struct TriangleFunctions {

  using count_triangles_fun = std::function<index_t(AdjacencyGraph<Index>*, void*)>;
  using get_helper_struct_fun = std::function<void*(AdjacencyGraph<Index>*)>;

  typedef index_t (*count_triangles_c_style)(AdjacencyGraph<Index>*, void*);
  typedef void* (*get_helper_c_style)(AdjacencyGraph<Index>*);

  template<class HelperStruct>
  TriangleFunctions(index_t (*c)(AdjacencyGraph<Index>*, HelperStruct*),
  HelperStruct* (*h)(AdjacencyGraph<Index>*) ) {
    // Ugh. This is disgusting...
    // But hey it works and its not undefined behaviour for compatible types which they should be.
    this->count = (count_triangles_c_style)(c);
    this->get_helper = (get_helper_c_style)(h);
  }

  count_triangles_fun count;
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

template<class Index>
struct BenchParams {
    size_t num_warmups;
    size_t num_runs;
    size_t num_phases;
    std::string file_name;
    std::string graph_file;
    std::vector<std::pair<std::string, TriangleFunctions<Index>>> bench_mark_functions;
};


#endif //ASL_TEAM02_BENCHMARKINGUTILS_H
