#include <cstdlib>
#include <iostream>
#include <map>
#include <vector>
#include <ctime>
#include <fstream>
#include <string>
#include "graph_generation.h"
#include "../src/common.h"


const static std::map<GraphType, std::string> graph_to_file = {
    {GraphType::GERMAN_ROAD_NETWORK, "german_road_network"},
    {GraphType::ACTOR_MOVIE_GRAPH, "actor_movie_graph"},
    {GraphType::COMP_SCIENCE_AUTHORS, "comp_science_authors"},
    {GraphType::GOOGLE_CONTEST, "google_contest"},
    {GraphType::HELP_LITERATURE, "hep_literature"},
    {GraphType::ROUTER_NETWORK, "router_network"},
    {GraphType::WWW_NOTRE_DAME, "www_notre_dame"},
    {GraphType::US_PATENTS, "us_patents"}
};

void generate_random_graph(const GraphDefinition& graph_definition, std::ofstream &outfile) {
    std::vector<std::vector<index_t>> adjacency_matrix(graph_definition.nodes, std::vector<index_t>(graph_definition.nodes, 0));
    index_t src, dst, edges_left = graph_definition.edges;
    while(edges_left > 0) {
        src = std::rand() % graph_definition.nodes; 
        dst = std::rand() % graph_definition.nodes; 
        if(src >= dst || adjacency_matrix[src][dst])
            continue;
        adjacency_matrix[src][dst] = 1;
        adjacency_matrix[src][src] += 1;
        adjacency_matrix[dst][src] = 1;
        adjacency_matrix[dst][dst] += 1;
        edges_left--;
    }
    outfile << graph_definition.nodes << std::endl;
    for(index_t i = 0; i < graph_definition.nodes; i++) {
        outfile << adjacency_matrix[i][i] << " ";
        adjacency_matrix[i][i] = 0;
        for(index_t j = 0; j < graph_definition.nodes; j++) {
            if(adjacency_matrix[i][j]) {
                outfile << adjacency_matrix[i][j] << " ";
            }
        }
        outfile << std::endl;
    }
}


// Output Format:
// num_nodes
// for each node:
//  num_neighbours, neighbour 1, neighbour 2, etc.
void generate_graph(const GraphDefinition& graph_definition) {
    std::srand(graph_definition.random_seed);
    std::ofstream outfile(graph_definition.output_filename);
    
    switch(graph_definition.graphType) {
        case GraphType::GERMAN_ROAD_NETWORK:
            break;
        case GraphType::ACTOR_MOVIE_GRAPH:
            break;
        case GraphType::COMP_SCIENCE_AUTHORS:
            break;
        case GraphType::GOOGLE_CONTEST:
            break;
        case GraphType::HELP_LITERATURE:
            break;
        case GraphType::ROUTER_NETWORK:
            break;
        case GraphType::WWW_NOTRE_DAME:
            break;
        case GraphType::US_PATENTS:
            break;
        case GraphType::GENERATED:
            switch(graph_definition.density) {
                case Density::SPARSE:
                    break;
                case Density::DENSE:
                    break;
                case Density::NONE:
                    generate_random_graph(graph_definition, outfile);
                    break;
            }
            break;
    }
}
