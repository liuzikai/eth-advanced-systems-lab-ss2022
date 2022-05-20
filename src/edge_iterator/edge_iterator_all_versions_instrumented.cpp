#define INSTRUMENTED
#include "edge_iterator_base.hpp"
#include "edge_iterator_v1.hpp"
#include "edge_iterator_v2.hpp"
#include "instrumented_index.h"


namespace ei0 {
    template TriangleListing::Count<InstrumentedIndex> edge_iterator(AdjacencyGraph<InstrumentedIndex> *G, void *dummy = nullptr);
    template TriangleListing::Collect<InstrumentedIndex> edge_iterator(AdjacencyGraph<InstrumentedIndex> *G, void *dummy = nullptr);
}

namespace ei1 {
    template TriangleListing::Count<InstrumentedIndex> edge_iterator(AdjacencyGraph<InstrumentedIndex> *G, void *dummy = nullptr);
    template TriangleListing::Collect<InstrumentedIndex> edge_iterator(AdjacencyGraph<InstrumentedIndex> *G, void *dummy = nullptr);
}

namespace ei2 {
    template TriangleListing::Count<InstrumentedIndex> edge_iterator(AdjacencyGraph<InstrumentedIndex> *G, void *dummy = nullptr);
    template TriangleListing::Collect<InstrumentedIndex> edge_iterator(AdjacencyGraph<InstrumentedIndex> *G, void *dummy = nullptr);
}