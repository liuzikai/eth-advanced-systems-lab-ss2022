#ifndef TEAM02_FORWARD_H
#define TEAM02_FORWARD_H

#include "common.h"
#include "adjacency_graph.h"

/// Accessory container (array A in the paper)
template<class Index>
struct ForwardNeighbourList {
    Index count;
    Index *neighbors;
};

template<class Index>
struct ForwardNeighborContainer {
    Index n;  // node count
    ForwardNeighbourList<Index> *adjacency;
};

template<class Index>
ForwardNeighborContainer<Index> *forward_create_neighbor_container(AdjacencyGraph<Index> *G);

template<class Index>
void forward_delete_neighbor_container(ForwardNeighborContainer<Index> *A);

/**
 * List triangles using the forward algorithm.
 * @param G  The input graph. Notice that the graph WILL be changed (sorted). Make a copy before calling the function.
 * @param A  The accessory container.
 * @return   The number of triangles.
 * @note     Triangles are printed using print_triangle if it is defined.
 */
template<class Index>
index_t forward(AdjacencyGraph<Index> *G, ForwardNeighborContainer<Index> *A);

#endif //TEAM02_FORWARD_H
