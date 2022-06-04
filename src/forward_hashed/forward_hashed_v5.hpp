#ifndef TEAM02_FORWARD_HASHED_V5_H
#define TEAM02_FORWARD_HASHED_V5_H

#include "common.h"
#include "adjacency_graph.h"
#include "triangle_lister.h"
#include "hash_table_v5.hpp"
#include "quick_sort.h"

namespace fh5 {

/// Accessory container

template<class Index, class Counter = index_t>
struct ForwardHashedHelper {
    Counter n;            // node count
    HashTable<Index> *A;  // array of hash tables of size n
};

template<class Index, class Counter = index_t>
ForwardHashedHelper<Index> *forward_hashed_create_neighbor_container(const AdjacencyGraph<Index> *G) {
    auto *A = new ForwardHashedHelper<Index>;
    A->n = G->n;
    A->A = new(std::align_val_t(HASH_ITEM_SIZE * 4)) HashTable<Index>[G->n];
    for (Counter u = 0; u < A->n; u++) {
        hashtable_init(&A->A[u]);
    }
    return A;
}

template<class Index, class Counter = index_t>
static void forward_hashed_reset_neighbor_container(AdjacencyGraph<Index> *G, ForwardHashedHelper<Index> *A) {
    (void) G;
    for (Counter u = 0; u < A->n; u++) {
        hashtable_clear(&A->A[u]);
    }
}

template<class Index, class Counter = index_t>
void forward_hashed_delete_neighbor_container(ForwardHashedHelper<Index> *A) {
    for (Counter u = 0; u < A->n; u++) {
        hashtable_clear(&A->A[u]);
    }
    delete[] A->A;
    delete A;
}

/**
 * List triangles using the forward-hashed algorithm.
 * @param G  The input graph. Notice that the graph WILL be changed (sorted). Make a copy before calling the function.
 * @param A  The accessory head.
 * @return   The number of triangles.
 * @note     Triangles are printed using print_triangle if it is defined.
 */
template<class Index, class Counter = index_t, class TRL = TriangleListing::Count<Index>>
void forward_hashed(TRL *lister, AdjacencyGraph<Index> *G, ForwardHashedHelper<Index> *A) {

    forward_hashed_reset_neighbor_container(G, A);

    for (Counter si = 0; si < G->n; si++) {  // this should not count toward op count
        Index s = (Index) si;

        for (Counter ti = 0; ti < G->adjacency[(index_t) s].count; ti++) {
            Index t = G->adjacency[(index_t) s].neighbors[ti];

            if (s < t) {

                const HashTable<Index> *s_table = &A->A[(index_t) s];
                HashTable<Index> *t_table = &A->A[(index_t) t];

                // set intersection: use the smaller hash table to probe the larger one.
                const HashTable<Index> *probe;
                const HashTable<Index> *build;
                if (s_table->count < t_table->count) {
                    probe = s_table;
                    build = t_table;
                } else {
                    probe = t_table;
                    build = s_table;
                }

                for (Counter i = 0; i < HASH_CONTAINER_SIZE; i++) {
                    const HashItem<Index> *head = &probe->head[i];
                    do {
                        bool in[HASH_ITEM_SIZE] = {false, false, false, false};
                        for (Counter j = 0; j < HASH_ITEM_SIZE; j++) {
                            if (head->number[j] != HASH_NULL_NUMBER) {
                                in[j] = hashtable_lookup(build, head->number[j]);
                            } else {
//                                in[j] = false;
                                break;
                            }
                        }

                        for (Counter j = 0; j < HASH_ITEM_SIZE; j++) {
                            if (in[j]) {
                                lister->list_triangle(s, t, head->number[j]);
                            }
                        }
                        head = head->next;
                    } while (head);

                }

                hashtable_insert(t_table, s);
            }
        }
    }


}

}

#endif
