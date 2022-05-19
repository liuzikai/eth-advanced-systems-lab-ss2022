#ifndef _EDGE_ITER_ALL_H
#define _EDGE_ITER_ALL_H

#include "common.h"
#include "triangle_lister.h"
#include "adjacency_graph.h"


namespace ei0 {
    template<class Index, class Counter = index_t, class TRL = TriangleListing::Count<Index>>
    TRL edge_iterator(AdjacencyGraph<Index> *G, void *dummy = nullptr);
}

#endif //_EDGE_ITER_ALL_H