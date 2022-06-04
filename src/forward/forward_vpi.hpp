#ifndef TEAM02_FORWARD_VPI_H
#define TEAM02_FORWARD_VPI_H

#include "common.h"
#include "adjacency_graph.h"
#include "instrumented_index.h"
#include "triangle_lister.h"
#include "quick_sort.h"
#include "instrumented_immintrin.h"

namespace fpi {

/// Accessory container (array A in the paper)
template<class Index, class Counter = index_t>
struct ForwardNeighbourList {
    Counter count;
    Index *neighbors;
};

template<class Index, class Counter = index_t>
struct ForwardNeighborContainer {
    Counter n;  // node count
    ForwardNeighbourList<Index> *adjacency;
};


template<class Index, class Counter = index_t>
ForwardNeighborContainer<Index> *forward_create_neighbor_container(const AdjacencyGraph<Index> *G) {
    auto *A = new ForwardNeighborContainer<Index>;
    A->adjacency = new ForwardNeighbourList<Index>[G->n];
    for (Counter u = 0; u < G->n; u++) {
        A->adjacency[u].neighbors = new Index[G->adjacency[u].orig_count - G->adjacency[u].count];
    }
    return A;
}

template<class Index, class Counter = index_t>
static void forward_reset_neighbor_container(AdjacencyGraph<Index> *G, ForwardNeighborContainer<Index> *A) {
    A->n = G->n;
    for (Counter u = 0; u < G->n; u++) {
        A->adjacency[u].count = 0;
    }
}

template<class Index, class Counter = index_t>
void forward_delete_neighbor_container(ForwardNeighborContainer<Index> *A) {
    for (Counter u = 0; u < A->n; u++) {
        delete[] A->adjacency[u].neighbors;
    }
    delete[] A->adjacency;
    delete A;
}

/**
 * List triangles using the forward algorithm.
 * @param G  The input graph. Notice that the graph WILL be changed (sorted). Make a copy before calling the function.
 * @param A  The accessory container.
 * @return   The number of triangles.
 * @note     Triangles are printed using print_triangle if it is defined.
 */
template<class Index, class Counter = index_t, class TRL = TriangleListing::Count<Index>>
void forward(TRL* lister,AdjacencyGraph<Index> *G, ForwardNeighborContainer<Index> *A) {
    forward_reset_neighbor_container(G, A);

    // Index s, t;
    //Counter i, j_lower_bound, j_upper_bound, j_current, j_inc, j_next;
    Counter i, j, j_lower_bound, j_inc, j_next, i_start, j_start;
    //Counter highest_i, highest_j;
    //Counter mid_i, mid_j;
    Index count_triangles = (Index) 0;
    (void) count_triangles;
    const Index merge_mask = (Index) 0x55555555; //all even bits
    ForwardNeighbourList<Index> *t_adj, *s_adj, *swap = NULL;
    //0x0002 0000 0002 0000 0002 0000 0002 0000
    __m256i perm_idx_low = _mm256_set_epi32(2, 0, 2, 0, 2, 0, 2, 0);
    __m256i perm_idx_high = _mm256_set_epi32(3, 1, 3, 1, 3, 1, 3, 1);

    // According to sec. 4, the sorting is included in the execution time
    // for (Counter u = 0; u < G->n; u++) {
    //     if (G->adjacency[u].count > 0) {
    //         quick_sort(G->adjacency[u].neighbors, 0, G->adjacency[u].count - 1);
    //     }
    // }

    Counter t_adj_count, s_adj_count, swap_count;
    Index* s_adj_neighbors, *t_adj_neighbors, *swap_neighbors;

    Counter Gn = (Counter) G->n;
    auto G_adj = G->adjacency;
    for (Counter si = 0; si < Gn; si++) {  // this should not count toward op count
        Index s = (Index) si;

        for (Counter ti = 0; ti < G_adj[(index_t) s].count; ti++) {
            Index t =G_adj[(index_t) s].neighbors[ti];

            s_adj = &A->adjacency[(index_t) s];
            s_adj_neighbors = s_adj->neighbors;
            s_adj_count = s_adj->count;
            t_adj = &A->adjacency[(index_t) t];
            t_adj_neighbors = t_adj->neighbors;
            t_adj_count = t_adj->count;



            if (s < t && t_adj_count > 0) {
                if(t_adj_count < s_adj_count) {
                    swap = t_adj;
                    t_adj = s_adj;
                    s_adj = swap;
                    swap_count = t_adj_count;
                    t_adj_count = s_adj_count;
                    s_adj_count = swap_count;
                    swap_neighbors = t_adj_neighbors;
                    t_adj_neighbors = s_adj_neighbors;
                    s_adj_neighbors = swap_neighbors;
                }
                i_start = 0, j_start = 0;
                if(s_adj_count << 3 < t_adj_count) {
                    i = i_start;
                    j_lower_bound = j_start;
                    // find the intersection of s's and t's neighbors
                    for(;i < s_adj_count; i++) {
                        //Perform an exponential search on t_adj->neighbors
                        j_inc = 1;
                        j_next = j_lower_bound + 2*j_inc;

                        while(j_next < t_adj_count && s_adj_neighbors[i] > t_adj_neighbors[j_next]) {
                            j_inc *= 2;
                            j_next = j_lower_bound + 2*j_inc;
                        }
                        j_lower_bound += (j_inc -1);
                        while(j_lower_bound < t_adj_count && s_adj_neighbors[i] > t_adj_neighbors[j_lower_bound]) {
                            j_lower_bound += 1;
                        }
                        if(s_adj_neighbors[i] == t_adj_neighbors[j_lower_bound]) {
                            lister->list_triangle(s, t, t_adj_neighbors[j_lower_bound]);
                        }
                    }
                } else {
                    #ifdef MEASURE_DEEP_LOOP_OPS
                    OpCounter::ResetOpCount();
                    #elif defined(MEASURE_DEEP_LOOP_CYCLES)
                    size_t cycles = start_tsc();
                    #endif
                    i = i_start, j = j_start;
                    
                    while(i + 3 < s_adj_count && j + 3 < t_adj_count) {
                        #if defined(MEASURE_DEEP_LOOP_OPS) || defined(MEASURE_DEEP_LOOP_CYCLES)
                        iters++;
                        #endif
                        // a0a1a2a3b0b1b2b3c0c1c2c3d0d1d2d3
                        __m128i s_neighbors = _mm_loadu_si128((__m128i *)(s_adj_neighbors + i));

                        // a0a1a0a1a2a3a2a3b0b1b0b1b2b3b2b3
                        __m128i low = _mm_unpacklo_epi16(s_neighbors, s_neighbors);
                        // c0c1c0c1c2c3c2c3d0d1d0d1d2d3d2d3
                        __m128i high = _mm_unpackhi_epi16(s_neighbors, s_neighbors);

                        // a0a1a0a1a2a3a2a3b0b1b0b1b2b3b2b3a0a1a0a1a2a3a2a3b0b1b0b1b2b3b2b3
                        __m256i merge_low_low = _mm256_inserti128_si256(_mm256_castsi128_si256(low), low, 1); 
                        // c0c1c0c1c2c3c2c3d0d1d0d1d2d3d2d3c0c1c0c1c2c3c2c3d0d1d0d1d2d3d2d3
                        __m256i merge_high_high = _mm256_inserti128_si256(_mm256_castsi128_si256(high), high, 1);
                        // a0a1a0a1b0b1b0b1a0a1a0a1b0b1b0b1a0a1a0a1b0b1b0b1a0a1a0a1b0b1b0b1
                        __m256i permuted_low = _mm256_permutevar8x32_epi32(merge_low_low, perm_idx_low);
                        // c0c1c0c1d0d1d0d1c0c1c0c1d0d1d0d1c0c1c0c1d0d1d0d1c0c1c0c1d0d1d0d1
                        __m256i permute_high = _mm256_permutevar8x32_epi32(merge_high_high, perm_idx_low);
                        // a0a1a0a1b0b1b0b1a0a1a0a1b0b1b0b1c0c1c0c1d0d1d0d1c0c1c0c1d0d1d0d1
                        __m256i merge_low_high = _mm256_permute2f128_si256(permuted_low, permute_high, 0x20);

                        // a0a1a0a1b0b1b0b1c0c1c0c1d0d1d0d1a0a1a0a1b0b1b0b1c0c1c0c1d0d1d0d1
                        __m256i a = _mm256_permute4x64_epi64(merge_low_high, 0xD8);

                        // a0a1a2a3b0b1b2b3c0c1c2c3d0d1d2d3
                        __m128i t_neighbors = _mm_loadu_si128((__m128i *)(t_adj_neighbors + j));
                        // a0a1a2a3b0b1b2b3c0c1c2c3d0d1d2d3
                        // a0a1a2a3b0b1b2b3c0c1c2c3d0d1d2d3

                        // a0a1a2a3b0b1b2b3c0c1c2c3d0d1d2d3
                        // c0c1c2c3d0d1d2d3a0a1a2a3b0b1b2b3
                        __m256i rearranged = _mm256_permute4x64_epi64(_mm256_castsi128_si256(t_neighbors), 0x14);
                        // a0a1b0b1a0a1b0b1_c0c1d0d1c0c1d0d1_
                        __m256i merged = _mm256_shufflelo_epi16(rearranged, 0x88);

                        // a0a1b0b1a0a1b0b1a0a1b0b1a0a1b0b1c0c1d0d1c0c1d0d1c0c1d0d1c0c1d0d1
                        __m256i b  = _mm256_unpacklo_epi32(merged, merged);

                        // a0a1a0a1b0b1b0b1c0c1c0c1d0d1d0d1a0a1a0a1b0b1b0b1c0c1c0c1d0d1d0d1
                        // a0a1b0b1a0a1b0b1a0a1b0b1a0a1b0b1c0c1d0d1c0c1d0d1c0c1d0d1c0c1d0d1
                        // if a[i+15:i] == b[i+15:i] => element 1 in s might match element 1 in t (if false, doesnt match, if true it might need to check upper half)
                        // if a[i+31:i+16] == b[i+31:i+16] => element 1 in s might match element 2 in t (if false, doesnt match, if true it might need to check upper half) 
                        // if a[i+15+128:i+128] == b[i+15+128:i+128] => element 1 in s might match element 3 in t (if false, doesnt match, if true it might need to check upper half)
                        // if a[i+31+128:i+16+128] == b[i+31+128:i+16+128] => element 1 in s might match element 4 in t (if false, doesnt match, if true it might need to check upper half) 
                        __m256i comp = _mm256_cmpeq_epi16(a, b);
                        Index bool_comps = (Index) _mm256_movemask_epi8(comp);
                        if(bool_comps != (Index)0) {
                            // a2a3a2a3b2b3b2b3a2a3a2a3b2b3b2b3a2a3a2a3b2b3b2b3a2a3a2a3b2b3b2b3
                            __m256i permuted_low2 = _mm256_permutevar8x32_epi32(merge_low_low, perm_idx_high);
                            // c2c3c2c3d2d3d2d3c2c3c2c3d2d3d2d3c2c3c2c3d2d3d2d3c2c3c2c3d2d3d2d3
                            __m256i permute_high2 = _mm256_permutevar8x32_epi32(merge_high_high, perm_idx_high);

                            // a2a3a2a3b2b3b2b3a2a3a2a3b2b3b2b3c2c3c2c3d2d3d2d3c2c3c2c3d2d3d2d3
                            __m256i merge_low_high2 = _mm256_permute2f128_si256(permuted_low2, permute_high2, 0x20);

                            // compare upper halves and identify which ones are matching
                            // a2a3a2a3b2b3b2b3c2c3c2c3d2d3d2d3a2a3a2a3b2b3b2b3c2c3c2c3d2d3d2d3
                            __m256i a2 = _mm256_permute4x64_epi64(merge_low_high2, 0xD8);
                            
                            // a2a3b2b3a2a3b2b3_c2c3d2d3c2c3d2d3_
                            __m256i merged2 = _mm256_shufflelo_epi16(rearranged, 0xDD);

                            // a2a3b2b3a2a3b2b3a2a3b2b3a2a3b2b3c2c3d2d3c2c3d2d3c2c3d2d3c2c3d2d3
                            __m256i b2 = _mm256_unpacklo_epi32(merged2, merged2);

                            __m256i comp2 = _mm256_cmpeq_epi16(a2, b2);
                            Index bool_comps2 = (Index)_mm256_movemask_epi8(comp2);

                            Index matches = bool_comps & bool_comps2;
                            Index matches_merged = (matches & (matches >> (Index)1)) & merge_mask;
                            if(matches_merged != (Index)0) {
                                if constexpr (std::is_same_v<TRL, TriangleListing::Count<Index>>) { 
                                    count_triangles = count_triangles + __builtin_popcount((index_t)matches_merged);
                                } else {
                                    if((matches_merged & (Index)0x50005) != (Index)0) {
                                        // element 1 in s matches any element in t
                                        lister->list_triangle(s, t, s_adj_neighbors[i]);
                                    }
                                    if((matches_merged & (Index)0x500050) != (Index)0) {
                                        // element 2 in s matches any element in t
                                        lister->list_triangle(s, t, s_adj_neighbors[i+1]);
                                    }
                                    if((matches_merged & (Index)0x5000500) != (Index)0) {
                                        // element 3 in s matches any element in t
                                        lister->list_triangle(s, t, s_adj_neighbors[i+2]);
                                    }
                                    if((matches_merged & (Index)0x50005000) != (Index)0) {
                                        // element 4 in s matches any element in t
                                        lister->list_triangle(s, t, s_adj_neighbors[i+3]);
                                    }
                                }
                            }
                        }
                        if(s_adj_neighbors[i+3] > t_adj_neighbors[j+3]) {
                            j += 4;
                        } else {
                            i += 4;
                        }
                    }
                    #ifdef MEASURE_DEEP_LOOP_OPS
                    op_sum += (OpCounter::GetOpCount());
                    #elif defined(MEASURE_DEEP_LOOP_CYCLES)
                    op_sum += stop_tsc(cycles);;
                    #endif
                    while (i < s_adj_count && j < t_adj_count) {
                        if (s_adj_neighbors[i] == t_adj_neighbors[j]) {/*&&
                            t_adj_neighbors[j] > t) {*/
                            lister->list_triangle(s, t, t_adj_neighbors[j]);
                            i++;
                            j++;
                        } else if (s_adj_neighbors[i] < t_adj_neighbors[j]) {
                            i++;
                        } else {
                            j++;
                        }
                    }
                }
                if constexpr (std::is_same_v<TRL, TriangleListing::Count<Index>>) { 
                    lister->count += (index_t)count_triangles;
                }
                //swap back if swapped
                if(swap != NULL) {
                    swap = t_adj;
                    t_adj = s_adj;
                    s_adj = swap;
                    swap = NULL;

                    swap_count = t_adj_count;
                    t_adj_count = s_adj_count;
                    s_adj_count = swap_count;

                    swap_neighbors = t_adj_neighbors;
                    t_adj_neighbors = s_adj_neighbors;
                    s_adj_neighbors = swap_neighbors;
                }
            }

            t_adj_neighbors[t_adj->count++] = s;
        }
    }

    
}

}

#endif //TEAM02_FORWARD_VA_H
