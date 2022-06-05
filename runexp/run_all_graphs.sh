#!/bin/bash

set -e  # exit on error

# german: GERMAN_ROAD_NETWORK
# DOESN'T WORK actor: ACTOR_MOVIE_GRAPH
# comp: COMP_SCIENCE_AUTHORS
# google: GOOGLE_CONTEST
# hep: HEP_LITERATURE
# www: WWW_NOTRE_DAME
# us: US_PATENTS
ALGO=ei_va,ei_u4,ei_vec5
BASE=va
# ./run.sh -g german -w 0 -r 1 -p 1 -l $ALGO
#./run.sh -g actor -w 0 -r 1 -p 1 -l $ALGO
# ./run.sh -g comp -w 0 -r 1 -p 1 -l $ALGO
# ./run.sh -g google -w 0 -r 1 -p 1 -l $ALGO
./run.sh -g hep -w 0 -r 1 -p 1 -l $ALGO
./run.sh -g www -w 0 -r 1 -p 1 -l $ALGO
# ./run.sh -g us -w 0 -r 1 -p 1 -l $ALGO
COMMIT=$(git rev-parse --short HEAD)
./clustered_bar.sh -c $COMMIT -w 0 -r 1 -p 1 -n 1 -l $ALGO -g HEP_LITERATURE,WWW_NOTRE_DAME -b $BASE
cd ../data
rm -r $COMMIT*
cd ../RUNEXP
