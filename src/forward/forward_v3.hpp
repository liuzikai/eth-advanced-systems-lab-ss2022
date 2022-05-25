#ifndef TEAM02_FORWARD_V3_H
#define TEAM02_FORWARD_V3_H

#include "common.h"
#include "adjacency_graph.h"
#include "instrumented_index.h"
#include "triangle_lister.h"
#include "quick_sort.h"

#include "instrumented_immintrin.h"
//#include <immintrin.h>
namespace f3 {

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
TRL forward(AdjacencyGraph<Index> *G, ForwardNeighborContainer<Index> *A) {
    TRL lister;
    forward_reset_neighbor_container(G, A);

    // According to sec. 4, the sorting is included in the execution time
    // for (Counter u = 0; u < G->n; u++) {
    //     if (G->adjacency[u].count > 0) {
    //         quick_sort(G->adjacency[u].neighbors, 0, G->adjacency[u].count - 1);
    //     }
    // }

    __m256i res_counter = _mm256_set1_epi32(0);

    for (Counter si = 0; si < G->n; si++) {  // this should not count toward op count
        Index s = (Index) si;
        ForwardNeighbourList<Index> *As = &A->adjacency[(index_t) s];
        AdjacencyList<Index>* Gs = &G->adjacency[(index_t) s];

        Counter ti = 0;
        for (; ti + 7 < G->adjacency[(index_t) s].count; ti+=8) {
            Index t_0 = Gs->neighbors[ti];
            Index t_1 = Gs->neighbors[ti + 1];
            Index t_2 = Gs->neighbors[ti + 2];
            Index t_3 = Gs->neighbors[ti + 3];

            Index t_4 = Gs->neighbors[ti + 4];
            Index t_5 = Gs->neighbors[ti + 5];
            Index t_6 = Gs->neighbors[ti + 6];
            Index t_7 = Gs->neighbors[ti + 7];
          
            ForwardNeighbourList<Index> *At_0 = &A->adjacency[(index_t) t_0];
            ForwardNeighbourList<Index> *At_1 = &A->adjacency[(index_t) t_1];
            ForwardNeighbourList<Index> *At_2 = &A->adjacency[(index_t) t_2];
            ForwardNeighbourList<Index> *At_3 = &A->adjacency[(index_t) t_3];
            ForwardNeighbourList<Index> *At_4 = &A->adjacency[(index_t) t_4];
            ForwardNeighbourList<Index> *At_5 = &A->adjacency[(index_t) t_5];
            ForwardNeighbourList<Index> *At_6 = &A->adjacency[(index_t) t_6];
            ForwardNeighbourList<Index> *At_7 = &A->adjacency[(index_t) t_7];           

            __m256i i_vec = _mm256_set1_epi32(0);
            __m256i j_vec = _mm256_set1_epi32(0);

            __m256i incer = _mm256_set1_epi32(1);

            __m256i as_count = _mm256_set1_epi32(As->count);
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

                __m256i s_neigbours = _mm256_i32gather_epi32((int*)As->neighbors, i_vec, 4);
                
                // Thats quite a bit of a hack.
                __m256i t_neigbours = _mm256_set_epi32(
                    (index_t)At_7->neighbors[j_mat[7]],(index_t) At_6->neighbors[j_mat[6]], 
                    (index_t)At_5->neighbors[j_mat[5]],(index_t) At_4->neighbors[j_mat[4]],
                    (index_t)At_3->neighbors[j_mat[3]],(index_t) At_2->neighbors[j_mat[2]], 
                    (index_t)At_1->neighbors[j_mat[1]],(index_t) At_0->neighbors[j_mat[0]]);

                __m256i eq = _mm256_cmpeq_epi32(s_neigbours, t_neigbours);
                __m256i gt = _mm256_cmpgt_epi32(s_neigbours, t_neigbours);
                __m256i lt = _mm256_cmpgt_epi32(t_neigbours, s_neigbours);
                
                __m256i ge = _mm256_or_si256(eq, gt);  
                __m256i le = _mm256_or_si256(eq, lt);  

                __m256i ge_and_l = _mm256_and_si256(ge, l_vec);
                __m256i le_and_l = _mm256_and_si256(le, l_vec); 
    
                __m256i inc_mask = _mm256_and_si256(ge_and_l, le_and_l);
                __m256i inc_count = _mm256_and_si256(inc_mask, incer);
                res_counter = _mm256_add_epi32(res_counter, inc_count);

                __m256i inc_count_i = _mm256_and_si256(le_and_l, incer);
                __m256i inc_count_j = _mm256_and_si256(ge_and_l, incer);
                i_vec = _mm256_add_epi32(i_vec, inc_count_i);
                j_vec = _mm256_add_epi32(j_vec, inc_count_j);
               
                // _mm256_store_si256((__m256i *)l, l_vec);     
            
            } while (hor_8x32(l_vec)); //(l[0] || l[1] || l[2] || l[3] || l[4] || l[5] || l[6] || l[7]);

            At_0->neighbors[At_0->count++] = s;
            At_1->neighbors[At_1->count++] = s;
            At_2->neighbors[At_2->count++] = s;
            At_3->neighbors[At_3->count++] = s;

            At_4->neighbors[At_4->count++] = s;
            At_5->neighbors[At_5->count++] = s;
            At_6->neighbors[At_6->count++] = s;
            At_7->neighbors[At_7->count++] = s;
            
        }

        for (; ti < G->adjacency[(index_t) s].count; ti++) {
            Index t = Gs->neighbors[ti];
            Counter i = 0, j = 0;
            
            ForwardNeighbourList<Index> *At = &A->adjacency[(index_t) t];
            while (i < As->count && j < At->count) {
                if (As->neighbors[i] == At->neighbors[j]) {
                    lister.list_triangle(s, t, As->neighbors[i]);
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
        lister.count += hsum_8x32(res_counter);
 }
    
    return lister;
}

}

#endif //TEAM02_FORWARD_H
