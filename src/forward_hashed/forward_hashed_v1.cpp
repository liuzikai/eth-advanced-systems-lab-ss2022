#include "forward_hashed/forward_hashed_v1.hpp"

namespace fh1 {
    template struct ForwardHashedHelper<index_t>;

    template ForwardHashedHelper<index_t> *forward_hashed_create_neighbor_container(const AdjacencyGraph<index_t> *G);

    template void forward_hashed_delete_neighbor_container<index_t>(ForwardHashedHelper<index_t> *A);

    template TriangleListing::Count<index_t> forward_hashed(AdjacencyGraph<index_t> *G, ForwardHashedHelper<index_t> * A);
    template TriangleListing::Collect<index_t> forward_hashed(AdjacencyGraph<index_t> *G, ForwardHashedHelper<index_t> * A);
}
