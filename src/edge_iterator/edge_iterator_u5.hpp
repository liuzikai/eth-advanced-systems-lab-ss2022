#ifndef TEAM02_EDGE_ITER_U5_H
#define TEAM02_EDGE_ITER_U5_H

#include "common.h"
#include "adjacency_graph.h"
#include "triangle_lister.h"
#include "quick_sort.h"

namespace eu5 {

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

    for (Counter si = 0; si < G->n; si++) {  // this should not count toward op count
        Index s = (Index) si;


        Counter Adj_s_size = G->adjacency[(index_t) s].count;
        if (Adj_s_size == 0) continue;
        const Index *Adj_s = G->adjacency[(index_t) s].neighbors;

        Counter ti = 0;
        {
            // check one s against 4 t's
            Counter Adj_t_size[4];
            const Index *Adj_t[4];
            Counter i[4], j[4];
            for (; ti + 3 < Adj_s_size; ti += 4) {
                Index tt0 = Adj_s[ti];
                Index tt1 = Adj_s[ti + 1];
                Index tt2 = Adj_s[ti + 2];
                Index tt3 = Adj_s[ti + 3];
                for (Counter k = 0; k < 4; k++) {
                    Adj_t_size[k] = G->adjacency[(Counter) Adj_s[ti+k]].count;
                    Adj_t[k] = G->adjacency[(Counter) Adj_s[ti+k]].neighbors;
                    i[k] = j[k] = 0;
                }

                for (Counter k = 0; k < 4; k++) {
                    while (j[k] < Adj_t_size[k] && Adj_t[k][j[k]] < Adj_s[0]) j[k]++;
                }

                Counter cnt = 0;
                do {
                    cnt = 0;
                    if (i[0] < Adj_s_size && j[0] < Adj_t_size[0]) {
                        Index s0 = Adj_s[i[0]];
                        Index t0 = Adj_t[0][j[0]];
                        if (s0 == t0) {
                            lister->list_triangle(s, tt0, s0);
                        }
                        bool b1 = s0 <= t0;
                        bool b2 = s0 >= t0;
                        i[0] += b1;
                        j[0] += b2;
                    } else {
                        cnt++;
                    }
                    if (i[1] < Adj_s_size && j[1] < Adj_t_size[1]) {
                        Index s1 = Adj_s[i[1]];
                        Index t1 = Adj_t[1][j[1]];
                        if (s1 == t1) {
                            lister->list_triangle(s, tt1, s1);
                        }
                        bool b1 = s1 <= t1;
                        bool b2 = s1 >= t1;
                        i[1] += b1;
                        j[1] += b2;
                    } else {
                        cnt++;
                    }
                    if (i[2] < Adj_s_size && j[2] < Adj_t_size[2]) {
                        Index s2 = Adj_s[i[2]];
                        Index t2 = Adj_t[2][j[2]];
                        if (s2 == t2) {
                            lister->list_triangle(s, tt2, s2);
                        }
                        bool b1 = s2 <= t2;
                        bool b2 = s2 >= t2;
                        i[2] += b1;
                        j[2] += b2;
                    } else {
                        cnt++;
                    }
                    if (i[3] < Adj_s_size && j[3] < Adj_t_size[3]) {
                        Index s3 = Adj_s[i[3]];
                        Index t3 = Adj_t[3][j[3]];
                        if (s3 == t3) {
                            lister->list_triangle(s, tt3, s3);
                        }
                        bool b1 = s3 <= t3;
                        bool b2 = s3 >= t3;
                        i[3] += b1;
                        j[3] += b2;
                    } else {
                        cnt++;
                    }
                } while (cnt < 4);

            }
        }

        // remainder
        for (; ti < Adj_s_size; ti++) {
            Index t = Adj_s[ti];

            Counter Adj_t_size = G->adjacency[(index_t) t].count;
            Index *Adj_t = G->adjacency[(index_t) t].neighbors;

            Counter i = 0, j = 0;
            // find the intersection of s's and t's neighbors
            while (i < Adj_s_size && j < Adj_t_size) {
                Index s0 = Adj_s[i];
                Index t0 = Adj_t[j];
                if (s0 == t0) {
                    if (t0 > t) {
                        lister->list_triangle(s, t, t0);
                    }
                    i++;
                    j++;
                } else if (s0 < t0) {
                    i++;
                } else {
                    j++;
                }
            }
        }
    }

}

}

#endif //TEAM02_EDGE_ITER_U5_H
