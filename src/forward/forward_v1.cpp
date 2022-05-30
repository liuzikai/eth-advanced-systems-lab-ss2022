#include "forward/forward_v1.hpp"

namespace f1 {
    template struct ForwardNeighborContainer<index_t, index_t>;

    template ForwardNeighborContainer<index_t> *forward_create_neighbor_container(const AdjacencyGraph<index_t> *G);

    template void forward_delete_neighbor_container<index_t, index_t>(ForwardNeighborContainer<index_t> *A);

    template void forward(TriangleListing::Count<index_t>* tlr, AdjacencyGraph<index_t> *G, ForwardNeighborContainer<index_t> * A);
    template void forward(TriangleListing::Collect<index_t>* tlr, AdjacencyGraph<index_t> *G, ForwardNeighborContainer<index_t> * A);
}
