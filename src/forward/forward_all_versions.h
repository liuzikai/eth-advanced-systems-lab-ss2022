#ifndef _FORWARD_ALL_H
#define _FORWARD_ALL_H

#include "common.h"
#include "triangle_lister.h"
#include "adjacency_graph.h"


namespace f0 {

    template<class Index, class Counter = index_t>
    struct ForwardNeighborContainer;

    template<class Index, class Counter = index_t>
    ForwardNeighborContainer<Index> *forward_create_neighbor_container(const AdjacencyGraph<Index> *G);

    template<class Index, class Counter = index_t>
    void forward_delete_neighbor_container(ForwardNeighborContainer<Index> *A);

    template<class Index, class Counter = index_t, class TRL = TriangleListing::Count<Index>>
    TRL forward(AdjacencyGraph<Index> *G, ForwardNeighborContainer<Index> *A);
}


namespace f1 {

    template<class Index, class Counter = index_t>
    struct ForwardNeighborContainer;

    template<class Index, class Counter = index_t>
    ForwardNeighborContainer<Index> *forward_create_neighbor_container(const AdjacencyGraph<Index> *G);

    template<class Index, class Counter = index_t>
    void forward_delete_neighbor_container(ForwardNeighborContainer<Index> *A);

    template<class Index, class Counter = index_t, class TRL = TriangleListing::Count<Index>>
    TRL forward(AdjacencyGraph<Index> *G, ForwardNeighborContainer<Index> *A);
}

#endif //_FORWARD_ALL_H