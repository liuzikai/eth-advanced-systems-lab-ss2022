#ifndef TEAM02_EDGE_ITER_H
#define TEAM02_EDGE_ITER_H

#include "common.h"
#include "adjacency_graph.h"
#include "triangle_lister.h"
#include "quick_sort.h"

template<class Index, class Counter = index_t>
void *edge_iterator_get_dummy_helper(const AdjacencyGraph<Index> *) {
    return nullptr;
}

template<class Index, class Counter = index_t>
void edge_iterator_free_dummy_helper(void *) {}

template<class Index, class Counter = index_t, class TRL = TriangleListing::Count<Index>>
TRL edge_iterator(AdjacencyGraph<Index> *G, void *dummy = nullptr) {
    (void) dummy;

    TRL lister;
    // According to sec. 4, the sorting is included in the execution time
    for (Counter u = 0; u < G->n; u++) {
        if (G->adjacency[u].count > 0) {
            quick_sort(G->adjacency[u].neighbors, 0, G->adjacency[u].count - 1);
        }
    }

    for (Counter si = 0; si < G->n; si++) {  // this should not count toward op count
        Index s = (Index) si;


        const AdjacencyList<Index> *s_adj = &G->adjacency[(index_t) s];

        // iterate over all edges (neighbors) of node s
        for (Counter ti = 0; ti < s_adj->count; ti++) {
            Index t = s_adj->neighbors[ti];
            const AdjacencyList<Index> *t_adj = &G->adjacency[(index_t) t];

            if (s < t) {
                Counter i = 0, j = 0;
                // find the intersection of s's and t's neighbors
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
        }
    }

    return lister;
}

#endif //TEAM02_EDGE_ITER_H
