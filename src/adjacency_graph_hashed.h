#ifndef TEAM02_ADJACENCY_GRAPH_HASHED_H
#define TEAM02_ADJACENCY_GRAPH_HASHED_H

#include "common.h"
#include "hash_table.h"

typedef struct {
    index_t count;
    hash_table_t *neighbors; // change this
} adjacency_list_hashed_t;

typedef struct {
    index_t n;  // node count
    adjacency_list_hashed_t *adjacency;
} adjacency_graph_hashed_t;


/**
 * Create an adjacency graph from a file.
 * @param filename  The input file. See README for input file format.
 * @param A         When not NULL, allocate an accessory graph (used by forward_hased), not populated. Hash-table-based adjacency lists, used for set intersection.
 * @return          The graph, array-based adjacency lists, populated with the input graph.
 */
adjacency_graph_t *create_graph_from_file_hashed(const char *filename, adjacency_graph_hashed_t **A);

/**
 * Free an hash-table based adjacency graph, including all its adjacency lists.
 * @param graph
 */
void free_graph_hashed(adjacency_graph_hashed_t *graph);

#endif //TEAM02_ADJACENCY_GRAPH_H
