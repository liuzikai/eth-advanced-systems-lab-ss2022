#include "forward_hashed/forward_hashed_v2.hpp"
#include "instrumented_index.h"

namespace fh2 {
    template struct ForwardHashedHelper<InstrumentedIndex>;

    template ForwardHashedHelper<InstrumentedIndex> *forward_hashed_create_neighbor_container(const AdjacencyGraph<InstrumentedIndex> *G);

    template void forward_hashed_delete_neighbor_container<InstrumentedIndex>(ForwardHashedHelper<InstrumentedIndex> *A);

    template TriangleListing::Count<InstrumentedIndex> forward_hashed(AdjacencyGraph<InstrumentedIndex> *G, ForwardHashedHelper<InstrumentedIndex> * A);
    template TriangleListing::Collect<InstrumentedIndex> forward_hashed(AdjacencyGraph<InstrumentedIndex> *G, ForwardHashedHelper<InstrumentedIndex> * A);
}