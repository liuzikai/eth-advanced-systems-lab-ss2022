#ifndef _EDGE_ITER_ALL_H
#define _EDGE_ITER_ALL_H

#include "common.h"
#include "triangle_lister.h"
#include "adjacency_graph.h"


namespace ei0 {
    template<class Index, class Counter = index_t, class TRL = TriangleListing::Count<Index>>
    void edge_iterator(TRL* lister,AdjacencyGraph<Index> *G, void *dummy = nullptr);
}

namespace ei1 {
    template<class Index, class Counter = index_t, class TRL = TriangleListing::Count<Index>>
    void edge_iterator(TRL* lister,AdjacencyGraph<Index> *G, void *dummy = nullptr);
}

namespace ei2 {
    template<class Index, class Counter = index_t, class TRL = TriangleListing::Count<Index>>
    void edge_iterator(TRL* lister,AdjacencyGraph<Index> *G, void *dummy = nullptr);
}

namespace ei3 {
    template<class Index, class Counter = index_t, class TRL = TriangleListing::Count<Index>>
    void edge_iterator(TRL* lister,AdjacencyGraph<Index> *G, void *dummy = nullptr);
}

namespace eu4 {
    template<class Index, class Counter = index_t, class TRL = TriangleListing::Count<Index>>
    void edge_iterator(TRL* lister,AdjacencyGraph<Index> *G, void *dummy = nullptr);
}

namespace eia {
    template<class Index, class Counter = index_t, class TRL = TriangleListing::Count<Index>>
    void edge_iterator(TRL* lister,AdjacencyGraph<Index> *G, void *dummy = nullptr);
}
#endif //_EDGE_ITER_ALL_H
