#ifndef TEAM02_EDGE_ITER_M1_H
#define TEAM02_EDGE_ITER_M1_H

#include "common.h"
#include "adjacency_graph.h"
#include "triangle_lister.h"
#include "quick_sort.h"

// merge u5 with v2
namespace em1 {

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
                // Index tt4 = Adj_s[ti + 4];
                // Index tt5 = Adj_s[ti + 5];
                // Index tt6 = Adj_s[ti + 6];
                // Index tt7 = Adj_s[ti + 7];
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
                    if (i[0]+1 < Adj_s_size && j[0]+1 < Adj_t_size[0]) {
                        // s00 == t01: advance j
                        // s01 == t00: advance i
                        // s01 >= t01: advance j
                        // s01 <= t01: advance i
                        Index s00 = Adj_s[i[0]], s01 = Adj_s[i[0]+1];
                        Index t00 = Adj_t[0][j[0]], t01 = Adj_t[0][j[0]+1];
                        if (s00 == t00 || s00 == t01) {
                            lister->list_triangle(s, tt0, s00);
                        }
                        if (s01 == t00 || s01 == t01) {
                            lister->list_triangle(s, tt0, s01);
                        }
                        bool b1 = (s01 <= t01 || s01 == t00);
                        bool b2 = (s01 >= t01 || s00 == t01);
                        i[0] += (b1+b1);
                        j[0] += (b2+b2);
                    } else {
                        cnt++;
                    }
                    if (i[1]+1 < Adj_s_size && j[1]+1 < Adj_t_size[1]) {
                        Index s10 = Adj_s[i[1]], s11 = Adj_s[i[1]+1];
                        Index t10 = Adj_t[1][j[1]], t11 = Adj_t[1][j[1]+1];
                        if (s10 == t10 || s10 == t11) {
                            lister->list_triangle(s, tt1, s10);
                        }
                        if (s11 == t10 || s11 == t11) {
                            lister->list_triangle(s, tt1, s11);
                        }
                        bool b1 = (s11 <= t11 || s11 == t10);
                        bool b2 = (s11 >= t11 || s10 == t11);
                        i[1] += (b1+b1);
                        j[1] += (b2+b2);
                    } else {
                        cnt++;
                    }
                    if (i[2]+1 < Adj_s_size && j[2]+1 < Adj_t_size[2]) {
                        Index s20 = Adj_s[i[2]], s21 = Adj_s[i[2]+1];
                        Index t20 = Adj_t[2][j[2]], t21 = Adj_t[2][j[2]+1];
                        if (s20 == t20 || s20 == t21) {
                            lister->list_triangle(s, tt2, s20);
                        }
                        if (s21 == t20 || s21 == t21) {
                            lister->list_triangle(s, tt2, s21);
                        }
                        bool b1 = (s21 <= t21 || s21 == t20);
                        bool b2 = (s21 >= t21 || s20 == t21);
                        i[2] += (b1+b1);
                        j[2] += (b2+b2);
                    } else {
                        cnt++;
                    }
                    if (i[3]+1 < Adj_s_size && j[3]+1 < Adj_t_size[3]) {
                        Index s30 = Adj_s[i[3]], s31 = Adj_s[i[3]+1];
                        Index t30 = Adj_t[3][j[3]], t31 = Adj_t[3][j[3]+1];
                        if (s30 == t30 || s30 == t31) {
                            lister->list_triangle(s, tt3, s30);
                        }
                        if (s31 == t30 || s31 == t31) {
                            lister->list_triangle(s, tt3, s31);
                        }
                        bool b1 = (s31 <= t31 || s31 == t30);
                        bool b2 = (s31 >= t31 || s30 == t31);
                        i[3] += (b1+b1);
                        j[3] += (b2+b2);
                    } else {
                        cnt++;
                    }
                    // if (i[4]+1 < Adj_s_size && j[4]+1 < Adj_t_size[4]) {
                    //     // s00 == t01: advance j
                    //     // s01 == t00: advance i
                    //     // s01 >= t01: advance j
                    //     // s01 <= t01: advance i
                    //     Index s40 = Adj_s[i[4]], s41 = Adj_s[i[4]+1];
                    //     Index t40 = Adj_t[4][j[4]], t41 = Adj_t[4][j[4]+1];
                    //     if (s40 == t40 || s40 == t41) {
                    //         lister->list_triangle(s, tt4, s40);
                    //     }
                    //     if (s41 == t40 || s41 == t41) {
                    //         lister->list_triangle(s, tt4, s41);
                    //     }
                    //     bool b1 = (s41 <= t41 || s41 == t40);
                    //     bool b2 = (s41 >= t41 || s40 == t41);
                    //     i[4] += (b1+b1);
                    //     j[4] += (b2+b2);
                    // } else {
                    //     cnt++;
                    // }
                    // if (i[5]+1 < Adj_s_size && j[5]+1 < Adj_t_size[5]) {
                    //     Index s50 = Adj_s[i[5]], s51 = Adj_s[i[5]+1];
                    //     Index t50 = Adj_t[5][j[5]], t51 = Adj_t[5][j[5]+1];
                    //     if (s50 == t50 || s50 == t51) {
                    //         lister->list_triangle(s, tt5, s50);
                    //     }
                    //     if (s51 == t50 || s51 == t51) {
                    //         lister->list_triangle(s, tt5, s51);
                    //     }
                    //     bool b1 = (s51 <= t51 || s51 == t50);
                    //     bool b2 = (s51 >= t51 || s50 == t51);
                    //     i[5] += (b1+b1);
                    //     j[5] += (b2+b2);
                    // } else {
                    //     cnt++;
                    // }
                    // if (i[6]+1 < Adj_s_size && j[6]+1 < Adj_t_size[6]) {
                    //     Index s60 = Adj_s[i[6]], s61 = Adj_s[i[6]+1];
                    //     Index t60 = Adj_t[6][j[6]], t61 = Adj_t[6][j[6]+1];
                    //     if (s60 == t60 || s60 == t61) {
                    //         lister->list_triangle(s, tt6, s60);
                    //     }
                    //     if (s61 == t60 || s61 == t61) {
                    //         lister->list_triangle(s, tt6, s61);
                    //     }
                    //     bool b1 = (s61 <= t61 || s61 == t60);
                    //     bool b2 = (s61 >= t61 || s60 == t61);
                    //     i[6] += (b1+b1);
                    //     j[6] += (b2+b2);
                    // } else {
                    //     cnt++;
                    // }
                    // if (i[7]+1 < Adj_s_size && j[7]+1 < Adj_t_size[7]) {
                    //     Index s70 = Adj_s[i[7]], s71 = Adj_s[i[7]+1];
                    //     Index t70 = Adj_t[7][j[7]], t71 = Adj_t[7][j[7]+1];
                    //     if (s70 == t70 || s70 == t71) {
                    //         lister->list_triangle(s, tt7, s70);
                    //     }
                    //     if (s71 == t70 || s71 == t71) {
                    //         lister->list_triangle(s, tt7, s71);
                    //     }
                    //     bool b1 = (s71 <= t71 || s71 == t70);
                    //     bool b2 = (s71 >= t71 || s70 == t71);
                    //     i[7] += (b1+b1);
                    //     j[7] += (b2+b2);
                    // } else {
                    //     cnt++;
                    // }
                } while (cnt < 4);

                // remainder
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
                    // if (i[4] < Adj_s_size && j[4] < Adj_t_size[4]) {
                    //     Index s4 = Adj_s[i[4]];
                    //     Index t4 = Adj_t[4][j[4]];
                    //     if (s4 == t4) {
                    //         lister->list_triangle(s, tt4, s4);
                    //     }
                    //     bool b1 = s4 <= t4;
                    //     bool b2 = s4 >= t4;
                    //     i[4] += b1;
                    //     j[4] += b2;
                    // } else {
                    //     cnt++;
                    // }
                    // if (i[5] < Adj_s_size && j[5] < Adj_t_size[5]) {
                    //     Index s5 = Adj_s[i[5]];
                    //     Index t5 = Adj_t[5][j[5]];
                    //     if (s5 == t5) {
                    //         lister->list_triangle(s, tt5, s5);
                    //     }
                    //     bool b1 = s5 <= t5;
                    //     bool b2 = s5 >= t5;
                    //     i[5] += b1;
                    //     j[5] += b2;
                    // } else {
                    //     cnt++;
                    // }
                    // if (i[6] < Adj_s_size && j[6] < Adj_t_size[6]) {
                    //     Index s6 = Adj_s[i[6]];
                    //     Index t6 = Adj_t[6][j[6]];
                    //     if (s6 == t6) {
                    //         lister->list_triangle(s, tt6, s6);
                    //     }
                    //     bool b1 = s6 <= t6;
                    //     bool b2 = s6 >= t6;
                    //     i[6] += b1;
                    //     j[6] += b2;
                    // } else {
                    //     cnt++;
                    // }
                    // if (i[7] < Adj_s_size && j[7] < Adj_t_size[7]) {
                    //     Index s7 = Adj_s[i[7]];
                    //     Index t7 = Adj_t[7][j[7]];
                    //     if (s7 == t7) {
                    //         lister->list_triangle(s, tt7, s7);
                    //     }
                    //     bool b1 = s7 <= t7;
                    //     bool b2 = s7 >= t7;
                    //     i[7] += b1;
                    //     j[7] += b2;
                    // } else {
                    //     cnt++;
                    // }
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

#endif //TEAM02_EDGE_ITER_M1_H
