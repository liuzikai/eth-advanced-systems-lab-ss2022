## How to plot roofline
`$ run_density.sh` as usual\
Fill in "qn.csv": data_transfer column\
deprecated `$ python3 roofline.py -d [path to dir of qn.csv] (-a [list algos you want to plot])`\
`python roofline.py -d [path\;path] -p [path] -a [algos] -m -v="-O3,-O3 -fno-strict-aliasing"`

## plot_n.py
same algo, same edge count, different degree\\
`python plot_n.py -d [path for degree4\;path degree 8\;path degree 16] -p ../data/compare -l 600 -r 20001 -i 500 -a fh_va,fh_v3,fh_v4 -b va -v "avg degree 4,avg degree 8,avg degree 16" -n 4,8,16 -s 1`

## plot_all.py
`python plot_all.py -d [paths] -p [path] -b [base version: eg. va] -v [version name of datadir1,version name of datadir2] -a [algos] -n [-n value]`\
deprecated `python plot_all.py -d [datadir1\;datadir2] -p [store plots here] -l 500 -r 6001 -i 500 -n 10 -s 1 -t -a [f_va,f_v6,f_u4,f_v5] -m -b va -v [version name of datadir1,version name of datadir2] -n [-n value]`

## How to use the exp pipeline
1. Modify env.sh
2. Run run.sh: -h for help

**!Does NOT support different sort/cut settings in one run!**
**!Does NOT support speedup plots for realworld graphs!**

## Output directory
random graph: data/`commit_hash`-`graph_type`-`low_edge_count`-`high_edge_count`-`interval-node_degree`(-`seed`)/`num_warmups`-`num_runs`-`num_phases`/`number`/\
real-world graph: data/`commit_hash`-`graph_type`/`num_warmups`-`num_runs`-`num_phases`/`number`/\
There's a `config.json` file in each folder.\
`number`: the number of times this configuration is experimented with.\
`config.json` stores all the above parameters.\
Every time this configuration is executed, [number] increments by one.


## Random graph example
`./run.sh -a 5000 -b 10000 -i 200 -n 10 -s 1 -w 10 -r 10 -p 10 -l ei_va,ei_v1`\
Run experiments with random graphs.\
Edge_count of the random graphs: range(5000,10000,200)\
Node_count of the random graphs: edge_count/10\
Node_count should be larger than sqrt(edge_count) (The current version of graph_generation doesn't halt if that is not satisfied. Should add a check for it somewhere.)\
Generate random graphs using seed=1\
10 warmups, 10 runs, 10 phases\
algorithms: ei_va, ei_v1\

`./run_density.sh -a 5000 -b 10000 -i 200 -n 10 -s 1 -w 10 -r 10 -p 10 -l ei_va,ei_v1`\
Node_count: range(5000,10000,200)\
Avg_degree: node_count * n / 100\

Plots are generated using `plot_speedup.py` and saved in the output directory at the end of each run.\
One cycles plot, one op_count plot\
and one performance plot for each algorithm\
and one speedup plot of all algorithms compared with "\_va".

## Real-world graph example
`./run.sh -g <graph_type> -w 10 -r 10 -p 10`\
Run experiments with graph [graph_type]\
10 warmups, 10 runs, 10 phases\

After all experiments are finished, plot the data.\
one cycles, one op_count vs. graph clustered bar graph.\
one performance plot per algorithm.\
`clustered_bar.sh -c <commit_hash> -w 10 -r 10 -p 10 -n 1`
`-n 1`: the first experiment of this configuration

## manually plot data from different experiments
`python plot_all.py -d [path-to-data eg.".../5-1-1/1"]\;[path] -p [where you'd like to save the plots] -l 500 -r 4000 -i 500 -n 5 -s 1 -a [all algos you'd like to plot] -b [base version name eg. "va"] [-t: use it if it's "_density" experiment]`
