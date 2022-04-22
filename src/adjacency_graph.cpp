#include "adjacency_graph.h"
#include <cstdio>
#include <cassert>

adjacency_graph_t *create_graph_from_file(const char *filename) {
    auto *G = new adjacency_graph_t;

    FILE *f = fopen(filename, "r");
    assert(f && "Invalid input: fail to open the file");
    int res;

    res = fscanf(f, INDEX_FMT, &G->n);
    assert(res == 1 && "Invalid input: missing n");
    G->adjacency = new adjacency_list_t[G->n];

    for (index_t u = 0; u < G->n; u++) {
        adjacency_list_t *adj = G->adjacency + u;
        res = fscanf(f, INDEX_FMT, &adj->count);
        assert(res == 1 && "Invalid input: missing node count");
        adj->neighbors = new index_t[adj->count];

        for (index_t i = 0; i < adj->count; i++) {
            res = fscanf(f, INDEX_FMT, adj->neighbors + i);
            assert(res == 1 && "Invalid input: missing target node");
        }
    }

    return G;
}

void free_graph(adjacency_graph_t *graph) {
    for (index_t u = 0; u < graph->n; u++) {
        delete[] graph->adjacency[u].neighbors;
    }
    delete[] graph->adjacency;
    delete graph;
}
