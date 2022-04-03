//
// Created by Zikai Liu on 3/20/22.
//

#include <assert.h>
#include "common.h"
#include "adjacency_graph.h"
#include "quick_sort.h"

index_t forward(adjacency_graph_t *G, adjacency_list_t *A) {
    // According to sec. 4, the sorting is included in the execution time
    for (index_t u = 0; u < G->n; u++) {
        if (G->adjacency[u].count > 0) {
            quick_sort(G->adjacency[u].neighbors, 0, G->adjacency[u].count - 1);
        }
    }

    index_t count = 0;
    for (index_t s = 0; s < G->n; s++) {
        for (index_t ti = 0; ti < G->adjacency[s].count; ti++) {
            index_t t = G->adjacency[s].neighbors[ti];

            if(s < t) {
                index_t i = 0, j = 0;
                adjacency_list_t *As = A + s;
                adjacency_list_t *At = A + t;
                while (i < As->count && j < At->count) {
                    if (As->neighbors[i] == At->neighbors[j]) {
                        count++;
#if defined(print_triangle)
                        print_triangle(s, t, As->neighbors[i]);
#endif
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

    return count;
}
