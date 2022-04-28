#ifndef TEAM02_EDGE_ITER_H
#define TEAM02_EDGE_ITER_H

#include "common.h"
#include "adjacency_graph.h"

void* edge_iterator_get_dummy_helper(adjacency_graph_t *);

index_t edge_iterator(adjacency_graph_t *G, void* dummy = nullptr);

#endif //TEAM02_EDGE_ITER_H
