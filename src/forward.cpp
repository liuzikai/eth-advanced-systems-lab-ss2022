//
// Created by Zikai Liu on 3/20/22.
//

#include "forward.h"
#include "quick_sort.h"
#include "instrumented_index.h"

template<class Index>
static void forward_reset_neighbor_container(AdjacencyGraph<Index> *G, ForwardNeighborContainer<Index> *A);

template<class Index>
ForwardNeighborContainer<Index> *forward_create_neighbor_container(AdjacencyGraph<Index> *G) {
    auto *A = new ForwardNeighborContainer<Index>;
    A->adjacency = new ForwardNeighbourList<Index>[G->n];
    for (Index u = 0; u < G->n; u++) {
        A->adjacency[u].neighbors = new Index[G->adjacency[u].count];
    }
    forward_reset_neighbor_container(G, A);
    return A;
}

template<class Index>
static void forward_reset_neighbor_container(AdjacencyGraph<Index> *G, ForwardNeighborContainer<Index> *A) {
    A->n = G->n;
    for (Index u = 0; u < G->n; u++) {
        A->adjacency[u].count = 0;
    }
}

template<class Index>
void forward_delete_neighbor_container(ForwardNeighborContainer<Index> *A) {
    for (Index u = 0; u < A->n; u++) {
        delete[] A->adjacency[u].neighbors;
    }
    delete[] A->adjacency;
    delete A;
}

template<class Index, class TRL>
TRL forward(AdjacencyGraph<Index> *G, ForwardNeighborContainer<Index> *A) {
    TRL lister;
    forward_reset_neighbor_container(G, A);

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
                Index i = 0, j = 0;
                ForwardNeighbourList<Index> *As = A->adjacency + s;
                ForwardNeighbourList<Index> *At = A->adjacency + t;
                while (i < As->count && j < At->count) {
                    if (As->neighbors[i] == At->neighbors[j]) {
                        lister.list_triangle(s, t, As->neighbors[i]);
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

    return lister;
}

template TriangleListing::Count forward<index_t, TriangleListing::Count>(AdjacencyGraph<index_t> *G, ForwardNeighborContainer<index_t> *A);
template TriangleListing::Count forward<InstrumentedIndex, TriangleListing::Count>(AdjacencyGraph<InstrumentedIndex> *G,
                                                                    ForwardNeighborContainer<InstrumentedIndex> *A);

template TriangleListing::Print forward<index_t, TriangleListing::Print>(AdjacencyGraph<index_t> *G, ForwardNeighborContainer<index_t> *A);
template TriangleListing::Print forward<InstrumentedIndex, TriangleListing::Print>(AdjacencyGraph<InstrumentedIndex> *G,
                                                                                   ForwardNeighborContainer<InstrumentedIndex> *A);

template TriangleListing::Collect forward<index_t, TriangleListing::Collect>(AdjacencyGraph<index_t> *G, ForwardNeighborContainer<index_t> *A);
template TriangleListing::Collect forward<InstrumentedIndex, TriangleListing::Collect>(AdjacencyGraph<InstrumentedIndex> *G,
                                                     ForwardNeighborContainer<InstrumentedIndex> *A);


template void forward_delete_neighbor_container(ForwardNeighborContainer<InstrumentedIndex> *A);
template void forward_delete_neighbor_container(ForwardNeighborContainer<index_t> *A);

template ForwardNeighborContainer<index_t> *forward_create_neighbor_container(AdjacencyGraph<index_t> *G);
template ForwardNeighborContainer<InstrumentedIndex> *forward_create_neighbor_container(AdjacencyGraph<InstrumentedIndex> *G);
