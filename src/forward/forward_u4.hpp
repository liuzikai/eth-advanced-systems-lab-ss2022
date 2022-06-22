#ifndef TEAM02_FORWARD_U4_H
#define TEAM02_FORWARD_U4_H

#include "common.h"
#include "adjacency_graph.h"
#include "instrumented_index.h"
#include "triangle_lister.h"
#include "quick_sort.h"
#include "merge_sort/merge_sort_v4.h"

namespace fu4 {

#define UNROLL_FACTOR 4

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
        index_t container_size;
        if(G->adjacency[u].orig_count == G->adjacency[u].count) {
            // No cutting happened.
            container_size = G->adjacency[u].orig_count;
        } else {
            container_size = G->adjacency[u].orig_count - G->adjacency[u].count;
        }
        A->adjacency[u].neighbors = new Index[container_size];
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

    #ifdef SORTING
    static Index sort_buf[10800];
    // According to sec. 4, the sorting is included in the execution time
    for (Counter u = 0; u < G->n; u++) {
        if (G->adjacency[u].count > 0) {
            quick_cut<Index>(G->adjacency[u].neighbors, 0, G->adjacency[u].count - 1, (Index) u, &G->adjacency[u].count);
            if (G->adjacency[u].count == 0) continue;
            // std::sort(G->adjacency[u].neighbors, G->adjacency[u].neighbors + G->adjacency[u].count);
            ms4::merge_sort(G->adjacency[u].neighbors, sort_buf, G->adjacency[u].count);
        }
    }
    #endif

    for (Counter si = 0; si < G->n; si++) {  // this should not count toward op count
        Index s = (Index) si;

        Counter Adj_s_size = G->adjacency[(index_t) s].count;
        if (Adj_s_size == 0) continue;
        const Index *Adj_s = G->adjacency[(index_t) s].neighbors;
        Counter As_size = A->adjacency[(index_t) s].count;
        Index *As = A->adjacency[(index_t) s].neighbors;

        Counter ti = 0;
        {
            Counter At_size[UNROLL_FACTOR];
            Index *At[UNROLL_FACTOR];
            Counter x[UNROLL_FACTOR], y[UNROLL_FACTOR];
            for (; ti + UNROLL_FACTOR - 1 < Adj_s_size; ti += UNROLL_FACTOR) {
                Counter cnt = 0;
                while (cnt < UNROLL_FACTOR) {
                    Index tt = Adj_s[ti + cnt];
                    At_size[cnt] = A->adjacency[(index_t) tt].count;
                    At[cnt] = A->adjacency[(index_t) tt].neighbors;
                    x[cnt] = y[cnt] = 0;
                    cnt++;
                }

                if (As_size > 0) {
                    for (Counter k = 0; k < UNROLL_FACTOR; k++) {
                        while (y[k] < At_size[k] && At[k][y[k]] < As[0]) y[k]++;
                    }

                    do {
                        cnt = 0;
                        for (Counter k = 0; k < UNROLL_FACTOR; k++) {
                            if (x[k] < As_size && y[k] < At_size[k]) {
                                if (As[x[k]] == At[k][y[k]]) {
                                    lister->list_triangle(s, Adj_s[ti + k], As[x[k]]);
                                }
                                bool b1 = As[x[k]] <= At[k][y[k]];
                                bool b2 = As[x[k]] >= At[k][y[k]];
                                x[k] += b1;
                                y[k] += b2;
                            } else {
                                cnt++;
                            }
                        }
                    } while (cnt < UNROLL_FACTOR);
                }

                for (Counter k = 0; k < UNROLL_FACTOR; k++) {
                    At[k][A->adjacency[(index_t) Adj_s[ti + k]].count++] = s;
                }

            }
        }

        for (; ti < Adj_s_size; ti++) {
            Index t = Adj_s[ti];

            Counter At_size = A->adjacency[(index_t) t].count;
            Index *At = A->adjacency[(index_t) t].neighbors;

            Counter x = 0, y = 0;
            while (x < As_size && y < At_size) {
                if (As[x] == At[y]) {
                    lister->list_triangle(s, t, At[y]);
                    x++;
                    y++;
                } else if (As[x] < At[y]) {
                    x++;
                } else {
                    y++;
                }
            }

            At[A->adjacency[(index_t) t].count++] = s;
        }
    }

}

}

#endif //TEAM02_FORWARD_U4_H
