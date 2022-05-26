#include "forward_hashed/forward_hashed_base.hpp"
#include "forward_hashed/forward_hashed_v1.hpp"
#include "forward_hashed/forward_hashed_v2.hpp"
#include "forward_hashed/forward_hashed_v3.hpp"
#include "forward_hashed/forward_hashed_v4.hpp"
#include "instrumented_index.h"

namespace fh0 {
    template struct ForwardHashedNeighborContainer<InstrumentedIndex>;

    template ForwardHashedNeighborContainer<InstrumentedIndex> *forward_hashed_create_neighbor_container(const AdjacencyGraph<InstrumentedIndex> *G);

    template void forward_hashed_delete_neighbor_container(ForwardHashedNeighborContainer<InstrumentedIndex> *A);

    template TriangleListing::Count<InstrumentedIndex> forward_hashed(AdjacencyGraph<InstrumentedIndex> *G, ForwardHashedNeighborContainer<InstrumentedIndex> * A);
    template TriangleListing::Collect<InstrumentedIndex> forward_hashed(AdjacencyGraph<InstrumentedIndex> *G, ForwardHashedNeighborContainer<InstrumentedIndex> * A);
}

namespace fh1 {
    template struct ForwardHashedHelper<InstrumentedIndex>;

    template ForwardHashedHelper<InstrumentedIndex> *forward_hashed_create_neighbor_container(const AdjacencyGraph<InstrumentedIndex> *G);

    template void forward_hashed_delete_neighbor_container<InstrumentedIndex>(ForwardHashedHelper<InstrumentedIndex> *A);

    template TriangleListing::Count<InstrumentedIndex> forward_hashed(AdjacencyGraph<InstrumentedIndex> *G, ForwardHashedHelper<InstrumentedIndex> * A);
    template TriangleListing::Collect<InstrumentedIndex> forward_hashed(AdjacencyGraph<InstrumentedIndex> *G, ForwardHashedHelper<InstrumentedIndex> * A);
}

namespace fh2 {
    template struct ForwardHashedHelper<InstrumentedIndex>;

    template ForwardHashedHelper<InstrumentedIndex> *forward_hashed_create_neighbor_container(const AdjacencyGraph<InstrumentedIndex> *G);

    template void forward_hashed_delete_neighbor_container<InstrumentedIndex>(ForwardHashedHelper<InstrumentedIndex> *A);

    template TriangleListing::Count<InstrumentedIndex> forward_hashed(AdjacencyGraph<InstrumentedIndex> *G, ForwardHashedHelper<InstrumentedIndex> * A);
    template TriangleListing::Collect<InstrumentedIndex> forward_hashed(AdjacencyGraph<InstrumentedIndex> *G, ForwardHashedHelper<InstrumentedIndex> * A);
}

namespace fh3 {
    template struct ForwardHashedHelper<InstrumentedIndex>;

    template ForwardHashedHelper<InstrumentedIndex> *forward_hashed_create_neighbor_container(const AdjacencyGraph<InstrumentedIndex> *G);

    template void forward_hashed_delete_neighbor_container<InstrumentedIndex>(ForwardHashedHelper<InstrumentedIndex> *A);

    template TriangleListing::Count<InstrumentedIndex> forward_hashed(AdjacencyGraph<InstrumentedIndex> *G, ForwardHashedHelper<InstrumentedIndex> * A);
    template TriangleListing::Collect<InstrumentedIndex> forward_hashed(AdjacencyGraph<InstrumentedIndex> *G, ForwardHashedHelper<InstrumentedIndex> * A);
}

namespace fh4 {
    template struct ForwardHashedHelper<InstrumentedIndex>;

    template ForwardHashedHelper<InstrumentedIndex> *forward_hashed_create_neighbor_container(const AdjacencyGraph<InstrumentedIndex> *G);

    template void forward_hashed_delete_neighbor_container<InstrumentedIndex>(ForwardHashedHelper<InstrumentedIndex> *A);

    template TriangleListing::Count<InstrumentedIndex> forward_hashed(AdjacencyGraph<InstrumentedIndex> *G, ForwardHashedHelper<InstrumentedIndex> * A);
    template TriangleListing::Collect<InstrumentedIndex> forward_hashed(AdjacencyGraph<InstrumentedIndex> *G, ForwardHashedHelper<InstrumentedIndex> * A);
}
