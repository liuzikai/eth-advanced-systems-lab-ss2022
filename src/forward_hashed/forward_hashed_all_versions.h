#ifndef _FORWARD_HASHED_ALL_H
#define _FORWARD_HASHED_ALL_H

#include "common.h"
#include "triangle_lister.h"
#include "adjacency_graph.h"


namespace fh0 {

    template<class Index, class Counter = index_t>
    struct ForwardHashedNeighborContainer;

    template<class Index, class Counter = index_t>
    ForwardHashedNeighborContainer<Index> *forward_hashed_create_neighbor_container(const AdjacencyGraph<Index> *G);

    template<class Index, class Counter = index_t>
    void forward_hashed_delete_neighbor_container(ForwardHashedNeighborContainer<Index> *A);
    

    template<class Index, class Counter = index_t, class TRL = TriangleListing::Count<Index>>
    TRL forward_hashed(AdjacencyGraph<Index> *G, ForwardHashedNeighborContainer<Index> *A);
}

namespace fh1 {

    template<class Index, class Counter = index_t>
    struct ForwardHashedHelper;

    template<class Index, class Counter = index_t>
    ForwardHashedHelper<Index> *forward_hashed_create_neighbor_container(const AdjacencyGraph<Index> *G);

    template<class Index, class Counter = index_t>
    void forward_hashed_delete_neighbor_container(ForwardHashedHelper<Index> *A);

    template<class Index, class Counter = index_t, class TRL = TriangleListing::Count<Index>>
    TRL forward_hashed(AdjacencyGraph<Index> *G, ForwardHashedHelper<Index> *A);
}

namespace fh2 {

    template<class Index, class Counter = index_t>
    struct ForwardHashedHelper;

    template<class Index, class Counter = index_t>
    ForwardHashedHelper<Index> *forward_hashed_create_neighbor_container(const AdjacencyGraph<Index> *G);

    template<class Index, class Counter = index_t>
    void forward_hashed_delete_neighbor_container(ForwardHashedHelper<Index> *A);

    template<class Index, class Counter = index_t, class TRL = TriangleListing::Count<Index>>
    TRL forward_hashed(AdjacencyGraph<Index> *G, ForwardHashedHelper<Index> *A);
}

namespace fh3 {

    template<class Index, class Counter = index_t>
    struct ForwardHashedHelper;

    template<class Index, class Counter = index_t>
    ForwardHashedHelper<Index> *forward_hashed_create_neighbor_container(const AdjacencyGraph<Index> *G);

    template<class Index, class Counter = index_t>
    void forward_hashed_delete_neighbor_container(ForwardHashedHelper<Index> *A);

    template<class Index, class Counter = index_t, class TRL = TriangleListing::Count<Index>>
    TRL forward_hashed(AdjacencyGraph<Index> *G, ForwardHashedHelper<Index> *A);
}

namespace fh4 {

    template<class Index, class Counter = index_t>
    struct ForwardHashedHelper;

    template<class Index, class Counter = index_t>
    ForwardHashedHelper<Index> *forward_hashed_create_neighbor_container(const AdjacencyGraph<Index> *G);

    template<class Index, class Counter = index_t>
    void forward_hashed_delete_neighbor_container(ForwardHashedHelper<Index> *A);

    template<class Index, class Counter = index_t, class TRL = TriangleListing::Count<Index>>
    TRL forward_hashed(AdjacencyGraph<Index> *G, ForwardHashedHelper<Index> *A);
}

#endif //_FORWARD_ALL_H