#ifndef TEAM02_FORWARD_H
#define TEAM02_FORWARD_H

#include "common.h"
#include "adjacency_graph.h"

/**
 * List triangles using the forward algorithm.
 * @param G  The input graph. Notice that the graph WILL be changed (sorted). Make a copy before calling the function.
 * @param A  The accessory container. See create_graph_from_file().
 * @return   The number of triangles.
 * @note     Triangles are printed using print_triangle if it is defined.
 */

index_t forward(adjacency_graph_t *G, adjacency_list_t *A);

#endif //TEAM02_FORWARD_H
