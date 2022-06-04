#ifndef TEAM02_FORWARD_V1_H
#define TEAM02_FORWARD_V1_H

#include "common.h"
#include "adjacency_graph.h"
#include "instrumented_index.h"
#include "triangle_lister.h"
#include "quick_sort.h"

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

    // According to sec. 4, the sorting is included in the execution time
    // for (Counter u = 0; u < G->n; u++) {
    //     if (G->adjacency[u].count > 0) {
    //         quick_sort(G->adjacency[u].neighbors, 0, G->adjacency[u].count - 1);
    //     }
    // }

    for (Counter si = 0; si < G->n; si++) {  // this should not count toward op count
        Index s = (Index) si;
        ForwardNeighbourList<Index> *As = &A->adjacency[(index_t) s];
        AdjacencyList<Index>* Gs = &G->adjacency[(index_t) s];

        Counter ti = 0;
        for (; ti + 3 < G->adjacency[(index_t) s].count; ti+=4) {
            Index t_0 = Gs->neighbors[ti];
            Counter i_0 = 0, j_0 = 0;

            Index t_1 = Gs->neighbors[ti + 1];
            Counter i_1 = 0, j_1 = 0;

            Index t_2 = Gs->neighbors[ti + 2];
            Counter i_2 = 0, j_2 = 0;

            Index t_3 = Gs->neighbors[ti + 3];
            Counter i_3 = 0, j_3 = 0;

            
            ForwardNeighbourList<Index> *At_0 = &A->adjacency[(index_t) t_0];
            ForwardNeighbourList<Index> *At_1 = &A->adjacency[(index_t) t_1];
            ForwardNeighbourList<Index> *At_2 = &A->adjacency[(index_t) t_2];
            ForwardNeighbourList<Index> *At_3 = &A->adjacency[(index_t) t_3];           


            bool l_0, l_1, l_2, l_3;
            do {
                l_0 = i_0 < As->count && j_0 < At_0->count;
                l_1 = i_1 < As->count && j_1 < At_1->count;
                l_2 = i_2 < As->count && j_2 < At_2->count;
                l_3 = i_3 < As->count && j_3 < At_3->count;

                bool se_0 = As->neighbors[i_0] <= At_0->neighbors[j_0] && l_0;
                bool ge_0 = As->neighbors[i_0] >= At_0->neighbors[j_0] && l_0;

                bool se_1 = As->neighbors[i_1] <= At_1->neighbors[j_1] && l_1;
                bool ge_1 = As->neighbors[i_1] >= At_1->neighbors[j_1] && l_1;

                bool se_2 = As->neighbors[i_2] <= At_2->neighbors[j_2] && l_2;
                bool ge_2 = As->neighbors[i_2] >= At_2->neighbors[j_2] && l_2;

                bool se_3 = As->neighbors[i_3] <= At_3->neighbors[j_3] && l_3;
                bool ge_3 = As->neighbors[i_3] >= At_3->neighbors[j_3] && l_3;
    
                
                if constexpr (std::is_same_v<TRL,TriangleListing::Count<Index>>) {
                    lister->count += se_0 && ge_0;
                    lister->count += se_1 && ge_1;
                    lister->count += se_2 && ge_2;
                    lister->count += se_3 && ge_3;
                } else {
                    if (se_0 && ge_0) {
                        lister->list_triangle(s, t_0, As->neighbors[i_0]);
                    }
                    if (se_1 && ge_1 ) {
                        lister->list_triangle(s, t_1, As->neighbors[i_1]);
                    }
                    if (se_2 && ge_2) {
                        lister->list_triangle(s, t_2, As->neighbors[i_2]);
                    }
                    if (se_3 && ge_3 ) {
                        lister->list_triangle(s, t_3, As->neighbors[i_3]);
                    }
                }

                i_0 += se_0;
                j_0 += ge_0;
                i_1 += se_1;
                j_1 += ge_1;
                i_2 += se_2;
                j_2 += ge_2;
                i_3 += se_3;
                j_3 += ge_3;

                    
             
            } while (l_0 || l_1 || l_2 || l_3);

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

    
}

}

#endif //TEAM02_FORWARD_H
