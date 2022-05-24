#!/bin/bash
# ./run.sh -a 500 -b 5000 -i 100 -n 5 -s 1 -l e_unroll_2,e_unroll_1,edge_iterator
# ./run.sh -a 5000 -b 50000 -i 1000 -n 5 -s 1 -l e_unroll_2,e_unroll_1,edge_iterator
# ./run.sh -a 100 -b 10000 -i 100 -n 3 -s 1 -l edge_iterator
# ./run.sh -a 100 -b 5000 -i 100 -n 3 -s 1 -w 5 -r 200 -l edge_iterator
./run_node.sh -a 200 -b 1500 -i 20 -n 99 -s 1 -w 1 -r 1 -p 1 -l ei_base
./run_node.sh -a 1000 -b 5000 -i 500 -n 50 -s 1 -w 1 -r 1 -p 1 -l ei_base
./run_node.sh -a 1000 -b 5000 -i 500 -n 250 -s 1 -w 1 -r 1 -p 1 -l ei_base
