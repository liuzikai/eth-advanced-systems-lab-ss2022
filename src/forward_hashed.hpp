#ifndef TEAM02_FORWARD_HASHED_H
#define TEAM02_FORWARD_HASHED_H

#include "common.h"
#include "adjacency_graph.h"
#include "triangle_lister.h"
#include "hash_table.hpp"
#include "quick_sort.h"

/// Accessory container

template<class Index, class Counter = index_t>
struct ForwardHashedNeighborList {
    Counter count;
    HashTable<Index> *neighbors;
};

template<class Index, class Counter = index_t>
struct ForwardHashedNeighborContainer{
    Counter n;  // node count
    ForwardHashedNeighborList<Index> *adjacency;
};

template<class Index, class Counter = index_t>
ForwardHashedNeighborContainer<Index>  *forward_hashed_create_neighbor_container(const AdjacencyGraph<Index> *G) {
    auto *A = new ForwardHashedNeighborContainer<Index>;
    A->adjacency = new ForwardHashedNeighborList<Index>[G->n];
    for (Counter u = 0; u < G->n; u++) {
        A->adjacency[u].neighbors = create_hashtable<Index>();
    }
    return A;
}

template<class Index, class Counter = index_t>
static void forward_hashed_reset_neighbor_container(AdjacencyGraph<Index> *G, ForwardHashedNeighborContainer<Index>  *A) {
    A->n = G->n;
    for (Counter u = 0; u < G->n; u++) {
        ForwardHashedNeighborList<Index> &a = A->adjacency[u];
        a.count = 0;
        hashtable_clear(a.neighbors);
    }
}

template<class Index, class Counter = index_t>
void forward_hashed_delete_neighbor_container(ForwardHashedNeighborContainer<Index>  *A) {
    for (Counter u = 0; u < A->n; u++) {
        free_hashtable(A->adjacency[u].neighbors);
    }
    delete[] A->adjacency;
    delete A;
}

/**
 * List triangles using the forward-hashed algorithm.
 * @param G  The input graph. Notice that the graph WILL be changed (sorted). Make a copy before calling the function.
 * @param A  The accessory container.
 * @return   The number of triangles.
 * @note     Triangles are printed using print_triangle if it is defined.
 */
template<class Index, class Counter = index_t, class TRL = TriangleListing::Count<Index>>
TRL forward_hashed(AdjacencyGraph<Index> *G, ForwardHashedNeighborContainer<Index>  *A) {
    TRL lister;
    forward_hashed_reset_neighbor_container(G, A);

    for (Counter si = 0; si < G->n; si++) {  // this should not count toward op count
        Index s = (Index) si;

        for (Counter ti = 0; ti < G->adjacency[(index_t) s].count; ti++) {
            Index t = G->adjacency[(index_t) s].neighbors[ti];

            if (s < t) {

                ForwardHashedNeighborList<Index> *As = &A->adjacency[(index_t) s];
                ForwardHashedNeighborList<Index> *At = &A->adjacency[(index_t) t];
                // set intersection: use the smaller hash table to probe the larger one.
                HashTable<Index> *probe = At->neighbors;
                HashTable<Index> *build = As->neighbors;
                if (As->count < At->count) {
                    probe = As->neighbors;
                    build = At->neighbors;
                }
                for (Counter i = 0; i < probe->size; i++) {
                    HashItem<Index> *head = probe->container[i];
                    while (head) {
                        if (hashtable_lookup(build, head->number)) {
                            lister.list_triangle(s, t, head->number);
                        }
                        head = head->next;
                    }
                }

                hashtable_insert(At->neighbors, s);
            }
        }
    }

    return lister;
}

#endif //TEAM02_FORWARD_HASHED_H
