#!/bin/bash

set -e  # exit on error

./run_density.sh -a 500 -b 5000 -i 500 -n 5 -s 1 -w 1 -r 1 -p 10 -l fh_va,fh_v1,fh_v4
# ./run_density.sh -a 500 -b 5000 -i 500 -n 5 -s 1 -w 1 -r 1 -p 10 -l fh_base
