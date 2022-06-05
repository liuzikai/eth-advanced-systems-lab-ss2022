#include "forward_hashed/forward_hashed_v4a.hpp"

namespace fh4a {
    template struct ForwardHashedHelper<index_t>;

    template ForwardHashedHelper<index_t> *forward_hashed_create_neighbor_container(const AdjacencyGraph<index_t> *G);

    template void forward_hashed_delete_neighbor_container<index_t>(ForwardHashedHelper<index_t> *A);

    template void forward_hashed(TriangleListing::Count<index_t>* tlr, AdjacencyGraph<index_t> *G, ForwardHashedHelper<index_t> * A);
    template void forward_hashed(TriangleListing::Collect<index_t>* tlr, AdjacencyGraph<index_t> *G, ForwardHashedHelper<index_t> * A);
}
