#include <cassert>
#include "forward_hashed.h"
#include "quick_sort.h"

static void forward_hashed_reset_neighbor_container(adjacency_graph_t *G, forward_hashed_neighbor_container_t *A);

forward_hashed_neighbor_container_t *forward_hashed_create_neighbor_container(adjacency_graph_t *G) {
    auto *A = new forward_hashed_neighbor_container_t;
    A->adjacency = new forward_hashed_neighbor_list_t[G->n];
    for (index_t u = 0; u < G->n; u++) {
        A->adjacency[u].neighbors = create_hashtable();
    }
    forward_hashed_reset_neighbor_container(G, A);
    return A;
}

static void forward_hashed_reset_neighbor_container(adjacency_graph_t *G, forward_hashed_neighbor_container_t *A) {
    A->n = G->n;
    for (index_t u = 0; u < G->n; u++) {
        forward_hashed_neighbor_list_t &a = A->adjacency[u];
        a.count = 0;
        hashtable_clear(a.neighbors);
    }
}

void forward_hashed_delete_neighbor_container(forward_hashed_neighbor_container_t *A) {
    for (index_t u = 0; u < A->n; u++) {
        free_hashtable(A->adjacency[u].neighbors);
    }
    delete[] A->adjacency;
    delete A;
}

index_t forward_hashed(adjacency_graph_t *G, forward_hashed_neighbor_container_t *A) {
    forward_hashed_reset_neighbor_container(G, A);

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

                forward_hashed_neighbor_list_t *As = A->adjacency + s;
                forward_hashed_neighbor_list_t *At = A->adjacency + t;
                // set intersection: use the smaller hash table to probe the larger one.
                hash_table_t *probe = At->neighbors;
                hash_table_t *build = As->neighbors;
                if (As->count < At->count) {
                    probe = As->neighbors;
                    build = At->neighbors;
                }
                for (index_t i = 0; i < probe->size; i++) {
                    hash_item_t *head = probe->container[i];
                    while (head) {
                        if (hashtable_lookup(build, head->number)) {
                            count++;
#if defined(print_triangle)
                            print_triangle(s, t, head->number);
#endif
                        }
                        head = head->next;
                    }
                }

                hashtable_insert(At->neighbors, s);
            }
        }
    }

    return count;
}
