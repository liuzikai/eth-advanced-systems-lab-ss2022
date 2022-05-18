#include "merge_sort_base.h"
#include "instrumented_index.h"

namespace ms0 {
template void merge_sort(InstrumentedIndex *arr, InstrumentedIndex* other, index_t total_size); 
template TriangleListing::Collect<InstrumentedIndex> merge_sort_timing(AdjacencyGraph<InstrumentedIndex> *G, void *dummy); 
}
