#!/bin/bash

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
    -l (required)... list of algos
    -g ... list of graphs, default "GERMAN_ROAD_NETWORK,COMP_SCIENCE_AUTHORS,GOOGLE_CONTEST,HEP_LITERATURE,WWW_NOTRE_DAME,US_PATENTS"
EOF
}

while getopts ":hc:w:r:p:n:l:g:" opt
do
    case $opt in
        h) help; exit 1;;
        c) COMMIT=$OPTARG;;
		w) WARMUP=$OPTARG;;
		r) RUN=$OPTARG;;
		p) PHASE=$OPTARG;;
        n) NUMBER=$OPTARG;;
        l) ALGO=$OPTARG;;
        g) GRAPHS=$OPTARG;;
        \?) help; exit 1;;
    esac
done

if [ -z $COMMIT ]; then COMMIT=$(git rev-parse --short HEAD); fi
if [ -z $WARMUP ]; then WARMUP="5"; fi
if [ -z $RUN ]; then RUN="10"; fi
if [ -z $PHASE ]; then PHASE="5"; fi
if [ -z $NUMBER ]; then NUMBER="1"; fi
if [ -z $ALGO ]; then
    echo "-l algos is required"
    exit 1
fi
if [ -z $GRAPHS ]; then
    GRAPHS="GERMAN_ROAD_NETWORK,COMP_SCIENCE_AUTHORS,GOOGLE_CONTEST,HEP_LITERATURE,WWW_NOTRE_DAME,US_PATENTS"
fi

PLOTDIR=$DATADIR/$ALGO"-"$COMMIT
if [ ! -d $PLOTDIR ]; then
    mkdir $PLOTDIR
fi

# python $RUNEXP/clustered_bar.py -m $METADATA -d $DATADIR -p $DATADIR -c $COMMIT -f $WARMUP"-"$RUN"-"$PHASE -n $NUMBER

python3 $RUNEXP/clustered_bar_old.py -m $METADATA -d $DATADIR -p $PLOTDIR -c $COMMIT -f $WARMUP"-"$RUN"-"$PHASE -n $NUMBER -g $GRAPHS
