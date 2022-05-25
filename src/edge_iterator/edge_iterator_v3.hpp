#ifndef TEAM02_EDGE_ITER_V3_H
#define TEAM02_EDGE_ITER_V3_H

#include "common.h"
#include "adjacency_graph.h"
#include "triangle_lister.h"
#include "quick_sort.h"

namespace ei3 {

template<class Index, class Counter = index_t, class TRL = TriangleListing::Count<Index>>
void edge_iterator(TRL* lister,AdjacencyGraph<Index> *G, void *dummy = nullptr) {
    (void) dummy;
    Index s, t;
    //Counter i, j_lower_bound, j_upper_bound, j_current, j_inc, j_next;
    Counter i, j, j_lower_bound, j_inc, j_next, i_start, j_start;
    Counter highest_i, highest_j;
    Counter mid_i, mid_j;
    const AdjacencyList<Index> *t_adj, *s_adj, *swap = NULL;

    Index s_neighbor1, s_neighbor2;
    Index t_neighbor1, t_neighbor2;
    
    TRL lister;
    // According to sec. 4, the sorting is included in the execution time
    // for (Counter u = 0; u < G->n; u++) {
    //     if (G->adjacency[u].count > 0) {
    //         quick_sort(G->adjacency[u].neighbors, 0, G->adjacency[u].count - 1);
    //     }
    // }

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
                i_start = 0, j_start = 0;
                highest_i = s_adj->count - 1;
                highest_j = t_adj->count - 1;
                while(i_start != highest_i) {
                    mid_i = (i_start + highest_i) / 2;
                    if (s_adj->neighbors[mid_i] <= t) {
                        i_start = mid_i + 1;
                    } else {
                        highest_i = mid_i;
                    }
                }
                while(j_start != highest_j) {
                    mid_j = (j_start + highest_j) / 2;
                    if (t_adj->neighbors[mid_j] <= t) {
                        j_start = mid_j + 1;
                    } else {
                        highest_j = mid_j;
                    }
                }
                /*while(i_start < s_adj->count && s_adj->neighbors[i_start] < t) {
                    i_start++;    
                }
                while(j_start < t_adj->count && t_adj->neighbors[j_start] < t) {
                    j_start++;    
                }*/
                if(s_adj->count << 3 < t_adj->count) {
                    i = i_start;
                    j_lower_bound = j_start;
                    // find the intersection of s's and t's neighbors
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
                        if(s_adj->neighbors[i] == t_adj->neighbors[j_lower_bound]) {
                            lister.list_triangle(s, t, t_adj->neighbors[j_lower_bound]);
                        }
                    }
                } else {
                    i = i_start, j = j_start;
                    while (i < s_adj->count -1 && j < t_adj->count -1) {
                        s_neighbor1 = s_adj->neighbors[i];
                        s_neighbor2 = s_adj->neighbors[i+1];
                        t_neighbor1 = t_adj->neighbors[j];
                        t_neighbor2 = t_adj->neighbors[j+1];
                        if (s_neighbor1 == t_neighbor1) {
                            lister.list_triangle(s, t, s_neighbor1);
                        } else if (s_neighbor1 == t_neighbor2) {
                            lister.list_triangle(s, t, s_neighbor1);
                            goto advance_t;
                        } else if (s_neighbor2 == t_neighbor1) {
                            lister.list_triangle(s, t, s_neighbor2);
                            goto advance_s;
                        } 
                        if (s_neighbor2 == t_neighbor2) {
                            lister.list_triangle(s, t, s_neighbor2);
                            goto advance_st;
                        } else if (s_neighbor2 > t_neighbor2) {
                            goto advance_t;
                        } else {
                            goto advance_s;
                        }
                    advance_s:
                        i+=2;
                        continue;
                    advance_t:
                        j+=2;
                        continue;
                    advance_st:
                        i+=2;
                        j+=2;
                    }
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
            }
        }
    }

    return lister;
}

}

#endif //TEAM02_EDGE_ITER_V3_H