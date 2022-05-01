#include <cassert>
#include "forward_hashed.h"
#include "quick_sort.h"
#include "instrumented_index.h"

template<class Index>
static void forward_hashed_reset_neighbor_container(AdjacencyGraph<Index> *G, ForwardHashedNeighborContainer<Index> *A);

template<class Index>
ForwardHashedNeighborContainer<Index>  *forward_hashed_create_neighbor_container(AdjacencyGraph<Index> *G) {
    auto *A = new ForwardHashedNeighborContainer<Index>;
    A->adjacency = new ForwardHashedNeighborList<Index>[G->n];
    for (Index u = 0; u < G->n; u++) {
        A->adjacency[u].neighbors = create_hashtable<Index>();
    }
    forward_hashed_reset_neighbor_container(G, A);
    return A;
}

template<class Index>
static void forward_hashed_reset_neighbor_container(AdjacencyGraph<Index> *G, ForwardHashedNeighborContainer<Index>  *A) {
    A->n = G->n;
    for (Index u = 0; u < G->n; u++) {
        ForwardHashedNeighborList<Index> &a = A->adjacency[u];
        a.count = 0;
        hashtable_clear(a.neighbors);
    }
}

template<class Index>
void forward_hashed_delete_neighbor_container(ForwardHashedNeighborContainer<Index>  *A) {
    for (Index u = 0; u < A->n; u++) {
        free_hashtable(A->adjacency[u].neighbors);
    }
    delete[] A->adjacency;
    delete A;
}

template<class Index, class TRL>
TRL forward_hashed(AdjacencyGraph<Index> *G, ForwardHashedNeighborContainer<Index>  *A) {
    TRL lister;
    forward_hashed_reset_neighbor_container(G, A);

    // According to sec. 4, the sorting is included in the execution time
    for (Index u = 0; u < G->n; u++) {
        if (G->adjacency[u].count > 0) {
            quick_sort(G->adjacency[u].neighbors, Index(0), G->adjacency[u].count - 1);
        }
    }

    for (Index s = 0; s < G->n; s++) {
        for (Index ti = 0; ti < G->adjacency[s].count; ti++) {
            Index t = G->adjacency[s].neighbors[ti];

            if (s < t) {

                ForwardHashedNeighborList<Index> *As = A->adjacency + s;
                ForwardHashedNeighborList<Index> *At = A->adjacency + t;
                // set intersection: use the smaller hash table to probe the larger one.
                HashTable<Index> *probe = At->neighbors;
                HashTable<Index> *build = As->neighbors;
                if (As->count < At->count) {
                    probe = As->neighbors;
                    build = At->neighbors;
                }
                for (Index i = 0; i < probe->size; i++) {
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


template TriangleListing::Count forward_hashed<InstrumentedIndex, TriangleListing::Count>(AdjacencyGraph<InstrumentedIndex> *G, ForwardHashedNeighborContainer<InstrumentedIndex>  *A);
template TriangleListing::Count forward_hashed<index_t, TriangleListing::Count>(AdjacencyGraph<index_t> *G, ForwardHashedNeighborContainer<index_t>  *A);

template TriangleListing::Print forward_hashed<InstrumentedIndex, TriangleListing::Print>(AdjacencyGraph<InstrumentedIndex> *G, ForwardHashedNeighborContainer<InstrumentedIndex>  *A);
template TriangleListing::Print forward_hashed<index_t, TriangleListing::Print>(AdjacencyGraph<index_t> *G, ForwardHashedNeighborContainer<index_t>  *A);

template TriangleListing::Collect forward_hashed<InstrumentedIndex, TriangleListing::Collect>(AdjacencyGraph<InstrumentedIndex> *G, ForwardHashedNeighborContainer<InstrumentedIndex>  *A);
template TriangleListing::Collect forward_hashed<index_t, TriangleListing::Collect>(AdjacencyGraph<index_t> *G, ForwardHashedNeighborContainer<index_t>  *A);

template void forward_hashed_delete_neighbor_container(ForwardHashedNeighborContainer<index_t> *A);
template void forward_hashed_delete_neighbor_container(ForwardHashedNeighborContainer<InstrumentedIndex> *A);

template ForwardHashedNeighborContainer<index_t> *forward_hashed_create_neighbor_container(AdjacencyGraph<index_t> *G);
template ForwardHashedNeighborContainer<InstrumentedIndex> *forward_hashed_create_neighbor_container(AdjacencyGraph<InstrumentedIndex> *G);
