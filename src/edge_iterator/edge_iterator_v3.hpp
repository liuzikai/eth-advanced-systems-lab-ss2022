#ifndef TEAM02_EDGE_ITER_V3_H
#define TEAM02_EDGE_ITER_V3_H

#include <bitset>
#include <string.h>
#include "common.h"
#include "adjacency_graph.h"
#include "triangle_lister.h"
#include "quick_sort.h"
#include "instrumented_index.h"
#include "instrumented_immintrin.h"

//#define MEASURE_DEEP_LOOP_OPS
#define MEASURE_DEEP_LOOP_CYCLES

#ifdef MEASURE_DEEP_LOOP_CYCLES
#include "tsc_x86.h"
#endif


namespace ei3 {

    template<class Index, class Counter = index_t, class TRL = TriangleListing::Count<Index>>
    void edge_iterator(TRL* lister,AdjacencyGraph<Index> *G, void *dummy = nullptr) {
        #if defined(MEASURE_DEEP_LOOP_OPS) || defined(MEASURE_DEEP_LOOP_CYCLES)
        size_t op_sum = 0;
        size_t iters  = 0;
        #endif
        (void) dummy;
        Index s, t;
        //Counter i, j_lower_bound, j_upper_bound, j_current, j_inc, j_next;
        Counter i, j, j_lower_bound, j_inc, j_next, i_start, j_start;
        //Counter highest_i, highest_j;
        //Counter mid_i, mid_j;
        Index count_triangles = (Index) 0;
        (void) count_triangles;
        const Index merge_mask = (Index) 0x55555555; //all even bits
        const AdjacencyList<Index> *t_adj, *s_adj, *swap = NULL;
        //0x0002 0000 0002 0000 0002 0000 0002 0000
        __m256i perm_idx_low = _mm256_set_epi32(2, 0, 2, 0, 2, 0, 2, 0);
        __m256i perm_idx_high = _mm256_set_epi32(3, 1, 3, 1, 3, 1, 3, 1);

        /*Index s_neighbor1, s_neighbor2;
        Index t_neighbor1, t_neighbor2;*/
        
        // According to sec. 4, the sorting is included in the execution time
        // for (Counter u = 0; u < G->n; u++) {
        //     if (G->adjacency[u].count > 0) {
        //         quick_sort(G->adjacency[u].neighbors, 0, G->adjacency[u].count - 1);
        //     }
        // }

        for (Counter si = 0; si < G->n; si++) {  // this should not count toward op count
            s = (Index) si;

            s_adj = &G->adjacency[(index_t) s];

            // iterate over all edges (neighbors) of node s
            for (Counter ti = 0; ti < s_adj->count; ti++) {
                t = s_adj->neighbors[ti];
                t_adj = &G->adjacency[(index_t) t];

                if (s < t && t_adj->count > 0) {
                    if(t_adj->count < s_adj->count) {
                        swap = t_adj;
                        t_adj = s_adj;
                        s_adj = swap;
                    }
                    i_start = 0, j_start = 0;
                    /*highest_i = s_adj->count - 1;
                    highest_j = t_adj->count - 1;
                    while(i_start != highest_i) {
                        mid_i = (i_start + highest_i) / 2;
                        if (s_adj->neighbors[mid_i] <= t) {
                            i_start = mid_i + 1;
                        } else {
                            highest_i = mid_i;
                        }
                    }
                    while(j_start != highest_j) {
                        mid_j = (j_start + highest_j) / 2;
                        if (t_adj->neighbors[mid_j] <= t) {
                            j_start = mid_j + 1;
                        } else {
                            highest_j = mid_j;
                        }
                    }*/
                    if(s_adj->count << 3 < t_adj->count) {
                        i = i_start;
                        j_lower_bound = j_start;
                        // find the intersection of s's and t's neighbors
                        for(;i < s_adj->count; i++) {
                            //Perform an exponential search on t_adj->neighbors
                            j_inc = 1;
                            j_next = j_lower_bound + 2*j_inc;

                            while(j_next < t_adj->count && s_adj->neighbors[i] > t_adj->neighbors[j_next]) {
                                j_inc *= 2;
                                j_next = j_lower_bound + 2*j_inc;
                            }
                            j_lower_bound += (j_inc -1);
                            while(j_lower_bound < t_adj->count && s_adj->neighbors[i] > t_adj->neighbors[j_lower_bound]) {
                                j_lower_bound += 1;
                            }
                            if(s_adj->neighbors[i] == t_adj->neighbors[j_lower_bound]) {
                                lister->list_triangle(s, t, t_adj->neighbors[j_lower_bound]);
                            }
                        }
                    } else {
                        #ifdef MEASURE_DEEP_LOOP_OPS
                        OpCounter::ResetOpCount();
                        #elif defined(MEASURE_DEEP_LOOP_CYCLES)
                        size_t cycles = start_tsc();
                        #endif
                        i = i_start, j = j_start;
                        while(i + 3 < s_adj->count && j + 3 < t_adj->count) {
                            #if defined(MEASURE_DEEP_LOOP_OPS) || defined(MEASURE_DEEP_LOOP_CYCLES)
                            iters++;
                            #endif
                            // a0a1a2a3b0b1b2b3c0c1c2c3d0d1d2d3
                            __m128i s_neighbors = _mm_loadu_si128((__m128i *)(s_adj->neighbors + i));

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
                            __m128i t_neighbors = _mm_loadu_si128((__m128i *)(t_adj->neighbors + j));
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
                                            lister->list_triangle(s, t, s_adj->neighbors[i]);
                                        }
                                        if((matches_merged & (Index)0x500050) != (Index)0) {
                                            // element 2 in s matches any element in t
                                            lister->list_triangle(s, t, s_adj->neighbors[i+1]);
                                        }
                                        if((matches_merged & (Index)0x5000500) != (Index)0) {
                                            // element 3 in s matches any element in t
                                            lister->list_triangle(s, t, s_adj->neighbors[i+2]);
                                        }
                                        if((matches_merged & (Index)0x50005000) != (Index)0) {
                                            // element 4 in s matches any element in t
                                            lister->list_triangle(s, t, s_adj->neighbors[i+3]);
                                        }
                                    }
                                }
                            }
                            if(s_adj->neighbors[i+3] > t_adj->neighbors[j+3]) {
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
                        while (i < s_adj->count && j < t_adj->count) {
                            if (s_adj->neighbors[i] == t_adj->neighbors[j]) {/*&&
                                t_adj->neighbors[j] > t) {*/
                                lister->list_triangle(s, t, t_adj->neighbors[j]);
                                i++;
                                j++;
                            } else if (s_adj->neighbors[i] < t_adj->neighbors[j]) {
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
                    }
                }
            }
        }
        #ifdef MEASURE_DEEP_LOOP_OPS
        std::cout << "Deep Loop Ops: " << (op_sum/iters) << std::endl;
        #elif defined(MEASURE_DEEP_LOOP_CYCLES)
        std::cout << "Deep Loop Cycles: " << (op_sum/iters) << std::endl;
        #endif
    }
}

#endif //TEAM02_EDGE_ITER_V3_H