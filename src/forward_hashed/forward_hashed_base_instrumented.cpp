#include "forward_hashed/forward_hashed_base.hpp"
#include "instrumented_index.h"


namespace fh0 {
    template struct ForwardHashedNeighborContainer<InstrumentedIndex>;

    template ForwardHashedNeighborContainer<InstrumentedIndex> *forward_hashed_create_neighbor_container(const AdjacencyGraph<InstrumentedIndex> *G);

    template void forward_hashed_delete_neighbor_container(ForwardHashedNeighborContainer<InstrumentedIndex> *A);

    template TriangleListing::Count<InstrumentedIndex> forward_hashed(AdjacencyGraph<InstrumentedIndex> *G, ForwardHashedNeighborContainer<InstrumentedIndex> * A);
    template TriangleListing::Collect<InstrumentedIndex> forward_hashed(AdjacencyGraph<InstrumentedIndex> *G, ForwardHashedNeighborContainer<InstrumentedIndex> * A);
}