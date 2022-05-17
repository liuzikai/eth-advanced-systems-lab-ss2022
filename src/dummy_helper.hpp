#ifndef TEAM02_DUMMY_HELPER_H
#define TEAM02_DUMMY_HELPER_H
#include "common.h"
#include "adjacency_graph.h"

template<class Index, class Counter = index_t>
void *get_dummy_helper(const AdjacencyGraph<Index> *) {
    return nullptr;
}


#endif