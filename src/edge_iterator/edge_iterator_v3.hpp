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

namespace ei3 {

// https://stackoverflow.com/questions/109023/how-to-count-the-number-of-set-bits-in-a-32-bit-integer
inline int numberOfSetBits(uint32_t i)
{
     // Java: use int, and use >>> instead of >>. Or use Integer.bitCount()
     // C or C++: use uint32_t
     i = i - ((i >> 1) & 0x55555555);        // add pairs of bits
     i = (i & 0x33333333) + ((i >> 2) & 0x33333333);  // quads
     i = (i + (i >> 4)) & 0x0F0F0F0F;        // groups of 8
     return (i * 0x01010101) >> 24;          // horizontal sum of bytes
}

template<class Index, class Counter = index_t, class TRL = TriangleListing::Count<Index>>
void edge_iterator(TRL* lister,AdjacencyGraph<Index> *G, void *dummy = nullptr) {
    (void) dummy;
    Index s, t;
    //Counter i, j_lower_bound, j_upper_bound, j_current, j_inc, j_next;
    Counter i, j, j_lower_bound, j_inc, j_next, i_start, j_start;
    Counter highest_i, highest_j;
    Counter mid_i, mid_j;
    int merge_mask = 0x55555555; //all even bits
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
                highest_i = s_adj->count - 1;
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
                }
                /*while(i_start < s_adj->count && s_adj->neighbors[i_start] < t) {
                    i_start++;    
                }
                while(j_start < t_adj->count && t_adj->neighbors[j_start] < t) {
                    j_start++;    
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
                    i = i_start, j = j_start;
                    while(i + 3 < s_adj->count && j + 3 < t_adj->count) {
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
                        //__m256i duplicated = _mm_broadcastsi128_si256(t_neighbors);

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
                        int bool_comps = _mm256_movemask_epi8(comp);
                        if(bool_comps != 0) {
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
                            int bool_comps2 = _mm256_movemask_epi8(comp2);

                            uint32_t matches = bool_comps & bool_comps2;
                            uint32_t matches_merged = (matches & (matches >> 1)) & merge_mask;
                            if(matches_merged != 0) {
                                if constexpr (std::is_same_v<TRL, TriangleListing::Count<Index>>) { 
                                    lister->count += numberOfSetBits(matches_merged);
                                    if(s_adj->neighbors[i+3] > t_adj->neighbors[j+3]) {
                                        j += 4;
                                    } else {
                                        i += 4;
                                    }
                                    continue;
                                } else {
                                    if(matches_merged & 1) {
                                        // element 1 in s matches element 1 in t
                                        lister->list_triangle(s, t, s_adj->neighbors[i]);
                                        // check if element 2 in s matches element 2 in t
                                        if(matches_merged & 0x40) {
                                            // element 2 in s matches element 2 in t
                                            lister->list_triangle(s, t, s_adj->neighbors[i + 1]);
                                            // check if element 3 in s matches element 3 in t
                                            if(matches_merged & 0x1000000) {
                                                lister->list_triangle(s, t, s_adj->neighbors[i + 2]);
                                                // check if element 4 in s matches element 4 in t
                                                if(matches_merged & 0x40000000) {
                                                    lister->list_triangle(s, t, s_adj->neighbors[i + 3]);
                                                    i += 4;
                                                    j += 4;
                                                    continue;
                                                } else if (s_adj->neighbors[i+3] > t_adj->neighbors[j+3]) {
                                                    i += 3;
                                                    j += 4;
                                                    continue;
                                                } else {
                                                    i += 4;
                                                    j += 3;
                                                    continue;
                                                }
                                            }
                                            i += 2;
                                            j += 2;
                                            continue;
                                        }
                                        i += 1;
                                        j += 1;
                                        continue;
                                    } else if(matches_merged & 0x4) {
                                        // element 1 in s matches element 2 in t
                                        lister->list_triangle(s, t, s_adj->neighbors[i]);
                                        i += 1;
                                        j += 2;
                                        continue;
                                    } else if(matches_merged & 0x10000) {
                                        // element 1 in s matches element 3 in t
                                        lister->list_triangle(s, t, s_adj->neighbors[i]);
                                        i += 1;
                                        j += 3;
                                        continue;
                                    } else if(matches_merged & 0x40000) {
                                        // element 1 in s matches element 4 in t
                                        lister->list_triangle(s, t, s_adj->neighbors[i]);
                                        i += 1;
                                        j += 4;
                                        continue;
                                    } else {
                                        i += 1;
                                        continue;
                                    }
                                }
                            } else {
                                if(s_adj->neighbors[i+3] > t_adj->neighbors[j+3]) {
                                    j += 4;
                                } else {
                                    i += 4;
                                }
                                continue;
                            }
                        } else {
                            if(s_adj->neighbors[i+3] > t_adj->neighbors[j+3]) {
                                j += 4;
                            } else {
                                i += 4;
                            }
                            continue;
                        }
                    }
                    /*while (i < s_adj->count -1 && j < t_adj->count -1) {
                        s_neighbor1 = s_adj->neighbors[i];
                        s_neighbor2 = s_adj->neighbors[i+1];
                        t_neighbor1 = t_adj->neighbors[j];
                        t_neighbor2 = t_adj->neighbors[j+1];
                        if (s_neighbor1 == t_neighbor1) {
                            lister.list_triangle(s, t, s_neighbor1);
                        } else if (s_neighbor1 == t_neighbor2) {
                            lister.list_triangle(s, t, s_neighbor1);
                            goto advance_t;
                        } else if (s_neighbor2 == t_neighbor1) {
                            lister.list_triangle(s, t, s_neighbor2);
                            goto advance_s;
                        } 
                        if (s_neighbor2 == t_neighbor2) {
                            lister.list_triangle(s, t, s_neighbor2);
                            goto advance_st;
                        } else if (s_neighbor2 > t_neighbor2) {
                            goto advance_t;
                        } else {
                            goto advance_s;
                        }
                    advance_s:
                        i+=2;
                        continue;
                    advance_t:
                        j+=2;
                        continue;
                    advance_st:
                        i+=2;
                        j+=2;
                    }*/
                    while (i < s_adj->count && j < t_adj->count) {
                        if (s_adj->neighbors[i] == t_adj->neighbors[j] &&
                            t_adj->neighbors[j] > t) {
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
}

}

#endif //TEAM02_EDGE_ITER_V3_H