#include "forward_hashed/forward_hashed_base.hpp"

namespace forward_hashed_base {
    template struct ForwardHashedNeighborContainer<index_t, index_t>;

    template ForwardHashedNeighborContainer<index_t> *forward_hashed_create_neighbor_container(const AdjacencyGraph<index_t> *G);

    template void forward_hashed_delete_neighbor_container<index_t, index_t>(ForwardHashedNeighborContainer<index_t> *A);

    template TriangleListing::Count<index_t> forward_hashed(AdjacencyGraph<index_t> *G, ForwardHashedNeighborContainer<index_t> * A);
    template TriangleListing::Collect<index_t> forward_hashed(AdjacencyGraph<index_t> *G, ForwardHashedNeighborContainer<index_t> * A);
}
