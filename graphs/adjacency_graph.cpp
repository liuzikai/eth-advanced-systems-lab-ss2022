#include "adjacency_graph.h"
#include "instrumented_index.h"
#include <cstdio>
#include <cassert>
#include <stdexcept>
#include <string>

template<class Index, class Counter>
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

    for (Counter u = 0; u < G->n; u++) {
        AdjacencyList<Index> *adj = G->adjacency + u;
        res = fscanf(f, INDEX_FMT, &read_val);
        if (res != 1) {
            throw std::invalid_argument("Invalid input: missing node count");
        }
        adj->count = read_val;
        adj->orig_count = read_val;

        index_t rounded_up_size = roundUp(adj->count, 32);
        adj->neighbors = new(std::align_val_t(32)) Index[rounded_up_size];

        Counter i;
        for (i = 0; i < adj->count; i++) {
            res = fscanf(f, INDEX_FMT, &read_val);
            if (res != 1) throw std::runtime_error("Invalid input: missing target node");
            *(adj->neighbors + i) = (Index) read_val;
        }
        for (; i < rounded_up_size; i++) {
            *(adj->neighbors + i) = (Index) -1;
        }
        
    }

    return G;
}

template<class Index, class Counter>
AdjacencyGraph<Index> *create_graph_copy(const AdjacencyGraph<Index> *graph) {
    auto *G = new AdjacencyGraph<Index>;

    G->n = graph->n;
    G->adjacency = new AdjacencyList<Index>[G->n];

    for (Counter u = 0; u < G->n; u++) {
        AdjacencyList<Index> *adj = G->adjacency + u;

        adj->count = graph->adjacency[u].count;
        adj->orig_count = graph->adjacency[u].orig_count;
        index_t rounded_up_size = roundUp(adj->orig_count, 32);
        adj->neighbors = new(std::align_val_t(32)) Index[rounded_up_size];

        Counter i;
        for (i = 0; i < adj->orig_count; i++) {
            *(adj->neighbors + i) = (Index) graph->adjacency[u].neighbors[i];
        }
        for (; i < rounded_up_size; i++) {
            *(adj->neighbors + i) = (Index) -1;
        }
    }

    return G;
}

template<class Index, class Counter>
void copy_graph(AdjacencyGraph<Index> *dest, const AdjacencyGraph<Index> *src) {
    dest->n = src->n;
    for (Counter u = 0; u < src->n; u++) {
        dest->adjacency[u].count = src->adjacency[u].count;
        dest->adjacency[u].orig_count = src->adjacency[u].orig_count;
        for (Counter i = 0; i < src->adjacency[u].orig_count; i++) {
            dest->adjacency[u].neighbors[i] = src->adjacency[u].neighbors[i];
        }
    }
}

template<class Index, class Counter>
void free_graph(const AdjacencyGraph<Index> *graph) {
    for (Counter u = 0; u < graph->n; u++) {
        delete[] graph->adjacency[u].neighbors;
    }
    delete[] graph->adjacency;
    delete graph;
}


template AdjacencyGraph<index_t> *create_graph_from_file(const char *filename);

template AdjacencyGraph<index_t> *create_graph_copy(const AdjacencyGraph<index_t> *graph);

template void copy_graph(AdjacencyGraph<index_t> *dest, const AdjacencyGraph<index_t> *src);

template void free_graph(const AdjacencyGraph<index_t> *graph);

#ifndef NO_INSTRUMENTATION

template AdjacencyGraph<InstrumentedIndex> *create_graph_from_file(const char *filename);

template AdjacencyGraph<InstrumentedIndex> *create_graph_copy(const AdjacencyGraph<InstrumentedIndex> *graph);

template void copy_graph(AdjacencyGraph<InstrumentedIndex> *dest, const AdjacencyGraph<InstrumentedIndex> *src);

template void free_graph(const AdjacencyGraph<InstrumentedIndex> *graph);

#endif
