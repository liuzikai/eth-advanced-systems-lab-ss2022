#define INSTRUMENTED
#include <forward/forward_base.hpp>
#include <forward/forward_v1.hpp>
#include <forward/forward_v2.hpp>
#include <forward/forward_v3.hpp>
#include <forward/forward_v4.hpp>
#include <forward/forward_v5.hpp>
#include <forward/forward_va.hpp>
#include "instrumented_index.h"


namespace f0 {

    template struct ForwardNeighborContainer<InstrumentedIndex>;

    template ForwardNeighborContainer<InstrumentedIndex> *forward_create_neighbor_container(const AdjacencyGraph<InstrumentedIndex> *G);

    template void forward_delete_neighbor_container(ForwardNeighborContainer<InstrumentedIndex> *A);

    template void forward(TriangleListing::Count<InstrumentedIndex>* tlr, AdjacencyGraph<InstrumentedIndex> *G, ForwardNeighborContainer<InstrumentedIndex> * A);
    template void forward(TriangleListing::SetCollect<InstrumentedIndex>* tlr, AdjacencyGraph<InstrumentedIndex> *G, ForwardNeighborContainer<InstrumentedIndex> * A);

}


namespace f1 {

    template struct ForwardNeighborContainer<InstrumentedIndex>;

    template ForwardNeighborContainer<InstrumentedIndex> *forward_create_neighbor_container(const AdjacencyGraph<InstrumentedIndex> *G);

    template void forward_delete_neighbor_container(ForwardNeighborContainer<InstrumentedIndex> *A);

    template void forward(TriangleListing::Count<InstrumentedIndex>* tlr, AdjacencyGraph<InstrumentedIndex> *G, ForwardNeighborContainer<InstrumentedIndex> * A);
    template void forward(TriangleListing::SetCollect<InstrumentedIndex>* tlr, AdjacencyGraph<InstrumentedIndex> *G, ForwardNeighborContainer<InstrumentedIndex> * A);

}

namespace f2 {

    template struct ForwardNeighborContainer<InstrumentedIndex>;

    template ForwardNeighborContainer<InstrumentedIndex> *forward_create_neighbor_container(const AdjacencyGraph<InstrumentedIndex> *G);

    template void forward_delete_neighbor_container(ForwardNeighborContainer<InstrumentedIndex> *A);

    template void forward(TriangleListing::Count<InstrumentedIndex>* tlr, AdjacencyGraph<InstrumentedIndex> *G, ForwardNeighborContainer<InstrumentedIndex> * A);
    template void forward(TriangleListing::SetCollect<InstrumentedIndex>* tlr, AdjacencyGraph<InstrumentedIndex> *G, ForwardNeighborContainer<InstrumentedIndex> * A);

}

namespace fa {
    
    template struct ForwardNeighborContainer<InstrumentedIndex>;

    template ForwardNeighborContainer<InstrumentedIndex> *forward_create_neighbor_container(const AdjacencyGraph<InstrumentedIndex> *G);

    template void forward_delete_neighbor_container(ForwardNeighborContainer<InstrumentedIndex> *A);

    template void forward(TriangleListing::Count<InstrumentedIndex>* tlr, AdjacencyGraph<InstrumentedIndex> *G, ForwardNeighborContainer<InstrumentedIndex> * A);
    template void forward(TriangleListing::SetCollect<InstrumentedIndex>* tlr, AdjacencyGraph<InstrumentedIndex> *G, ForwardNeighborContainer<InstrumentedIndex> * A);

}
namespace f3 {
    
    template struct ForwardNeighborContainer<InstrumentedIndex>;

    template ForwardNeighborContainer<InstrumentedIndex> *forward_create_neighbor_container(const AdjacencyGraph<InstrumentedIndex> *G);

    template void forward_delete_neighbor_container(ForwardNeighborContainer<InstrumentedIndex> *A);

    template void forward(TriangleListing::Count<InstrumentedIndex>* tlr, AdjacencyGraph<InstrumentedIndex> *G, ForwardNeighborContainer<InstrumentedIndex> * A);
    template void forward(TriangleListing::SetCollect<InstrumentedIndex>* tlr, AdjacencyGraph<InstrumentedIndex> *G, ForwardNeighborContainer<InstrumentedIndex> * A);

}


namespace f4 {
    
    template struct ForwardNeighborContainer<InstrumentedIndex>;

    template ForwardNeighborContainer<InstrumentedIndex> *forward_create_neighbor_container(const AdjacencyGraph<InstrumentedIndex> *G);

    template void forward_delete_neighbor_container(ForwardNeighborContainer<InstrumentedIndex> *A);

    template void forward(TriangleListing::Count<InstrumentedIndex>* tlr, AdjacencyGraph<InstrumentedIndex> *G, ForwardNeighborContainer<InstrumentedIndex> * A);
    template void forward(TriangleListing::SetCollect<InstrumentedIndex>* tlr, AdjacencyGraph<InstrumentedIndex> *G, ForwardNeighborContainer<InstrumentedIndex> * A);

}

namespace f5 {
    
    template struct ForwardNeighborContainer<InstrumentedIndex>;

    template ForwardNeighborContainer<InstrumentedIndex> *forward_create_neighbor_container(const AdjacencyGraph<InstrumentedIndex> *G);

    template void forward_delete_neighbor_container(ForwardNeighborContainer<InstrumentedIndex> *A);

    template void forward(TriangleListing::Count<InstrumentedIndex>* tlr, AdjacencyGraph<InstrumentedIndex> *G, ForwardNeighborContainer<InstrumentedIndex> * A);
    template void forward(TriangleListing::SetCollect<InstrumentedIndex>* tlr, AdjacencyGraph<InstrumentedIndex> *G, ForwardNeighborContainer<InstrumentedIndex> * A);

}
