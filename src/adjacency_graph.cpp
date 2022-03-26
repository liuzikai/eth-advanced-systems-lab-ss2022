#include "adjacency_graph.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

adjacency_graph_t *create_graph_from_file(const char *filename, adjacency_graph_t **A) {
    adjacency_graph_t *graph = static_cast<adjacency_graph_t*>(malloc(sizeof(*graph)));
    if (A) {
        *A = static_cast<adjacency_graph_t*>(malloc(sizeof(*graph)));
    }

    FILE *f = fopen(filename, "r");
    assert(f && "Invalid input: fail to open the file");
    int res;

    res = fscanf(f, INDEX_FMT, &graph->n);
    assert(res == 1 && "Invalid input: missing n");
    graph->adjacency = static_cast<adjacency_list_t*>(malloc(sizeof(*graph->adjacency) * graph->n));
    if (A) {
        (*A)->n = graph->n;
        (*A)->adjacency = static_cast<adjacency_list_t*>(malloc(sizeof(*((*A)->adjacency)) * graph->n));
    }

    for (index_t u = 0; u < graph->n; u++) {
        adjacency_list_t *adj = graph->adjacency + u;
        res = fscanf(f, INDEX_FMT, &adj->count);
        assert(res == 1 && "Invalid input: missing node count");
        adj->neighbors = static_cast<index_t*>(malloc(sizeof(*adj->neighbors) * adj->count));
        if (A) {
            adjacency_list_t *a = (*A)->adjacency + u;
            a->count = 0;
            a->neighbors = static_cast<index_t*>(malloc(sizeof(*adj->neighbors) * adj->count));
        }

        for (index_t i = 0; i < adj->count; i++) {
            res = fscanf(f, INDEX_FMT, adj->neighbors + i);
            assert(res == 1 && "Invalid input: missing target node");
            assert(res == 1 && "Invalid input: u -> v, u should be less than v");
        }
    }

    return graph;
}

void free_graph(adjacency_graph_t *graph) {
    for (index_t u = 0; u < graph->n; u++) {
        free(graph->adjacency[u].neighbors);
    }
    free(graph->adjacency);
    free(graph);
}
