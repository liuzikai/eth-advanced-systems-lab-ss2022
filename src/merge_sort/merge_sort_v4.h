#ifndef _MSORT_V4_H
#define _MSORT_V4_H

#include "common.h"
#include "triangle_lister.h"
#include "adjacency_graph.h"
#include <stdio.h>

#include "instrumented_immintrin.h"


namespace ms4 {



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

inline __m256i sort_singel_vec(__m256i vec)
{
    {
        __m256i perm_idx = _mm256_set_epi32(6, 7, 4, 5, 2, 3, 0, 1);
        __m256i permed =  _mm256_permutevar8x32_epi32(vec, perm_idx);
        __m256i min_vec = _mm256_min_epu32(permed, vec);
        __m256i max_vec = _mm256_max_epu32(permed, vec);
        vec = _mm256_blend_epi32(min_vec, max_vec, 0xAA);
    }
    {
        __m256i perm_idx = _mm256_set_epi32(4, 5, 6, 7, 0, 1, 2, 3);
        __m256i permed =  _mm256_permutevar8x32_epi32(vec, perm_idx);
        __m256i min_vec = _mm256_min_epu32(permed, vec);
        __m256i max_vec = _mm256_max_epu32(permed, vec);
        vec = _mm256_blend_epi32(min_vec, max_vec, 0xCC);
    }
    {
        __m256i perm_idx = _mm256_set_epi32(6, 7, 4, 5, 2, 3, 0, 1);
        __m256i permed =  _mm256_permutevar8x32_epi32(vec, perm_idx);
        __m256i min_vec = _mm256_min_epu32(permed, vec);
        __m256i max_vec = _mm256_max_epu32(permed, vec);
        vec = _mm256_blend_epi32(min_vec, max_vec, 0xAA);
    }
    {
        __m256i perm_idx = _mm256_set_epi32(0, 1, 2, 3, 4, 5, 6, 7);
        __m256i permed =  _mm256_permutevar8x32_epi32(vec, perm_idx);
        __m256i min_vec = _mm256_min_epu32(permed, vec);
        __m256i max_vec = _mm256_max_epu32(permed, vec);
        vec = _mm256_blend_epi32(min_vec, max_vec, 0xF0);
    }
    {
        __m256i perm_idx = _mm256_set_epi32(5, 4, 7, 6, 1, 0, 3, 2);
        __m256i permed =  _mm256_permutevar8x32_epi32(vec, perm_idx);
        __m256i min_vec = _mm256_min_epu32(permed, vec);
        __m256i max_vec = _mm256_max_epu32(permed, vec);
        vec = _mm256_blend_epi32(min_vec, max_vec, 0xCC);
    }
    {
        __m256i perm_idx = _mm256_set_epi32(6, 7, 4, 5, 2, 3, 0, 1);
        __m256i permed =  _mm256_permutevar8x32_epi32(vec, perm_idx);
        __m256i min_vec = _mm256_min_epu32(permed, vec);
        __m256i max_vec = _mm256_max_epu32(permed, vec);
        vec = _mm256_blend_epi32(min_vec, max_vec, 0xAA);
    }
  return vec;
}

template<class Index>
inline __m256i load_reg256(Index *a) {
  return *(__m256i*)a;
}

template<class Index>
inline void store_reg256(Index *a, __m256i& b) {
  *((__m256i*)a) = b;
}

template<class Index>
static inline bool merge_sort_internal(Index *arr, Index* other, index_t bucket_size, index_t total_size) {
    if (bucket_size == 8) {
        index_t i;
        // We do not have overhang anymore in this version.
        for (i = 0; i < total_size; i+= 8) {
            __m256i vec = load_reg256(arr + i);
            __m256i sorted_vec  = sort_singel_vec(vec);
            store_reg256(arr + i, sorted_vec);
        }
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
    total_size = roundUp(total_size, 8);
    index_t bucket_size = upper_power_of_two(total_size);
    bool in_arr = merge_sort_internal(arr, other, bucket_size, total_size);
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


#endif
