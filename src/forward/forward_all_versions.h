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
    void forward(TRL* lister,AdjacencyGraph<Index> *G, ForwardNeighborContainer<Index> *A);
}


namespace f1 {

    template<class Index, class Counter = index_t>
    struct ForwardNeighborContainer;

    template<class Index, class Counter = index_t>
    ForwardNeighborContainer<Index> *forward_create_neighbor_container(const AdjacencyGraph<Index> *G);

    template<class Index, class Counter = index_t>
    void forward_delete_neighbor_container(ForwardNeighborContainer<Index> *A);

    template<class Index, class Counter = index_t, class TRL = TriangleListing::Count<Index>>
    void forward(TRL* lister,AdjacencyGraph<Index> *G, ForwardNeighborContainer<Index> *A);
}

namespace f2 {

    template<class Index, class Counter = index_t>
    struct ForwardNeighborContainer;

    template<class Index, class Counter = index_t>
    ForwardNeighborContainer<Index> *forward_create_neighbor_container(const AdjacencyGraph<Index> *G);

    template<class Index, class Counter = index_t>
    void forward_delete_neighbor_container(ForwardNeighborContainer<Index> *A);

    template<class Index, class Counter = index_t, class TRL = TriangleListing::Count<Index>>
    void forward(TRL* lister,AdjacencyGraph<Index> *G, ForwardNeighborContainer<Index> *A);
}

namespace f3 {

    template<class Index, class Counter = index_t>
    struct ForwardNeighborContainer;

    template<class Index, class Counter = index_t>
    ForwardNeighborContainer<Index> *forward_create_neighbor_container(const AdjacencyGraph<Index> *G);

    template<class Index, class Counter = index_t>
    void forward_delete_neighbor_container(ForwardNeighborContainer<Index> *A);

    template<class Index, class Counter = index_t, class TRL = TriangleListing::Count<Index>>
    void forward(TRL* lister,AdjacencyGraph<Index> *G, ForwardNeighborContainer<Index> *A);
}

namespace f4 {

    template<class Index, class Counter = index_t>
    struct ForwardNeighborContainer;

    template<class Index, class Counter = index_t>
    ForwardNeighborContainer<Index> *forward_create_neighbor_container(const AdjacencyGraph<Index> *G);

    template<class Index, class Counter = index_t>
    void forward_delete_neighbor_container(ForwardNeighborContainer<Index> *A);

    template<class Index, class Counter = index_t, class TRL = TriangleListing::Count<Index>>
    void forward(TRL* lister,AdjacencyGraph<Index> *G, ForwardNeighborContainer<Index> *A);
}

namespace f5 {

    template<class Index, class Counter = index_t>
    struct ForwardNeighborContainer;

    template<class Index, class Counter = index_t>
    ForwardNeighborContainer<Index> *forward_create_neighbor_container(const AdjacencyGraph<Index> *G);

    template<class Index, class Counter = index_t>
    void forward_delete_neighbor_container(ForwardNeighborContainer<Index> *A);

    template<class Index, class Counter = index_t, class TRL = TriangleListing::Count<Index>>
    void forward(TRL* lister,AdjacencyGraph<Index> *G, ForwardNeighborContainer<Index> *A);
}


namespace fu4 {

    template<class Index, class Counter = index_t>
    struct ForwardNeighborContainer;

    template<class Index, class Counter = index_t>
    ForwardNeighborContainer<Index> *forward_create_neighbor_container(const AdjacencyGraph<Index> *G);

    template<class Index, class Counter = index_t>
    void forward_delete_neighbor_container(ForwardNeighborContainer<Index> *A);

    template<class Index, class Counter = index_t, class TRL = TriangleListing::Count<Index>>
    void forward(TRL* lister,AdjacencyGraph<Index> *G, ForwardNeighborContainer<Index> *A);
}

namespace fa {

    template<class Index, class Counter = index_t>
    struct ForwardNeighborContainer;

    template<class Index, class Counter = index_t>
    ForwardNeighborContainer<Index> *forward_create_neighbor_container(const AdjacencyGraph<Index> *G);

    template<class Index, class Counter = index_t>
    void forward_delete_neighbor_container(ForwardNeighborContainer<Index> *A);

    template<class Index, class Counter = index_t, class TRL = TriangleListing::Count<Index>>
    void forward(TRL* lister,AdjacencyGraph<Index> *G, ForwardNeighborContainer<Index> *A);
}


namespace fpi {

    template<class Index, class Counter = index_t>
    struct ForwardNeighborContainer;

    template<class Index, class Counter = index_t>
    ForwardNeighborContainer<Index> *forward_create_neighbor_container(const AdjacencyGraph<Index> *G);

    template<class Index, class Counter = index_t>
    void forward_delete_neighbor_container(ForwardNeighborContainer<Index> *A);

    template<class Index, class Counter = index_t, class TRL = TriangleListing::Count<Index>>
    void forward(TRL* lister,AdjacencyGraph<Index> *G, ForwardNeighborContainer<Index> *A);
}

namespace f6 {

    template<class Index, class Counter = index_t>
    struct ForwardNeighborContainer;

    template<class Index, class Counter = index_t>
    ForwardNeighborContainer<Index> *forward_create_neighbor_container(const AdjacencyGraph<Index> *G);

    template<class Index, class Counter = index_t>
    void forward_delete_neighbor_container(ForwardNeighborContainer<Index> *A);

    template<class Index, class Counter = index_t, class TRL = TriangleListing::Count<Index>>
    void forward(TRL* lister,AdjacencyGraph<Index> *G, ForwardNeighborContainer<Index> *A);
}

#endif //_FORWARD_ALL_H
