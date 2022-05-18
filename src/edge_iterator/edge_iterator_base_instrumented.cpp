#include <edge_iterator/edge_iterator_base.hpp>
#include "instrumented_index.h"


namespace edge_iterator_base {
    template TriangleListing::Count<InstrumentedIndex> edge_iterator(AdjacencyGraph<InstrumentedIndex> *G, void *dummy = nullptr);
    template TriangleListing::Collect<InstrumentedIndex> edge_iterator(AdjacencyGraph<InstrumentedIndex> *G, void *dummy = nullptr);
}