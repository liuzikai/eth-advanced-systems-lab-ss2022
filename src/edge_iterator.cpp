#include <assert.h>
#include "common.h"
#include "adjacency_graph.h"
#include "quick_sort.h"
#include <stdio.h>

void* edge_iterator_get_dummy_helper(adjacency_graph_t *) {
    return nullptr;
}

index_t edge_iterator(adjacency_graph_t *G, __attribute__((unused)) void* dummy = nullptr) {
    // According to sec. 4, the sorting is included in the execution time
    for (index_t u = 0; u < G->n; u++) {
        if (G->adjacency[u].count > 0) {
            quick_sort(G->adjacency[u].neighbors, 0, G->adjacency[u].count - 1);
        }
    }

    index_t count = 0;
    for (index_t s = 0; s < G->n; s++) {
        // iterate over all edges (neighbors) of node s
        for (index_t ti = 0; ti < G->adjacency[s].count; ti++) {
            index_t t = G->adjacency[s].neighbors[ti];

            if(s < t) {
                index_t i = 0, j = 0;
                // find the intersection of s's and t's neighbors
                while (i < G->adjacency[s].count && j < G->adjacency[t].count) {
                    if (G->adjacency[s].neighbors[i] == G->adjacency[t].neighbors[j] && G->adjacency[t].neighbors[j] > t) {
                        count++;
#if defined(print_triangle)
                        print_triangle(s, t, G->adjacency[s].neighbors[i]);
#endif
                        i++;
                        j++;
                    } else if (G->adjacency[s].neighbors[i] < G->adjacency[t].neighbors[j]) {
                        i++;
                    } else {
                        j++;
                    }
                }
            }
        }
    }

    return count;
}
