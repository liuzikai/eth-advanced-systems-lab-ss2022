#ifndef _MSORT_ALL_H
#define _MSORT_ALL_H

#include "common.h"
#include "triangle_lister.h"
#include "adjacency_graph.h"

namespace merge_sort_base {
    template<class Index, class Counter = index_t, class TRL = TriangleListing::Count<Index>>
    TRL merge_sort_timing(AdjacencyGraph<Index> *G, void *dummy); 
}

namespace merge_sort_v2 {
    template<class Index, class Counter = index_t, class TRL = TriangleListing::Count<Index>>
    TRL merge_sort_timing(AdjacencyGraph<Index> *G, void *dummy); 
}

namespace merge_sort_v3 {
    template<class Index, class Counter = index_t, class TRL = TriangleListing::Count<Index>>
    TRL merge_sort_timing(AdjacencyGraph<Index> *G, void *dummy); 
}

namespace merge_sort_v4 {
    template<class Index, class Counter = index_t, class TRL = TriangleListing::Count<Index>>
    TRL merge_sort_timing(AdjacencyGraph<Index> *G, void *dummy); 
}


#endif