#ifndef TEAM02_FORWARD_HASHED_H
#define TEAM02_FORWARD_HASHED_H

#include "common.h"
#include "adjacency_graph.h"
#include "hash_table.h"

/// Accessory container

typedef struct {
    index_t count;
    hash_table_t *neighbors;
} forward_hashed_neighbor_list_t;

typedef struct {
    index_t n;  // node count
    forward_hashed_neighbor_list_t *adjacency;
} forward_hashed_neighbor_container_t;

forward_hashed_neighbor_container_t *forward_hashed_create_neighbor_container(adjacency_graph_t *G);

void forward_hashed_delete_neighbor_container(forward_hashed_neighbor_container_t *A);

/**
 * List triangles using the forward-hashed algorithm.
 * @param G  The input graph. Notice that the graph WILL be changed (sorted). Make a copy before calling the function.
 * @param A  The accessory container.
 * @return   The number of triangles.
 * @note     Triangles are printed using print_triangle if it is defined.
 */
index_t forward_hashed(adjacency_graph_t *G, forward_hashed_neighbor_container_t *A);

#endif //TEAM02_FORWARD_HASHED_H
