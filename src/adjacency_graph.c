//
// Created by Zikai Liu on 3/20/22.
//

#include "adjacency_graph.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

adjacency_graph_t *create_graph_from_file(const char *filename, adjacency_graph_t **A) {
    adjacency_graph_t *graph = malloc(sizeof(*graph));
    if (A) {
        *A = malloc(sizeof(*graph));
    }

    FILE *f = fopen(filename, "r");
    assert(f && "Invalid input: fail to open the file");
    int res;

    res = fscanf(f, INDEX_FMT, &graph->n);
    assert(res == 1 && "Invalid input: missing n");
    graph->adjacency = malloc(sizeof(*graph->adjacency) * graph->n);
    if (A) {
        (*A)->n = graph->n;
        (*A)->adjacency = malloc(sizeof(*((*A)->adjacency)) * graph->n);
    }

    for (index_t u = 0; u < graph->n; u++) {
        adjacency_list_t *adj = graph->adjacency + u;
        res = fscanf(f, INDEX_FMT, &adj->count);
        assert(res == 1 && "Invalid input: missing node count");
        adj->neighbors = malloc(sizeof(*adj->neighbors) * adj->count);
        if (A) {
            adjacency_list_t *a = (*A)->adjacency + u;
            a->count = 0;
            a->neighbors = malloc(sizeof(*adj->neighbors) * adj->count);
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
