#!/bin/bash

GRAPHS="GERMAN_ROAD_NETWORK,COMP_SCIENCE_AUTHORS,GOOGLE_CONTEST,HELP_LITERATURE,WWW_NOTRE_DAME,US_PATENTS"
# GRAPHS="US_PATENTS,GERMAN_ROAD_NETWORK,GOOGLE_CONTEST,WWW_NOTRE_DAME"

# directories
RUNEXP=$PWD
PROJECT="$(dirname $RUNEXP)"
DATADIR=$PROJECT/data
METADATA=$PROJECT/graphs/graph_data/metadata.json
echo "You are in $PWD"

help() {
cat << EOF
Usage:
    -h ... help
    -c ... short commit hash, default current branch current commit
    -w ... num warmups, default 5
    -r ... num runs, default 10
    -p ... num phases, default 5
    -n ... nth exp of the config, default 1
EOF
}

while getopts ":hc:w:r:p:n:" opt
do
    case $opt in
        h) help; exit 1;;
        c) COMMIT=$OPTARG;;
		w) WARMUP=$OPTARG;;
		r) RUN=$OPTARG;;
		p) PHASE=$OPTARG;;
        n) NUMBER=$OPTARG;;
        \?) help; exit 1;;
    esac
done

if [ -z $COMMIT ]; then COMMIT=$(git rev-parse --short HEAD); fi
if [ -z $WARMUP ]; then WARMUP="5"; fi
if [ -z $RUN ]; then RUN="10"; fi
if [ -z $PHASE ]; then PHASE="5"; fi
if [ -z $NUMBER ]; then NUMBER="1"; fi

# python $RUNEXP/clustered_bar.py -m $METADATA -d $DATADIR -p $DATADIR -c $COMMIT -f $WARMUP"-"$RUN"-"$PHASE -n $NUMBER

python3 $RUNEXP/clustered_bar.py -m $METADATA -d $DATADIR -p $DATADIR -c $COMMIT -f $WARMUP"-"$RUN"-"$PHASE -n $NUMBER -g $GRAPHS
