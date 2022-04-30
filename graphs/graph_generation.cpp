#include <cstdlib>
#include <iostream>
#include <map>
#include <vector>
#include <ctime>
#include <fstream>
#include <string>
#include <algorithm>
#include <random>

#include "ArgParser.h"

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
    {GraphType::US_PATENTS, "us_patents"}};

void generate_random_graph(const GraphDefinition &graph_definition, std::ofstream &outfile)
{
    std::vector<std::vector<uint64_t>> adjacency_matrix(graph_definition.nodes, std::vector<uint64_t>(graph_definition.nodes, 0));
    uint64_t src, dst, edges_left = graph_definition.edges;
    while (edges_left > 0)
    {
        src = std::rand() % graph_definition.nodes;
        dst = std::rand() % graph_definition.nodes;
        if (src >= dst || adjacency_matrix[src][dst])
            continue;
        adjacency_matrix[src][dst] = 1;
        adjacency_matrix[src][src] += 1;
        adjacency_matrix[dst][src] = 1;
        adjacency_matrix[dst][dst] += 1;
        edges_left--;
    }
    outfile << graph_definition.nodes << std::endl;
    std::default_random_engine gen(graph_definition.random_seed);
    for (uint64_t i = 0; i < graph_definition.nodes; i++)
    {
        outfile << adjacency_matrix[i][i] << " ";
        //get all indecies of non-zero elements in row i of adjacency_matrix
        std::vector<uint64_t> non_zero_indecies;
        adjacency_matrix[i][i] = 0;
        for (uint64_t j = 0; j < graph_definition.nodes; j++)
        {
            if (adjacency_matrix[i][j]) {
                non_zero_indecies.push_back(j);
            }
        }

        //shuffle non_zero_indecies if shuffle_edges is true
        if (graph_definition.shuffle_edges) {
            std::shuffle(non_zero_indecies.begin(), non_zero_indecies.end(), gen);
        }

        for (uint64_t j = 0; j < non_zero_indecies.size(); j++)
        {
            outfile << non_zero_indecies[j] << " ";
        }
        outfile << std::endl;
    }
}

// Output Format:
// num_nodes
// for each node:
//  num_neighbours, neighbour 1, neighbour 2, etc.
void generate_graph(const GraphDefinition &graph_definition)
{
    //print the seed
    std::srand(graph_definition.random_seed);
    std::ofstream outfile(graph_definition.output_filename);

    switch (graph_definition.graphType)
    {
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
        switch (graph_definition.density)
        {
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

GraphDefinition parse_arguments(ArgParser &parser)
{
    GraphDefinition graph;

    graph.graphType = string_to_graphType(std::string{parser.getCmdOption("-gt").value_or("GENERATED")});
    graph.nodes = parser.getCmdOptionAsInt("-num_nodes").value_or(100);
    graph.edges = parser.getCmdOptionAsInt("-num_edges").value_or(1000);
    graph.density = string_to_density(std::string{parser.getCmdOption("-density").value_or("NONE")});
    graph.hdng = string_to_highDegreeNodeGeneration(std::string{parser.getCmdOption("-hdng").value_or("NONE")});
    graph.random_seed = parser.getCmdOptionAsInt("-seed").value_or(std::time(nullptr));
    graph.shuffle_edges = parser.cmdOptionExists("-shuffle_edges");
    graph.output_filename = parser.getCmdOption("-o").value_or("graph.txt");

    return graph;
}

int main(int argc, char *argv[])
{
    ArgParser parser(argc, argv);
    std::cout << "Generating graph..." << std::endl;
    GraphDefinition graph = parse_arguments(parser);
    generate_graph(graph);
    std::cout << "Graph generated." << std::endl;
}