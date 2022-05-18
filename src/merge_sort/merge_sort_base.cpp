#include "merge_sort_base.h"

namespace merge_sort_base {
template void merge_sort(index_t *arr, index_t* other, index_t total_size); 
template TriangleListing::Count<index_t> merge_sort_timing(AdjacencyGraph<index_t> *G, void *dummy); 
}
