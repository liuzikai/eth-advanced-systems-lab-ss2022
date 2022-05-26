#ifndef TEAM02_EDGE_ITER_U4_H
#define TEAM02_EDGE_ITER_U4_H

#include "common.h"
#include "adjacency_graph.h"
#include "triangle_lister.h"
#include "quick_sort.h"

namespace eu4 {

#define UNROLL_FACTOR 4

template<class Index, class Counter = index_t, class TRL = TriangleListing::Count<Index>>
TRL edge_iterator(AdjacencyGraph<Index> *G, void *dummy = nullptr) {
    (void) dummy;

    TRL lister;

#if 0
    // According to sec. 4, the sorting is included in the execution time
    for (Counter u = 0; u < G->n; u++) {
        if (G->adjacency[u].count > 0) {
            quick_sort(G->adjacency[u].neighbors, 0, G->adjacency[u].count - 1);
        }
    }
#endif

    for (Counter si = 0; si < G->n; si++) {  // this should not count toward op count
        Index s = (Index) si;


        Counter Adj_s_size = G->adjacency[(index_t) s].count;
        if (Adj_s_size == 0) continue;
        const Index *Adj_s = G->adjacency[(index_t) s].neighbors;

        Counter i = 0;

        {
            Counter Adj_t_size[UNROLL_FACTOR];
            const Index *Adj_t[UNROLL_FACTOR];
            Counter x[UNROLL_FACTOR], y[UNROLL_FACTOR];
            for (; i + UNROLL_FACTOR - 1 < Adj_s_size; i += UNROLL_FACTOR) {
                Counter cnt = 0;
                while (cnt < UNROLL_FACTOR) {
                    Index tt = Adj_s[i + cnt];
                    Adj_t_size[cnt] = G->adjacency[(index_t) tt].count;
                    Adj_t[cnt] = G->adjacency[(index_t) tt].neighbors;
                    x[cnt] = y[cnt] = 0;
                    cnt++;
                }

                for (Counter k = 0; k < UNROLL_FACTOR; k++) {
                    while (y[k] < Adj_t_size[k] && Adj_t[k][y[k]] < Adj_s[0]) y[k]++;
                }

                do {
                    cnt = 0;
                    for (Counter k = 0; k < UNROLL_FACTOR; k++) {
                        if (x[k] < Adj_s_size && y[k] < Adj_t_size[k]) {
                            if (Adj_s[x[k]] == Adj_t[k][y[k]]) {
                                lister.list_triangle(s, Adj_s[i + k], Adj_t[k][y[k]]);
                            }
                            bool b1 = Adj_s[x[k]] <= Adj_t[k][y[k]];
                            bool b2 = Adj_s[x[k]] >= Adj_t[k][y[k]];
                            x[k] += b1;
                            y[k] += b2;
                        } else {
                            cnt++;
                        }
                    }
                } while (cnt < UNROLL_FACTOR);

            }
        }

        for (Counter ti = i; ti < Adj_s_size; ti++) {
            Index t = Adj_s[ti];

            Counter Adj_t_size = G->adjacency[(index_t) t].count;
            Index *Adj_t = G->adjacency[(index_t) t].neighbors;

//            if (s < t) {
            Counter x = 0, y = 0;
//                for (x = 0; x < Adj_s_size; x++) {
//                    for (y = 0; y < Adj_t_size; y++) {
//                        if (Adj_s[x] == Adj_t[y]) {
//                            if (Adj_t[y] > t) {
//                                lister.list_triangle(s, t, Adj_t[y]);
//                            }
//                        }
//                    }
//                }
            // find the intersection of s's and t's neighbors
            while (x < Adj_s_size && y < Adj_t_size) {
                if (Adj_s[x] == Adj_t[y]) {
                    if (Adj_t[y] > t) {
                        lister.list_triangle(s, t, Adj_t[y]);
                    }
                    x++;
                    y++;
                } else if (Adj_s[x] < Adj_t[y]) {
                    x++;
                } else {
                    y++;
                }
            }
//            }
        }
    }

    return lister;

}

}

#endif //TEAM02_EDGE_ITER_U4_H
