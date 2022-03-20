//
// Created by Zikai Liu on 3/20/22.
//

#ifndef TEAM02_ADJACENCY_GRAPH_H
#define TEAM02_ADJACENCY_GRAPH_H

#include "common.h"

typedef struct {
    index_t count;
    index_t *neighbors;
} adjacency_list_t;

typedef struct {
    index_t n;  // node count
    adjacency_list_t *adjacency;
} adjacency_graph_t;

/**
 * Create an adjacency graph from a file.
 * @param filename  The input file. See README for input file format.
 * @param A         When not NULL, allocate an accessory graph (used by forward), which has the same space as the
 *                  return graph but adjacency.count is initialized to 0.
 * @return          The graph.
 */
adjacency_graph_t *create_graph_from_file(const char *filename, adjacency_graph_t **A);

/**
 * Free an adjacency graph, including all its adjacency lists.
 * @param graph
 */
void free_graph(adjacency_graph_t *graph);

#endif //TEAM02_ADJACENCY_GRAPH_H
