#ifndef TEAM02_FORWARD_V1_H
#define TEAM02_FORWARD_V1_H

#include "common.h"
#include "adjacency_graph.h"
#include "instrumented_index.h"
#include "triangle_lister.h"
#include "quick_sort.h"

#include "instrumented_immintrin.h"
//#include <immintrin.h>
namespace f1 {

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

    __m256i res_counter = _mm256_set1_epi64x(0);

    for (Counter si = 0; si < G->n; si++) {  // this should not count toward op count
        Index s = (Index) si;
        ForwardNeighbourList<Index> *As = &A->adjacency[(index_t) s];
        AdjacencyList<Index>* Gs = &G->adjacency[(index_t) s];

        Counter ti = 0;
        for (; ti + 3 < G->adjacency[(index_t) s].count; ti+=4) {
            Index t_0 = Gs->neighbors[ti];

            Index t_1 = Gs->neighbors[ti + 1];

            Index t_2 = Gs->neighbors[ti + 2];

            Index t_3 = Gs->neighbors[ti + 3];
          
            ForwardNeighbourList<Index> *At_0 = &A->adjacency[(index_t) t_0];
            ForwardNeighbourList<Index> *At_1 = &A->adjacency[(index_t) t_1];
            ForwardNeighbourList<Index> *At_2 = &A->adjacency[(index_t) t_2];
            ForwardNeighbourList<Index> *At_3 = &A->adjacency[(index_t) t_3];           

            __m256i i_vec = _mm256_set1_epi64x(0);
            __m256i j_vec = _mm256_set1_epi64x(0);

            __m256i incer = _mm256_set1_epi64x(1);

            __m256i as_count = _mm256_set1_epi64x(As->count);
            __m256i at_count = _mm256_set_epi64x((index_t)At_3->count, (index_t)At_2->count,(index_t) At_1->count, (index_t)At_0->count);

            size_t l[4];
            do {
                __m256i icount = _mm256_cmpgt_epi64(as_count, i_vec);
                __m256i jcount = _mm256_cmpgt_epi64(at_count, j_vec);
                __m256i l_vec = _mm256_and_si256(icount, jcount);

                // This will only work when we change vector size from 64 to to 32
                //__m256i s_neigbours = _mm256_i64gather_epi64((const long long int *) As->neighbors, i_vec, 1);
                // This will not work....
                //__m256i t_neigbours = _mm256_i64gather_epi64((const long long int *) At_0->neighbors, j_vec, 1);
                size_t j_mat[4];
                size_t i_mat[4];
                _mm256_store_si256((__m256i *)j_mat, j_vec);
                _mm256_store_si256((__m256i *)i_mat, i_vec);

                __m256i s_neigbours = _mm256_set_epi64x((index_t)As->neighbors[i_mat[3]], (index_t)As->neighbors[i_mat[2]], (index_t)As->neighbors[i_mat[1]], (index_t)As->neighbors[i_mat[0]]);
                __m256i t_neigbours = _mm256_set_epi64x((index_t)At_3->neighbors[j_mat[3]],(index_t) At_2->neighbors[j_mat[2]], (index_t)At_1->neighbors[j_mat[1]],(index_t) At_0->neighbors[j_mat[0]]);

                __m256i eq = _mm256_cmpeq_epi64(s_neigbours, t_neigbours);
                __m256i gt = _mm256_cmpgt_epi64(s_neigbours, t_neigbours);
                __m256i lt = _mm256_cmpgt_epi64(t_neigbours, s_neigbours);
                
                __m256i ge = _mm256_or_si256(eq, gt);  
                __m256i le = _mm256_or_si256(eq, lt);  

                __m256i ge_and_l = _mm256_and_si256(ge, l_vec);
                __m256i le_and_l = _mm256_and_si256(le, l_vec);

                
    
                __m256i inc_mask = _mm256_and_si256(ge_and_l, le_and_l);
                __m256i inc_count = _mm256_and_si256(inc_mask, incer);
                res_counter = _mm256_add_epi64(res_counter, inc_count);

                __m256i inc_count_i = _mm256_and_si256(le_and_l, incer);
                __m256i inc_count_j = _mm256_and_si256(ge_and_l, incer);
                i_vec = _mm256_add_epi64(i_vec, inc_count_i);
                j_vec = _mm256_add_epi64(j_vec, inc_count_j);
               
                _mm256_store_si256((__m256i *)l, l_vec);     
            
            } while (l[0] || l[1] || l[2] || l[3]);

            At_0->neighbors[At_0->count++] = s;
            At_1->neighbors[At_1->count++] = s;
            At_2->neighbors[At_2->count++] = s;
            At_3->neighbors[At_3->count++] = s;
            
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
        size_t res[4];
        _mm256_store_si256((__m256i *)res, res_counter);
        lister.count += res[0] + res[1] + res[2] + res[3];
    }
    
    return lister;
}

}

#endif //TEAM02_FORWARD_H
