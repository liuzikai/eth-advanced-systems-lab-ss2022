#include <assert.h>
#include "common.h"
#include "../graphs/adjacency_graph.h"
#include "quick_sort.h"
#include "instrumented_index.h"
#include <stdio.h>
#include "triangle_lister.h"


template<class Index>
void *edge_iterator_get_dummy_helper(AdjacencyGraph<Index> *) {
    return nullptr;
}

template<class Index, class TRL = TriangleListing::Count>
TRL edge_iterator(AdjacencyGraph<Index> *G, __attribute__((unused)) void *dummy = nullptr) {
    TRL lister;
    // According to sec. 4, the sorting is included in the execution time
    for (Index u = 0; u < G->n; u++) {
        if (G->adjacency[u].count > 0) {
            quick_sort(G->adjacency[u].neighbors, Index(0), G->adjacency[u].count - 1);
        }
    }

    for (Index s = 0; s < G->n; s++) {
        // iterate over all edges (neighbors) of node s
        for (Index ti = 0; ti < G->adjacency[s].count; ti++) {
            Index t = G->adjacency[s].neighbors[ti];

            if (s < t) {
                Index i = 0, j = 0;
                // find the intersection of s's and t's neighbors
                while (i < G->adjacency[s].count && j < G->adjacency[t].count) {
                    if (G->adjacency[s].neighbors[i] == G->adjacency[t].neighbors[j] &&
                        G->adjacency[t].neighbors[j] > t) {
                        lister.list_triangle(s, t, G->adjacency[t].neighbors[j]);
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

    return lister;
}

template TriangleListing::Count edge_iterator<index_t, TriangleListing::Count>(AdjacencyGraph<index_t> *G, void *);
template TriangleListing::Count edge_iterator<InstrumentedIndex, TriangleListing::Count>(AdjacencyGraph<InstrumentedIndex> *G, void *);

template TriangleListing::Print edge_iterator<index_t, TriangleListing::Print>(AdjacencyGraph<index_t> *G, void *);
template TriangleListing::Print edge_iterator<InstrumentedIndex, TriangleListing::Print>(AdjacencyGraph<InstrumentedIndex> *G, void *);

template TriangleListing::Collect edge_iterator<index_t, TriangleListing::Collect>(AdjacencyGraph<index_t> *G, void *);
template TriangleListing::Collect edge_iterator<InstrumentedIndex, TriangleListing::Collect>(AdjacencyGraph<InstrumentedIndex> *G, void *);

template void *edge_iterator_get_dummy_helper(AdjacencyGraph<index_t> *);
template void *edge_iterator_get_dummy_helper(AdjacencyGraph<InstrumentedIndex> *);
