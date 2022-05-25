#!/bin/bash
# ./run.sh -a 500 -b 5000 -i 100 -n 5 -s 1 -l f_unroll_1,forward
# ./run.sh -a 5000 -b 50000 -i 1000 -n 5 -s 1 -l f_unroll_1,forward
./run.sh -a 1000 -b 10000 -i 200 -n 20 -s 1 -w 1 -r 1 -l forward0
# ./run_node.sh -a 200 -b 1500 -i 20 -n 99 -s 1 -w 1 -r 1 -p 1 -l forward0,forward2
# ./run_node.sh -a 1000 -b 5000 -i 500 -n 50 -s 1 -w 1 -r 1 -p 1 -l forward2
