#ifndef ASL_TEAM02_BENCHMARK_UTILS_H
#define ASL_TEAM02_BENCHMARK_UTILS_H

#include <sstream>
#include <vector>

#include "adjacency_graph.h"
#include "edge_iterator.h"
#include "common.h"


template<class Index, class TLR>
struct TriangleFunctions {

    using count_triangles_fun = std::function<TLR(AdjacencyGraph<Index> *, void *)>;
    using get_helper_struct_fun = std::function<void *(AdjacencyGraph<Index> *)>;

    typedef TLR (*count_triangles_c_style)(AdjacencyGraph<Index> *, void *);

    typedef void *(*get_helper_c_style)(AdjacencyGraph<Index> *);

    template<class HelperStruct>
    TriangleFunctions(TLR (*c)(AdjacencyGraph<Index> *, HelperStruct *),
                      HelperStruct *(*h)(AdjacencyGraph<Index> *)) {
        // Ugh. This is disgusting...
        // But hey it works and its not undefined behaviour for compatible types which they should be.
        this->count = (count_triangles_c_style) (c);
        this->get_helper = (get_helper_c_style) (h);
    }

    count_triangles_fun count;
    get_helper_struct_fun get_helper;
    get_helper_struct_fun reset_helper;
};

std::vector<std::string> split(const std::string &s, char delim) {
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
    std::string file_name;
    std::string graph_file;
    std::vector<std::string> algos;
};


#endif //ASL_TEAM02_BENCHMARK_UTILS_H