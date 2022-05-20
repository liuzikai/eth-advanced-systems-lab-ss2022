#ifndef TEAM02_EDGE_ITER_V2_H
#define TEAM02_EDGE_ITER_V2_H

#include "common.h"
#include "adjacency_graph.h"
#include "triangle_lister.h"
#include "quick_sort.h"

namespace ei2 {

template<class Index, class Counter = index_t, class TRL = TriangleListing::Count<Index>>
TRL edge_iterator(AdjacencyGraph<Index> *G, void *dummy = nullptr) {
    (void) dummy;
    Index s, t;
    //Counter i, j_lower_bound, j_upper_bound, j_current, j_inc, j_next;
    Counter i, j, j_lower_bound, j_inc, j_next;
    const AdjacencyList<Index> *t_adj, *s_adj, *swap = NULL;

    TRL lister;
    // According to sec. 4, the sorting is included in the execution time
    for (Counter u = 0; u < G->n; u++) {
        if (G->adjacency[u].count > 0) {
            quick_sort(G->adjacency[u].neighbors, 0, G->adjacency[u].count - 1);
        }
    }

    for (Counter si = 0; si < G->n; si++) {  // this should not count toward op count
        s = (Index) si;

        s_adj = &G->adjacency[(index_t) s];

        // iterate over all edges (neighbors) of node s
        for (Counter ti = 0; ti < s_adj->count; ti++) {
            t = s_adj->neighbors[ti];
            t_adj = &G->adjacency[(index_t) t];

            if (s < t) {
                if(t_adj->count < s_adj->count) {
                    swap = t_adj;
                    t_adj = s_adj;
                    s_adj = swap;
                }
                if(s_adj->count << 3 < t_adj->count) {
                    i = 0;
                    j_lower_bound = 0;
                    // find the intersection of s's and t's neighbors
                    // we will perform a binary search on the intersection
                    for(;i < s_adj->count; i++) {
                        //Perform an exponential search on t_adj->neighbors
                        j_inc = 1;
                        j_next = j_lower_bound + 2*j_inc;

                        while(j_next < t_adj->count && s_adj->neighbors[i] > t_adj->neighbors[j_next]) {
                            j_inc *= 2;
                            j_next = j_lower_bound + 2*j_inc;
                        }
                        j_lower_bound += (j_inc -1);
                        while(j_lower_bound < t_adj->count && s_adj->neighbors[i] > t_adj->neighbors[j_lower_bound]) {
                            j_lower_bound += 1;
                        }
                        if(s_adj->neighbors[i] == t_adj->neighbors[j_lower_bound] && t_adj->neighbors[j_lower_bound] > t) {
                            lister.list_triangle(s, t, t_adj->neighbors[j_lower_bound]);
                        }
                    }
                } else {
                    i = 0, j = 0;
                    while (i < s_adj->count && j < t_adj->count) {
                        if (s_adj->neighbors[i] == t_adj->neighbors[j] &&
                            t_adj->neighbors[j] > t) {
                            lister.list_triangle(s, t, t_adj->neighbors[j]);
                            i++;
                            j++;
                        } else if (s_adj->neighbors[i] < t_adj->neighbors[j]) {
                            i++;
                        } else {
                            j++;
                        }
                    }
                }
                //swap back if swapped
                if(swap != NULL) {
                    swap = t_adj;
                    t_adj = s_adj;
                    s_adj = swap;
                    swap = NULL;
                }
                /*i = 0;
                j_lower_bound = 0;
                // find the intersection of s's and t's neighbors
                // we will perform a binary search on the intersection
                for(;i < s_adj->count; i++) {
                    j_upper_bound = t_adj->count - 1;
                    if(s_adj->neighbors[i] < t_adj->neighbors[j_lower_bound])
                    while(j_lower_bound != j_upper_bound) {
                        j_current = (j_lower_bound + j_upper_bound) / 2;
                        if (s_adj->neighbors[i] < t_adj->neighbors[j_current]) {
                            j_upper_bound = j_current;
                        } else if (s_adj->neighbors[i] > t_adj->neighbors[j_current]) {
                            j_lower_bound = j_current + 1;
                        } else {
                            j_lower_bound = j_current;
                            break;
                        }
                    }
                    if(s_adj->neighbors[i] == t_adj->neighbors[j_lower_bound] && t_adj->neighbors[j_lower_bound] > t) {
                        lister.list_triangle(s, t, t_adj->neighbors[j_lower_bound]);
                    }
                }*/
                /*while (i < s_adj->count && j < t_adj->count) {
                    if (s_adj->neighbors[i] == t_adj->neighbors[j] &&
                        t_adj->neighbors[j] > t) {
                        lister.list_triangle(s, t, t_adj->neighbors[j]);
                        i++;
                        j++;
                    } else if (s_adj->neighbors[i] < t_adj->neighbors[j]) {
                        i++;
                    } else {
                        j++;
                    }
                }*/
            }
        }
    }

    return lister;
}

}

#endif //TEAM02_EDGE_ITER_V2_H
