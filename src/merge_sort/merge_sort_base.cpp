#include "merge_sort_base.h"

namespace ms0 {
template void merge_sort(index_t *arr, index_t* other, index_t total_size); 
template void merge_sort_timing(TriangleListing::Count<index_t>* ,AdjacencyGraph<index_t> *G, void *dummy); 
template void merge_sort_timing(TriangleListing::Collect<index_t>* ,AdjacencyGraph<index_t> *G, void *dummy); 
}
