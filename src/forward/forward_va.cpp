#include "forward/forward_va.hpp"

namespace fa {
    template struct ForwardNeighborContainer<index_t, index_t>;

    template ForwardNeighborContainer<index_t> *forward_create_neighbor_container(const AdjacencyGraph<index_t> *G);

    template void forward_delete_neighbor_container<index_t, index_t>(ForwardNeighborContainer<index_t> *A);

    template TriangleListing::Count<index_t> forward(AdjacencyGraph<index_t> *G, ForwardNeighborContainer<index_t> * A);
    template TriangleListing::Collect<index_t> forward(AdjacencyGraph<index_t> *G, ForwardNeighborContainer<index_t> * A);
}
