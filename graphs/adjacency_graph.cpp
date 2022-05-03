#include "adjacency_graph.h"
#include "instrumented_index.h"
#include <cstdio>
#include <cassert>
#include <stdexcept>
#include <string>

template<class Index>
AdjacencyGraph<Index> *create_graph_from_file(const char *filename) {
    auto *G = new AdjacencyGraph<Index>;

    FILE *f = fopen(filename, "r");
    if (f == nullptr) {
        throw std::invalid_argument("Could not open file " + std::string(filename) + "\n");
    }
    assert(f && "Invalid input: fail to open the file");
    int res;
    (void) res;

    index_t read_val;

    res = fscanf(f, INDEX_FMT, &read_val);
    if (res != 1) {
        throw std::invalid_argument("Invalid input: missing n");
    }
    G->n = read_val;
    G->adjacency = new AdjacencyList<Index>[G->n];

    for (index_t u = 0; u < G->n; u++) {
        AdjacencyList<Index> *adj = G->adjacency + u;
        res = fscanf(f, INDEX_FMT, &read_val);
        if (res != 1) {
            throw std::invalid_argument("Invalid input: missing node count");
        }
        adj->count = read_val;

        adj->neighbors = new Index[adj->count];

        for (index_t i = 0; i < adj->count; i++) {
            res = fscanf(f, INDEX_FMT, &read_val);
            if (res != 1) throw std::runtime_error("Invalid input: missing target node");
            *(adj->neighbors + i) = read_val;
            if (res != 1) {
                throw std::invalid_argument("Invalid input: missing target node");
            }
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

