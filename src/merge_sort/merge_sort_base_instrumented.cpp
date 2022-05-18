#include "merge_sort_base.h"
#include "instrumented_index.h"

namespace merge_sort_base {
template TriangleListing::Collect<InstrumentedIndex> merge_sort_timing(AdjacencyGraph<InstrumentedIndex> *G, void *dummy); 
}
