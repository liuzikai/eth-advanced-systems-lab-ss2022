#ifndef TEAM02_FORWARD_V4_H
#define TEAM02_FORWARD_V4_H

#include "common.h"
#include "adjacency_graph.h"
#include "instrumented_index.h"
#include "triangle_lister.h"
#include "quick_sort.h"

#include "instrumented_immintrin.h"
//#include <immintrin.h>
namespace f4 {

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
        A->adjacency[u].neighbors = new Index[G->adjacency[u].orig_count];
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

static inline uint32_t hor_epi32_avx(__m128i x)
{
    __m128i hi64  = _mm_unpackhi_epi64(x, x);           // 3-operand non-destructive AVX lets us save a byte without needing a movdqa
    __m128i sum64 = _mm_or_si128(hi64, x);
    __m128i hi32  = _mm_shuffle_epi32(sum64, _MM_SHUFFLE(2, 3, 0, 1));    // Swap the low two elements
    __m128i sum32 = _mm_or_si128(sum64, hi32);
    return _mm_cvtsi128_si32(sum32);       // movd
}

static inline uint32_t hor_8x32(__m256i v)
{
    __m128i sum128 = _mm_or_si128( 
                 _mm256_castsi256_si128(v),
                 _mm256_extracti128_si256(v, 1)); // silly GCC uses a longer AXV512VL instruction if AVX512 is enabled :/
    return hsum_epi32_avx(sum128);
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

    // According to sec. 4, the sorting is included in the execution time
    // for (Counter u = 0; u < G->n; u++) {
    //     if (G->adjacency[u].count > 0) {
    //         quick_sort(G->adjacency[u].neighbors, 0, G->adjacency[u].count - 1);
    //     }
    // }

    __m256i res_counter_a = _mm256_set1_epi32(0);
    __m256i res_counter_b = _mm256_set1_epi32(0);

    (void) res_counter_a;
    (void) res_counter_b;

    for (Counter si = 0; si < G->n; si++) {  // this should not count toward op count
        Index s = (Index) si;
        ForwardNeighbourList<Index> *As = &A->adjacency[(index_t) s];
        AdjacencyList<Index>* Gs = &G->adjacency[(index_t) s];

        Counter ti = 0;
        Counter s_count = G->adjacency[(index_t) s].count;
        for (; ti + 15 < s_count; ti+=16) {
            Index t_0 = Gs->neighbors[ti];
            Index t_1 = Gs->neighbors[ti + 1];
            Index t_2 = Gs->neighbors[ti + 2];
            Index t_3 = Gs->neighbors[ti + 3];
            Index t_4 = Gs->neighbors[ti + 4];
            Index t_5 = Gs->neighbors[ti + 5];
            Index t_6 = Gs->neighbors[ti + 6];
            Index t_7 = Gs->neighbors[ti + 7];

            Index t_8 = Gs->neighbors[ti + 8];
            Index t_9 = Gs->neighbors[ti + 9];
            Index t_10 = Gs->neighbors[ti + 10];
            Index t_11 = Gs->neighbors[ti + 11];
            Index t_12 = Gs->neighbors[ti + 12];
            Index t_13 = Gs->neighbors[ti + 13];
            Index t_14 = Gs->neighbors[ti + 14];
            Index t_15 = Gs->neighbors[ti + 15];

          
            ForwardNeighbourList<Index> *At_0 = &A->adjacency[(index_t) t_0];
            ForwardNeighbourList<Index> *At_1 = &A->adjacency[(index_t) t_1];
            ForwardNeighbourList<Index> *At_2 = &A->adjacency[(index_t) t_2];
            ForwardNeighbourList<Index> *At_3 = &A->adjacency[(index_t) t_3];
            ForwardNeighbourList<Index> *At_4 = &A->adjacency[(index_t) t_4];
            ForwardNeighbourList<Index> *At_5 = &A->adjacency[(index_t) t_5];
            ForwardNeighbourList<Index> *At_6 = &A->adjacency[(index_t) t_6];
            ForwardNeighbourList<Index> *At_7 = &A->adjacency[(index_t) t_7];   

            ForwardNeighbourList<Index> *At_8 = &A->adjacency[(index_t) t_8];
            ForwardNeighbourList<Index> *At_9 = &A->adjacency[(index_t) t_9];
            ForwardNeighbourList<Index> *At_10 = &A->adjacency[(index_t) t_10];
            ForwardNeighbourList<Index> *At_11 = &A->adjacency[(index_t) t_11];
            ForwardNeighbourList<Index> *At_12 = &A->adjacency[(index_t) t_12];
            ForwardNeighbourList<Index> *At_13 = &A->adjacency[(index_t) t_13];
            ForwardNeighbourList<Index> *At_14 = &A->adjacency[(index_t) t_14];
            ForwardNeighbourList<Index> *At_15 = &A->adjacency[(index_t) t_15];
       

            __m256i i_vec_a = _mm256_set1_epi32(0);
            __m256i j_vec_a = _mm256_set1_epi32(0);

            __m256i i_vec_b = _mm256_set1_epi32(0);
            __m256i j_vec_b = _mm256_set1_epi32(0);

            __m256i incer = _mm256_set1_epi32(1);

            __m256i as_count = _mm256_set1_epi32(As->count);
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

                __m256i s_neigbours_a = _mm256_i32gather_epi32((int*)As->neighbors, i_vec_a, 4);
                __m256i s_neigbours_b = _mm256_i32gather_epi32((int*)As->neighbors, i_vec_b, 4);
                
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

                __m256i eq_a = _mm256_cmpeq_epi32(s_neigbours_a, t_neigbours_a);
                __m256i gt_a = _mm256_cmpgt_epi32(s_neigbours_a, t_neigbours_a);
                __m256i lt_a = _mm256_cmpgt_epi32(t_neigbours_a, s_neigbours_a);

                __m256i eq_b = _mm256_cmpeq_epi32(s_neigbours_b, t_neigbours_b);
                __m256i gt_b = _mm256_cmpgt_epi32(s_neigbours_b, t_neigbours_b);
                __m256i lt_b = _mm256_cmpgt_epi32(t_neigbours_b, s_neigbours_b);
                
                __m256i ge_a = _mm256_or_si256(eq_a, gt_a);  
                __m256i le_a = _mm256_or_si256(eq_a, lt_a);  

                __m256i ge_b = _mm256_or_si256(eq_b, gt_b);  
                __m256i le_b = _mm256_or_si256(eq_b, lt_b);  

                __m256i ge_and_l_a = _mm256_and_si256(ge_a, l_vec_a);
                __m256i le_and_l_a = _mm256_and_si256(le_a, l_vec_a); 

                __m256i ge_and_l_b = _mm256_and_si256(ge_b, l_vec_b);
                __m256i le_and_l_b = _mm256_and_si256(le_b, l_vec_b); 
    
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
                                Index t = Gs->neighbors[ti + index];
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
                                Index t = Gs->neighbors[ti + index + 8];
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
            
            } while (hor_8x32(l_vec_a) || hor_8x32(l_vec_b)); //(l[0] || l[1] || l[2] || l[3] || l[4] || l[5] || l[6] || l[7]);

            At_0->neighbors[At_0->count++] = s;
            At_1->neighbors[At_1->count++] = s;
            At_2->neighbors[At_2->count++] = s;
            At_3->neighbors[At_3->count++] = s;

            At_4->neighbors[At_4->count++] = s;
            At_5->neighbors[At_5->count++] = s;
            At_6->neighbors[At_6->count++] = s;
            At_7->neighbors[At_7->count++] = s;

            At_8->neighbors[At_8->count++] = s;
            At_9->neighbors[At_9->count++] = s;
            At_10->neighbors[At_10->count++] = s;
            At_11->neighbors[At_11->count++] = s;
            At_12->neighbors[At_12->count++] = s;
            At_13->neighbors[At_13->count++] = s;
            At_14->neighbors[At_14->count++] = s;
            At_15->neighbors[At_15->count++] = s;
            
            
        }

        for (; ti < G->adjacency[(index_t) s].count; ti++) {
            Index t = Gs->neighbors[ti];
            Counter i = 0, j = 0;
            
            ForwardNeighbourList<Index> *At = &A->adjacency[(index_t) t];
            while (i < As->count && j < At->count) {
                if (As->neighbors[i] == At->neighbors[j]) {
                    lister->list_triangle(s, t, As->neighbors[i]);
                    i++;
                    j++;
                } else if (As->neighbors[i] < At->neighbors[j]) {
                    i++;
                } else {
                    j++;
                }
            }

            At->neighbors[At->count++] = s;
            
        }
    }

    if constexpr (std::is_same_v<TRL, TriangleListing::Count<Index>>) {
        __m256i res = _mm256_add_epi32(res_counter_a, res_counter_b);
        lister->count += hsum_8x32(res);
    }
    
    
}

}

#endif //TEAM02_FORWARD_H
