#ifndef ASL_TEAM02_BENCHMARK_UTILS_H
#define ASL_TEAM02_BENCHMARK_UTILS_H

#include <sstream>
#include <vector>

#include "adjacency_graph.h"
#include "common.h"


template<class Index, class Counter, class TLR>
struct TriangleFunctions {

    using Graph = AdjacencyGraph<Index, Counter>;
    using count_triangles_fun = std::function<TLR(Graph *, void *)>;
    using get_helper_struct_fun = std::function<void *(const Graph *)>;
    using free_helper_struct_fun = std::function<void(void *)>;

    typedef TLR (*count_triangles_c_style)(Graph *, void *);

    typedef void *(*get_helper_c_style)(const Graph *);
    typedef void (*free_helper_c_style)(void *);

    template<class HelperStruct>
    TriangleFunctions(TLR (*c)(Graph *, HelperStruct *),
                      HelperStruct *(*h)(const Graph *),
                      void(*f)(HelperStruct *)) {
        // Ugh. This is disgusting...
        // But hey it works and its not undefined behaviour for compatible types which they should be.
        this->count = (count_triangles_c_style) (c);
        this->get_helper = (get_helper_c_style) (h);
        this->free_helper = (free_helper_c_style) (f);
    }

    count_triangles_fun count;
    get_helper_struct_fun get_helper;
    free_helper_struct_fun free_helper;
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
