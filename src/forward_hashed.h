#ifndef TEAM02_FORWARD_HASHED_H
#define TEAM02_FORWARD_HASHED_H

#include "common.h"
#include "../graphs/adjacency_graph.h"
#include "hash_table.h"

/// Accessory container

template<class Index>
struct ForwardHashedNeighborList {
    Index count;
    HashTable<Index> *neighbors;
};

template<class Index>
struct ForwardHashedNeighborContainer{
    Index n;  // node count
    ForwardHashedNeighborList<Index> *adjacency;
};

template<class Index>
ForwardHashedNeighborContainer<Index> *forward_hashed_create_neighbor_container(AdjacencyGraph<Index> *G);

template<class Index>
void forward_hashed_delete_neighbor_container(ForwardHashedNeighborContainer<Index> *A);

/**
 * List triangles using the forward-hashed algorithm.
 * @param G  The input graph. Notice that the graph WILL be changed (sorted). Make a copy before calling the function.
 * @param A  The accessory container.
 * @return   The number of triangles.
 * @note     Triangles are printed using print_triangle if it is defined.
 */
template<class Index>
index_t forward_hashed(AdjacencyGraph<Index> *G, ForwardHashedNeighborContainer<Index> *A);

#endif //TEAM02_FORWARD_HASHED_H
