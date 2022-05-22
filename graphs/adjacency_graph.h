#ifndef TEAM02_ADJACENCY_GRAPH_H
#define TEAM02_ADJACENCY_GRAPH_H

#include "common.h"

template<class Index, class Counter = index_t>
struct AdjacencyList {
    Counter count;
    Counter orig_count;  // original node count
    Index *neighbors;
};

template<class Index, class Counter = index_t>
struct AdjacencyGraph {
    Counter n;  // node count
    AdjacencyList<Index> *adjacency;
};

/**
 * Create an adjacency graph from a file.
 * @param filename  The input file. See README for input file format.
 * @return          The graph.
 */
template<class Index, class Counter = index_t>
AdjacencyGraph<Index> *create_graph_from_file(const char *filename);

/**
 * Create an adjacency graph copy (with memory allocation).
 * @param filename  The input file. See README for input file format.
 * @return          The graph.
 */
template<class Index, class Counter = index_t>
AdjacencyGraph<Index> *create_graph_copy(const AdjacencyGraph<Index> *graph);

/**
 * Copy the graph. Caller should ensure that dest has at least the same memory as src.
 * @param dest
 * @param src
 */
template<class Index, class Counter = index_t>
void copy_graph(AdjacencyGraph<Index> *dest, const AdjacencyGraph<Index> *src);

/**
 * Free an adjacency graph, including all its adjacency lists.
 * @param graph
 */
template<class Index, class Counter = index_t>
void free_graph(const AdjacencyGraph<Index> *graph);

#endif //TEAM02_ADJACENCY_GRAPH_H
