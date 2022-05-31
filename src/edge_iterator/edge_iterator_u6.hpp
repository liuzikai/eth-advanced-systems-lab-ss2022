#ifndef TEAM02_EDGE_ITER_U6_H
#define TEAM02_EDGE_ITER_U6_H

#include "common.h"
#include "adjacency_graph.h"
#include "triangle_lister.h"
#include "quick_sort.h"

#include "instrumented_immintrin.h"

namespace eu6 {

// https://stackoverflow.com/questions/60108658/fastest-method-to-calculate-sum-of-all-packed-32-bit-integers-using-avx512-or-av
static inline uint32_t hsum_epi32_avx(__m128i x)
{
    __m128i hi64  = _mm_unpackhi_epi64(x, x);           // 3-operand non-destructive AVX lets us save a byte without needing a movdqa
    __m128i sum64 = _mm_add_epi32(hi64, x);
    __m128i hi32  = _mm_shuffle_epi32(sum64, _MM_SHUFFLE(2, 3, 0, 1));    // Swap the low two elements
    __m128i sum32 = _mm_add_epi32(sum64, hi32);
    return _mm_cvtsi128_si32(sum32);       // movd
}

static inline uint32_t hsum_8x32(__m256i v)
{
    __m128i sum128 = _mm_add_epi32(
                 _mm256_castsi256_si128(v),
                 _mm256_extracti128_si256(v, 1)); // silly GCC uses a longer AXV512VL instruction if AVX512 is enabled :/
    return hsum_epi32_avx(sum128);
}

template<class Index, class Counter = index_t, class TRL = TriangleListing::Count<Index>>
void edge_iterator(TRL* lister,AdjacencyGraph<Index> *G, void *dummy = nullptr) {
    (void) dummy;



#if 0
    // According to sec. 4, the sorting is included in the execution time
    for (Counter u = 0; u < G->n; u++) {
        if (G->adjacency[u].count > 0) {
            quick_sort(G->adjacency[u].neighbors, 0, G->adjacency[u].count - 1);
        }
    }
#endif

    __m256i res_counter_a = _mm256_set1_epi32(0);
    __m256i res_counter_b = _mm256_set1_epi32(0);

    (void) res_counter_a;
    (void) res_counter_b;

    for (Counter si = 0; si < G->n; si++) {  // this should not count toward op count
        Index s = (Index) si;
        Counter Adj_s_size = G->adjacency[(index_t) s].count;
        if (Adj_s_size == 0) continue;
        const Index *Adj_s = G->adjacency[(index_t) s].neighbors;

        Counter ti = 0;
        for (; ti + 15 < Adj_s_size; ti+=16) {
            Index t_0 = Adj_s[ti];
            Index t_1 = Adj_s[ti + 1];
            Index t_2 = Adj_s[ti + 2];
            Index t_3 = Adj_s[ti + 3];
            Index t_4 = Adj_s[ti + 4];
            Index t_5 = Adj_s[ti + 5];
            Index t_6 = Adj_s[ti + 6];
            Index t_7 = Adj_s[ti + 7];

            Index t_8 = Adj_s[ti + 8];
            Index t_9 = Adj_s[ti + 9];
            Index t_10 = Adj_s[ti + 10];
            Index t_11 = Adj_s[ti + 11];
            Index t_12 = Adj_s[ti + 12];
            Index t_13 = Adj_s[ti + 13];
            Index t_14 = Adj_s[ti + 14];
            Index t_15 = Adj_s[ti + 15];

            AdjacencyList<Index> *At_0 = &G->adjacency[(index_t) t_0];
            AdjacencyList<Index> *At_1 = &G->adjacency[(index_t) t_1];
            AdjacencyList<Index> *At_2 = &G->adjacency[(index_t) t_2];
            AdjacencyList<Index> *At_3 = &G->adjacency[(index_t) t_3];
            AdjacencyList<Index> *At_4 = &G->adjacency[(index_t) t_4];
            AdjacencyList<Index> *At_5 = &G->adjacency[(index_t) t_5];
            AdjacencyList<Index> *At_6 = &G->adjacency[(index_t) t_6];
            AdjacencyList<Index> *At_7 = &G->adjacency[(index_t) t_7];

            AdjacencyList<Index> *At_8 = &G->adjacency[(index_t) t_8];
            AdjacencyList<Index> *At_9 = &G->adjacency[(index_t) t_9];
            AdjacencyList<Index> *At_10 = &G->adjacency[(index_t) t_10];
            AdjacencyList<Index> *At_11 = &G->adjacency[(index_t) t_11];
            AdjacencyList<Index> *At_12 = &G->adjacency[(index_t) t_12];
            AdjacencyList<Index> *At_13 = &G->adjacency[(index_t) t_13];
            AdjacencyList<Index> *At_14 = &G->adjacency[(index_t) t_14];
            AdjacencyList<Index> *At_15 = &G->adjacency[(index_t) t_15];

            __m256i i_vec_a = _mm256_set1_epi32(0);
            __m256i j_vec_a = _mm256_set1_epi32(0);

            __m256i i_vec_b = _mm256_set1_epi32(0);
            __m256i j_vec_b = _mm256_set1_epi32(0);

            __m256i incer = _mm256_set1_epi32(1);

            __m256i as_count = _mm256_set1_epi32(Adj_s_size);
            __m256i at_count_a = _mm256_set_epi32(
                (index_t)At_7->count, (index_t)At_6->count,(index_t) At_5->count, (index_t)At_4->count,
                (index_t)At_3->count, (index_t)At_2->count,(index_t) At_1->count, (index_t)At_0->count);

            __m256i at_count_b = _mm256_set_epi32(
                (index_t) At_15->count, (index_t)At_14->count,(index_t)At_13->count, (index_t)At_12->count,
                (index_t) At_11->count, (index_t)At_10->count, (index_t)At_9->count, (index_t)At_8->count);

            // uint32_t l[8];
            __m256i l_vec_a;
            __m256i l_vec_b;
            do {
                __m256i icount_a = _mm256_cmpgt_epi32(as_count, i_vec_a);
                __m256i jcount_a = _mm256_cmpgt_epi32(at_count_a, j_vec_a);
                __m256i icount_b = _mm256_cmpgt_epi32(as_count, i_vec_b);
                __m256i jcount_b = _mm256_cmpgt_epi32(at_count_b, j_vec_b);
                l_vec_a = _mm256_and_si256(icount_a, jcount_a);
                l_vec_b = _mm256_and_si256(icount_b, jcount_b);

                uint32_t j_mat_a[8];
                uint32_t j_mat_b[8];
                _mm256_store_si256((__m256i *)j_mat_a, j_vec_a);
                _mm256_store_si256((__m256i *)j_mat_b, j_vec_b);

                __m256i s_neigbours_a = _mm256_i32gather_epi32((int*)Adj_s, i_vec_a, 4);
                __m256i s_neigbours_b = _mm256_i32gather_epi32((int*)Adj_s, i_vec_b, 4);

                // Thats quite a bit of a hack.
                __m256i t_neigbours_a = _mm256_set_epi32(
                    (index_t)At_7->neighbors[j_mat_a[7]],(index_t) At_6->neighbors[j_mat_a[6]],
                    (index_t)At_5->neighbors[j_mat_a[5]],(index_t) At_4->neighbors[j_mat_a[4]],
                    (index_t)At_3->neighbors[j_mat_a[3]],(index_t) At_2->neighbors[j_mat_a[2]],
                    (index_t)At_1->neighbors[j_mat_a[1]],(index_t) At_0->neighbors[j_mat_a[0]]);

                __m256i t_neigbours_b = _mm256_set_epi32(
                    (index_t)At_15->neighbors[j_mat_b[7]],(index_t) At_14->neighbors[j_mat_b[6]],
                    (index_t)At_13->neighbors[j_mat_b[5]],(index_t) At_12->neighbors[j_mat_b[4]],
                    (index_t)At_11->neighbors[j_mat_b[3]],(index_t) At_10->neighbors[j_mat_b[2]],
                    (index_t)At_9->neighbors[j_mat_b[1]],(index_t) At_8->neighbors[j_mat_b[0]]);

                __m256i gt_a = _mm256_cmpgt_epi32(s_neigbours_a, t_neigbours_a);
                __m256i lt_a = _mm256_cmpgt_epi32(t_neigbours_a, s_neigbours_a);

                __m256i gt_b = _mm256_cmpgt_epi32(s_neigbours_b, t_neigbours_b);
                __m256i lt_b = _mm256_cmpgt_epi32(t_neigbours_b, s_neigbours_b);

                __m256i ge_and_l_a = _mm256_andnot_si256(lt_a, l_vec_a);
                __m256i le_and_l_a = _mm256_andnot_si256(gt_a, l_vec_a);

                __m256i ge_and_l_b = _mm256_andnot_si256(lt_b, l_vec_b);
                __m256i le_and_l_b = _mm256_andnot_si256(gt_b, l_vec_b);

                __m256i inc_mask_a = _mm256_and_si256(ge_and_l_a, le_and_l_a);
                __m256i inc_mask_b = _mm256_and_si256(ge_and_l_b, le_and_l_b);

                if constexpr (std::is_same_v<TRL, TriangleListing::Count<Index>>) {
                    __m256i inc_count_a = _mm256_and_si256(inc_mask_a, incer);
                    __m256i inc_count_b = _mm256_and_si256(inc_mask_b, incer);

                    res_counter_a = _mm256_add_epi32(res_counter_a, inc_count_a);
                    res_counter_b = _mm256_add_epi32(res_counter_b, inc_count_b);
                } else {
                    uint32_t trianlge_mask_a = _mm256_movemask_epi8(inc_mask_a);
                    uint32_t trianlge_mask_b = _mm256_movemask_epi8(inc_mask_b);
                    if(trianlge_mask_a) {
                        // If there is at leas one triangle
                        uint32_t s_neigbours_mat[8];
                        _mm256_store_si256((__m256i *)s_neigbours_mat, s_neigbours_a);
                        for (uint32_t k = 0, index = 0; k < 32; k += 4, index++) {
                            if ((trianlge_mask_a & (1 << k)) != 0 ) {
                                Index t = Adj_s[ti + index];
                                lister->list_triangle(s, t, (Index) s_neigbours_mat[index]);
                            }
                        }
                    }
                    if(trianlge_mask_b) {
                        // If there is at leas one triangle
                        uint32_t s_neigbours_mat[8];
                        _mm256_store_si256((__m256i *)s_neigbours_mat, s_neigbours_b);
                        for (uint32_t k = 0, index = 0; k < 32; k += 4, index++) {
                            if ((trianlge_mask_b & (1 << k)) != 0 ) {
                                Index t = Adj_s[ti + index + 8];
                                lister->list_triangle(s, t, (Index) s_neigbours_mat[index]);
                            }
                        }
                    }
                }

                __m256i inc_count_i_a = _mm256_and_si256(le_and_l_a, incer);
                __m256i inc_count_j_a = _mm256_and_si256(ge_and_l_a, incer);
                __m256i inc_count_i_b = _mm256_and_si256(le_and_l_b, incer);
                __m256i inc_count_j_b = _mm256_and_si256(ge_and_l_b, incer);
                i_vec_a = _mm256_add_epi32(i_vec_a, inc_count_i_a);
                j_vec_a = _mm256_add_epi32(j_vec_a, inc_count_j_a);

                i_vec_b = _mm256_add_epi32(i_vec_b, inc_count_i_b);
                j_vec_b = _mm256_add_epi32(j_vec_b, inc_count_j_b);

                // _mm256_store_si256((__m256i *)l, l_vec);

            } while (_mm256_movemask_epi8(l_vec_a) || _mm256_movemask_epi8(l_vec_b)); //(l[0] || l[1] || l[2] || l[3] || l[4] || l[5] || l[6] || l[7]);
        }

        for (; ti + 7 < Adj_s_size; ti+=8) {
            Index t_0 = Adj_s[ti];
            Index t_1 = Adj_s[ti + 1];
            Index t_2 = Adj_s[ti + 2];
            Index t_3 = Adj_s[ti + 3];

            Index t_4 = Adj_s[ti + 4];
            Index t_5 = Adj_s[ti + 5];
            Index t_6 = Adj_s[ti + 6];
            Index t_7 = Adj_s[ti + 7];

            AdjacencyList<Index> *At_0 = &G->adjacency[(index_t) t_0];
            AdjacencyList<Index> *At_1 = &G->adjacency[(index_t) t_1];
            AdjacencyList<Index> *At_2 = &G->adjacency[(index_t) t_2];
            AdjacencyList<Index> *At_3 = &G->adjacency[(index_t) t_3];
            AdjacencyList<Index> *At_4 = &G->adjacency[(index_t) t_4];
            AdjacencyList<Index> *At_5 = &G->adjacency[(index_t) t_5];
            AdjacencyList<Index> *At_6 = &G->adjacency[(index_t) t_6];
            AdjacencyList<Index> *At_7 = &G->adjacency[(index_t) t_7];

            __m256i i_vec = _mm256_set1_epi32(0);
            __m256i j_vec = _mm256_set1_epi32(0);

            __m256i incer = _mm256_set1_epi32(1);

            __m256i as_count = _mm256_set1_epi32(Adj_s_size);
            __m256i at_count = _mm256_set_epi32(
                (index_t)At_7->count, (index_t)At_6->count,(index_t) At_5->count, (index_t)At_4->count,
                (index_t)At_3->count, (index_t)At_2->count,(index_t) At_1->count, (index_t)At_0->count);

            // uint32_t l[8];
            __m256i l_vec;
            do {
                __m256i icount = _mm256_cmpgt_epi32(as_count, i_vec);
                __m256i jcount = _mm256_cmpgt_epi32(at_count, j_vec);
                l_vec = _mm256_and_si256(icount, jcount);

                uint32_t j_mat[8];
                _mm256_store_si256((__m256i *)j_mat, j_vec);

                __m256i s_neigbours = _mm256_i32gather_epi32((int*)Adj_s, i_vec, 4);

                // Thats quite a bit of a hack.
                __m256i t_neigbours = _mm256_set_epi32(
                    (index_t)At_7->neighbors[j_mat[7]],(index_t) At_6->neighbors[j_mat[6]],
                    (index_t)At_5->neighbors[j_mat[5]],(index_t) At_4->neighbors[j_mat[4]],
                    (index_t)At_3->neighbors[j_mat[3]],(index_t) At_2->neighbors[j_mat[2]],
                    (index_t)At_1->neighbors[j_mat[1]],(index_t) At_0->neighbors[j_mat[0]]);

                __m256i gt = _mm256_cmpgt_epi32(s_neigbours, t_neigbours);
                __m256i lt = _mm256_cmpgt_epi32(t_neigbours, s_neigbours);

                __m256i ge_and_l = _mm256_andnot_si256(lt, l_vec);
                __m256i le_and_l = _mm256_andnot_si256(gt, l_vec);
                __m256i inc_mask = _mm256_and_si256(ge_and_l, le_and_l);

                if constexpr (std::is_same_v<TRL, TriangleListing::Count<Index>>) {
                    __m256i inc_count = _mm256_and_si256(inc_mask, incer);
                    res_counter_a = _mm256_add_epi32(res_counter_a, inc_count);
                } else {
                    // If we need to list unpack here and inspect
                    uint32_t trianlge_mask = _mm256_movemask_epi8(inc_mask);
                    if(trianlge_mask) {
                        // If there is at leas one triangle
                        uint32_t s_neigbours_mat[8];
                        _mm256_store_si256((__m256i *)s_neigbours_mat, s_neigbours);
                        for (uint32_t k = 0, index = 0; k < 32; k += 4, index++) {
                            if ((trianlge_mask & (1 << k)) != 0 ) {
                                Index t = Adj_s[ti + index];
                                lister->list_triangle(s, t, (Index) s_neigbours_mat[index]);
                            }
                        }
                    }
                }
                __m256i inc_count_i = _mm256_and_si256(le_and_l, incer);
                __m256i inc_count_j = _mm256_and_si256(ge_and_l, incer);
                i_vec = _mm256_add_epi32(i_vec, inc_count_i);
                j_vec = _mm256_add_epi32(j_vec, inc_count_j);
            } while (_mm256_movemask_epi8(l_vec)); //(l[0] || l[1] || l[2] || l[3] || l[4] || l[5] || l[6] || l[7]);
        }

        for (; ti < Adj_s_size; ti++) {
            Index t = Adj_s[ti];
            Counter Adj_t_size = G->adjacency[(index_t) t].count;
            Index *Adj_t = G->adjacency[(index_t) t].neighbors;
            Counter i = 0, j = 0;

            while (i < Adj_s_size && j < Adj_t_size) {
                if (Adj_s[i] == Adj_t[j]) {
                    lister->list_triangle(s, t, Adj_s[i]);
                    i++;
                    j++;
                } else if (Adj_s[i] < Adj_t[j]) {
                    i++;
                } else {
                    j++;
                }
            }
        }
    }
    // end:
    if constexpr (std::is_same_v<TRL, TriangleListing::Count<Index>>) {
        __m256i res = _mm256_add_epi32(res_counter_a, res_counter_b);
        lister->count += hsum_8x32(res);
    }

}

}

#endif //TEAM02_EDGE_ITER_U6_H
