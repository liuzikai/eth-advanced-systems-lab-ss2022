#define INSTRUMENTED
#include "merge_sort_base.h"
#include "merge_sort_v2.h"
#include "merge_sort_v3.h"
#include "merge_sort_v4.h"
#include "instrumented_index.h"

namespace ms0 {
template void merge_sort(InstrumentedIndex *arr, InstrumentedIndex* other, index_t total_size); 
template void merge_sort_timing(TriangleListing::SetCollect<InstrumentedIndex>* ,AdjacencyGraph<InstrumentedIndex> *G, void *dummy); 
template void merge_sort_timing(TriangleListing::Count<InstrumentedIndex>* ,AdjacencyGraph<InstrumentedIndex> *G, void *dummy); 

}



namespace ms2 {
template void merge_sort(InstrumentedIndex *arr, InstrumentedIndex* other, index_t total_size); 
template void merge_sort_timing(TriangleListing::SetCollect<InstrumentedIndex>* ,AdjacencyGraph<InstrumentedIndex> *G, void *dummy); 
template void merge_sort_timing(TriangleListing::Count<InstrumentedIndex>* ,AdjacencyGraph<InstrumentedIndex> *G, void *dummy); 

}

namespace ms3 {
template void merge_sort(InstrumentedIndex *arr, InstrumentedIndex* other, index_t total_size); 
template void merge_sort_timing(TriangleListing::SetCollect<InstrumentedIndex>* ,AdjacencyGraph<InstrumentedIndex> *G, void *dummy); 
template void merge_sort_timing(TriangleListing::Count<InstrumentedIndex>* ,AdjacencyGraph<InstrumentedIndex> *G, void *dummy); 
}

namespace ms4 {
template void merge_sort(InstrumentedIndex *arr, InstrumentedIndex* other, index_t total_size); 
template void merge_sort_timing(TriangleListing::SetCollect<InstrumentedIndex>* ,AdjacencyGraph<InstrumentedIndex> *G, void *dummy); 
template void merge_sort_timing(TriangleListing::Count<InstrumentedIndex>* ,AdjacencyGraph<InstrumentedIndex> *G, void *dummy); 
}
