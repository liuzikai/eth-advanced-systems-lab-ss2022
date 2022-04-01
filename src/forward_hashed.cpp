#include <assert.h>
#include "common.h"
#include "adjacency_graph.h"
#include "adjacency_graph_hashed.h"
#include "quick_sort.h"
#include "hash_table.h"

index_t forward_hashed(adjacency_graph_t *G, adjacency_list_hashed_t *A) {
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

            // adjacency_list_hashsed_t guaranteed s < t, the if statement is eliminated
            assert(s < t && "Forward_hashed: s < t is not satisfied from adjacency_list_hashed_t");
            // TODO: for all three algos
            // Add "if s<t" logic to support undirected graphs. Eg. input/sample_undirected.txt

            adjacency_list_hashed_t *As = A + s;
            adjacency_list_hashed_t *At = A + t;
            // set intersection: use the smaller hash table to probe the larger one.
            hash_table_t *probe = At->neighbors;
            hash_table_t *build = As->neighbors;
            if (As->count < At->count) {
                probe = As->neighbors;
                build = At->neighbors;
            }
            for (index_t i=0; i < probe->size; i++) {
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

    return count;
}
