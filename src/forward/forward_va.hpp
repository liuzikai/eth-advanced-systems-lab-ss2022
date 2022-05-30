#ifndef TEAM02_FORWARD_VA_H
#define TEAM02_FORWARD_VA_H

#include "common.h"
#include "adjacency_graph.h"
#include "instrumented_index.h"
#include "triangle_lister.h"
#include "quick_sort.h"

namespace fa {

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

        for (Counter ti = 0; ti < G->adjacency[(index_t) s].count; ti++) {
            Index t = G->adjacency[(index_t) s].neighbors[ti];

            Counter i = 0, j = 0;
            ForwardNeighbourList<Index> *As = &A->adjacency[(index_t) s];
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

#endif //TEAM02_FORWARD_VA_H
