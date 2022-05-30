#include "forward_hashed/forward_hashed_base.hpp"

namespace fh0 {
    template struct ForwardHashedNeighborContainer<index_t, index_t>;

    template ForwardHashedNeighborContainer<index_t> *forward_hashed_create_neighbor_container(const AdjacencyGraph<index_t> *G);

    template void forward_hashed_delete_neighbor_container<index_t, index_t>(ForwardHashedNeighborContainer<index_t> *A);

    template void forward_hashed(TriangleListing::Count<index_t>* tlr, AdjacencyGraph<index_t> *G, ForwardHashedNeighborContainer<index_t> * A);
    template void forward_hashed(TriangleListing::Collect<index_t>* tlr, AdjacencyGraph<index_t> *G, ForwardHashedNeighborContainer<index_t> * A);
}
