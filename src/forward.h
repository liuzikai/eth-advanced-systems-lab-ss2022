#ifndef TEAM02_FORWARD_H
#define TEAM02_FORWARD_H

#include "common.h"
#include "adjacency_graph.h"

/// Accessory container (array A in the paper)

typedef struct {
    index_t count;
    index_t *neighbors;
} forward_neighbour_list_t;

typedef struct {
    index_t n;  // node count
    forward_neighbour_list_t *adjacency;
} forward_neighbor_container_t;

forward_neighbor_container_t *forward_create_neighbor_container(adjacency_graph_t *G);

void forward_delete_neighbor_container(forward_neighbor_container_t *A);

/**
 * List triangles using the forward algorithm.
 * @param G  The input graph. Notice that the graph WILL be changed (sorted). Make a copy before calling the function.
 * @param A  The accessory container.
 * @return   The number of triangles.
 * @note     Triangles are printed using print_triangle if it is defined.
 */
index_t forward(adjacency_graph_t *G, forward_neighbor_container_t *A);

#endif //TEAM02_FORWARD_H
