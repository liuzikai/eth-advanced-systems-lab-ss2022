#include "adjacency_graph.h"
#include "../src/instrumented_index.h"
#include <cstdio>
#include <cassert>

template<class Index>
AdjacencyGraph<Index> *create_graph_from_file(const char *filename) {
    auto *G = new AdjacencyGraph<Index>;

    FILE *f = fopen(filename, "r");
    assert(f && "Invalid input: fail to open the file");
    int res;

    index_t read_val;

    res = fscanf(f, INDEX_FMT, &read_val);
    G->n = read_val;
    assert(res == 1 && "Invalid input: missing n");
    G->adjacency = new AdjacencyList<Index>[G->n];

    for (index_t u = 0; u < G->n; u++) {
        AdjacencyList<Index> *adj = G->adjacency + u;
        res = fscanf(f, INDEX_FMT, &read_val);
        adj->count = read_val;
        assert(res == 1 && "Invalid input: missing node count");
        adj->neighbors = new Index[adj->count];

        for (index_t i = 0; i < adj->count; i++) {
            res = fscanf(f, INDEX_FMT, &read_val);
            *(adj->neighbors + i) = read_val;
            assert(res == 1 && "Invalid input: missing target node");
        }
    }

    return G;
}

template<class Index>
void free_graph(AdjacencyGraph<Index> *graph) {
    for (index_t u = 0; u < graph->n; u++) {
        delete[] graph->adjacency[u].neighbors;
    }
    delete[] graph->adjacency;
    delete graph;
}


template AdjacencyGraph<index_t> *create_graph_from_file(const char *filename);
template void free_graph(AdjacencyGraph<index_t> *graph);

template AdjacencyGraph<InstrumentedIndex> *create_graph_from_file(const char *filename);
template void free_graph(AdjacencyGraph<InstrumentedIndex> *graph);

