//
// Created by Zikai Liu on 3/20/22.
//

#include "forward.h"
#include "quick_sort.h"

static void forward_reset_neighbor_container(adjacency_graph_t *G, forward_neighbor_container_t *A);

forward_neighbor_container_t *forward_create_neighbor_container(adjacency_graph_t *G) {
    auto *A = new forward_neighbor_container_t;
    A->adjacency = new forward_neighbour_list_t[G->n];
    for (index_t u = 0; u < G->n; u++) {
        A->adjacency[u].neighbors = new index_t[G->adjacency[u].count];
    }
    forward_reset_neighbor_container(G, A);
    return A;
}

static void forward_reset_neighbor_container(adjacency_graph_t *G, forward_neighbor_container_t *A) {
    A->n = G->n;
    for (index_t u = 0; u < G->n; u++) {
        A->adjacency[u].count = 0;
    }
}

void forward_delete_neighbor_container(forward_neighbor_container_t *A) {
    for (index_t u = 0; u < A->n; u++) {
        delete[] A->adjacency[u].neighbors;
    }
    delete[] A->adjacency;
    delete A;
}

index_t forward(adjacency_graph_t *G, forward_neighbor_container_t *A) {
    forward_reset_neighbor_container(G, A);

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

            if (s < t) {
                index_t i = 0, j = 0;
                forward_neighbour_list_t *As = A->adjacency + s;
                forward_neighbour_list_t *At = A->adjacency + t;
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
