#include "merge_sort_v3.h"
#include "merge_sort_all_versions.h"

namespace ms3 {
template void merge_sort(index_t *arr, index_t* other, index_t total_size); 
template TriangleListing::Count<index_t> merge_sort_timing(AdjacencyGraph<index_t> *G, void *dummy); 
}
