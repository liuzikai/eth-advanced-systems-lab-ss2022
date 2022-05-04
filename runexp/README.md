## How to use the exp pipeline
1. Modify env.sh
2. Run run.sh: -h for help

## Output directory
random graph: data/`commit_hash`-`graph_type`-`low_edge_count`-`high_edge_count`-`interval-node_degree`(-`seed`)/`num_warmups`-`num_runs`-`num_phases`/`number`/\
real-world graph: data/`commit_hash`-`graph_type`/`num_warmups`-`num_runs`-`num_phases`/`number`/\
There's a `config.json` file in each folder.\
`number`: the number of times this configuration is experimented with.\
`config.json` stores all the above parameters.\
Every time this configuration is executed, <number> increments by one.

## Random graph example
`./run.sh -a 5000 -b 10000 -i 200 -n 10 -s 1 -w 10 -r 10 -p 10`\
Run experiments with random graphs.\
The edge_count of the random graphs: range(5000,10000,200)\
The node_count of the random graphs: edge_count/10\
The node_count should be larger than sqrt(edge_count) (The current version of graph_generation doesn't halt if that is not satisfied. Should add a check for it somewhere.)\
Generate random graphs using seed=1\
10 warmups, 10 runs, 10 phases\
A plot will be generated and saved in the output directory at the end of the run.

## Real-world graph example
`./run.sh -g <graph_type> -w 10 -r 10 -p 10`\
Run experiments with graph <graph_type>\
10 warmups, 10 runs, 10 phases\

After all experiments are finished, plot the data.
performance/opcount/runtime vs. graph clustered bar graph.
`clustered_bar.sh -c <commit_hash> -w 10 -r 10 -p 10 -n 1`
`-n 1`: the first experiment of this configuration
