//
// Reference：[QuickSort - GeeksforGeeks](https://www.geeksforgeeks.org/quick-sort/)
//

#ifndef _MSORT_H
#define _MSORT_H

#include "common.h"
#include "triangle_lister.h"
#include "adjacency_graph.h"
#include <stdio.h>
#include <immintrin.h>

namespace merge_sort_v1 {

template<class Index>
static inline void merge(Index* out, Index *in, index_t left, index_t mid, index_t right) {
    index_t out_index = left;
    index_t l_index = left;
    index_t r_index = mid;

    while (l_index < mid && r_index < right) {
        if (in[l_index] < in[r_index]) {
            out[out_index] = in[l_index];
            ++l_index;
        } else {
            out[out_index] = in[r_index];
            ++r_index;
        }
        out_index++;
    }

    while (l_index < mid) {
        out[out_index] = in[l_index];
        ++l_index;
        out_index++;
    }

    while (r_index < right) {
        out[out_index] = in[r_index];
        ++r_index;
        out_index++;
    }
}

template<class Index>
static inline bool merge_sort_internal(Index *arr, Index* other, index_t bucket_size, index_t total_size) {
    if (bucket_size == 1) {
        // Size 1 is already sorted.
        // True means data is now in arr.
        return true;
    }

    index_t  child_bucket_size = bucket_size / 2;
    bool in_arr = merge_sort_internal(arr, other, child_bucket_size, total_size);
    if (in_arr) {
      Index* tmp = arr;
      arr = other;
      other = tmp;
    }
    index_t i;
    for (i = 0; i + child_bucket_size * 2 < total_size  + 1; i+= child_bucket_size * 2) {
        merge(arr, other,  i, i + child_bucket_size, i + 2 * child_bucket_size);
    }
    index_t diff = total_size - i;
    if (diff > child_bucket_size) {
      merge(arr, other, i, i + child_bucket_size, total_size);
    } else if (diff > 0) {
      merge(arr, other, i, total_size, total_size);
    }
    return !in_arr;
}

static inline unsigned long upper_power_of_two(index_t v) {
  v--;
  v |= v >> 1;
  v |= v >> 2;
  v |= v >> 4;
  v |= v >> 8;
  v |= v >> 16;
  v++;
  return v;
}


template<class Index>
static inline void merge_sort(Index *arr, Index* other, index_t total_size) {
  bool in_arr = merge_sort_internal(arr, other, upper_power_of_two(total_size), total_size);
  if (!in_arr) {
    // We just merged ito into other so lets copy it back.
    for (index_t j = 0; j < total_size; j++ ) {
      arr[j] = other[j];
    }
  }
}

// This is for testing
template<class Index, class Counter = index_t, class TRL = TriangleListing::Count<Index>>
TRL merge_sort_timing(AdjacencyGraph<Index> *G, void *dummy) {
    static Index sort_buf[10800];
    (void) dummy;
    // According to sec. 4, the sorting is included in the execution time
    for (Counter u = 0; u < G->n; u++) {
        if (G->adjacency[u].count > 0) {
            merge_sort(G->adjacency[u].neighbors, sort_buf, G->adjacency[u].count);
        }
    }
    TRL res;
    return res;
}


}



#endif //_QSORT_H
