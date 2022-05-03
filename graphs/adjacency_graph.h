#ifndef TEAM02_ADJACENCY_GRAPH_H
#define TEAM02_ADJACENCY_GRAPH_H

#include "common.h"

template<class Index>
struct AdjacencyList {
    Index count;
    Index *neighbors;
};

template<class Index>
struct AdjacencyGraph {
    typedef Index index_type;
    Index n;  // node count
    AdjacencyList<Index> *adjacency;
};

/**
 * Create an adjacency graph from a file.
 * @param filename  The input file. See README for input file format.
 * @return          The graph.
 */
template<class Index>
AdjacencyGraph<Index> *create_graph_from_file(const char *filename);

/**
 * Free an adjacency graph, including all its adjacency lists.
 * @param graph
 */
template<class Index>
void free_graph(AdjacencyGraph<Index> *graph);

#endif //TEAM02_ADJACENCY_GRAPH_H
