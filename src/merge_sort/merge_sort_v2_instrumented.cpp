#include "merge_sort_v2.h"
#include "instrumented_index.h"

namespace merge_sort_v2 {
template TriangleListing::Collect<InstrumentedIndex> merge_sort_timing(AdjacencyGraph<InstrumentedIndex> *G, void *dummy); 
}
