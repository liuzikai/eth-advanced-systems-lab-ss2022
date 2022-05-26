#!/bin/bash
# ./run_density.sh -a 10 -b 20 -i 10 -n 50 -s 1 -w 1 -r 1 -p 1 -l ei_base,ei_va
./run_density.sh -a 100 -b 200 -i 100 -n 10 -s 1 -w 0 -r 1 -p 1 -l f_base,f_va
# ./run_density.sh -a 100 -b 1000 -i 100 -n 10 -s 1 -w 1 -r 1 -p 1 -l fh_base,fh_v2,fh_va
# IFS=',' read -ra ADDR <<< "$ALGOS"
# for ALGO in "${ADDR[@]}"; do
#     echo $ALGO
# done
