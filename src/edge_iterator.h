#ifndef TEAM02_EDGE_ITER_H
#define TEAM02_EDGE_ITER_H

#include "common.h"
#include "adjacency_graph.h"

template<class Index>
void* edge_iterator_get_dummy_helper(AdjacencyGraph<Index> *);

template<class Index>
index_t edge_iterator(AdjacencyGraph<Index> *G, void* dummy = nullptr);

#endif //TEAM02_EDGE_ITER_H
