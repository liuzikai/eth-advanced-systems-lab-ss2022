#!/bin/bash

source env.sh

# directories
RUNEXP=$PWD
PROJECT="$(dirname $RUNEXP)"
BUILDDIR=$PROJECT/build
INPUTDIR=$PROJECT/input
DATADIR=$PROJECT/data
echo "You are in $PWD"

help() {
cat << EOF
Usage:
    -h ... help
    -g ... graph type, optional
        german ... GERMAN_ROAD_NETWORK
        actor ... ACTOR_MOVIE_GRAPH
        comp ... COMP_SCIENCE_AUTHORS
        google ... GOOGLE_CONTEST
        hep ... HEP_LITERATURE
        router ... ROUTER_NETWORK
        www ... WWW_NOTRE_DAME
        us ... US_PATENTS
        gen or [remove -g] ... GENERATED
    -a ... start edge count, default 1000
    -b ... end edge count, default 5000
    -i ... interval (step), default 50
        for edge_count in range(l,r,i)
    -n ... n such that node_count = edge_count / n, default 10
    -s ... seed for random graph, optional
    -w ... num warmups, default 5
    -r ... num runs, default 10
    -p ... num phases, default 5
    -l ... list of algorithms algo1,algo2,algo3
EOF
}

graph_type() {
cat << EOF
-g:
    german ... GERMAN_ROAD_NETWORK
    actor ... ACTOR_MOVIE_GRAPH
    comp ... COMP_SCIENCE_AUTHORS
    google ... GOOGLE_CONTEST
    hep ... HEP_LITERATURE
    router ... ROUTER_NETWORK
    www ... WWW_NOTRE_DAME
    us ... US_PATENTS
    gen or [remove -g] ... GENERATED
EOF
}

echo "!!! Configure your environment: env.sh !!!"

# check if directories exist
if [ ! -d $PROJECT ]; then
    echo "$PROJECT doesn't exist!"; exit 1
fi
if [ ! -d $RUNEXP ]; then
    echo "$RUNEXP doesn't exist!"; exit 1
fi
if [ ! -d $BUILDDIR ]; then
    mkdir $BUILDDIR
fi
if [ ! -d $DATADIR ]; then
    mkdir $DATADIR
fi

# read opts
while getopts ":hg:a:b:i:n:s:w:r:p:l:" opt
do
    case $opt in
        h) help; exit 1;;
        g) GRAPHTYPE=$OPTARG;;
        a) LOWEDGE=$OPTARG;;
        b) HIGHEDGE=$OPTARG;;
        i) INTERVAL=$OPTARG;;
        n) NODE=$OPTARG;;
        s) SEED=$OPTARG;;
        w) WARMUP=$OPTARG;;
        r) RUN=$OPTARG;;
        p) PHASE=$OPTARG;;
        l) ALGO=$OPTARG;;
        \?) help; exit 1;;
    esac
done

# algos
if [ -z $ALGO ]; then
    ALGO="edge_iterator,forward,forward_hashed"
fi
echo $ALGO

# read opt graph type
if [ -z $GRAPHTYPE ]; then
    GRAPHTYPE="GENERATED"
    if [ -z $LOWEDGE ]; then LOWEDGE="1000"; fi
    if [ -z $HIGHEDGE ]; then HIGHEDGE="5000"; fi
    if [ -z $INTERVAL ]; then INTERVAL="500"; fi
    if [ -z $NODE ]; then NODE="10"; fi
fi
case $GRAPHTYPE in
    german) GRAPHTYPE="GERMAN_ROAD_NETWORK";;
    actor) GRAPHTYPE="ACTOR_MOVIE_GRAPH";;
    comp) GRAPHTYPE="COMP_SCIENCE_AUTHORS";;
    google) GRAPHTYPE="GOOGLE_CONTEST";;
    hep) GRAPHTYPE="HEP_LITERATURE";;
    router) GRAPHTYPE="ROUTER_NETWORK";;
    www) GRAPHTYPE="WWW_NOTRE_DAME";;
    us) GRAPHTYPE="US_PATENTS";;
    gen) GRAPHTYPE="GENERATED";;
    GENERATED) ;;
    ?) graph_type; exit 1;;
esac

# read opt experiment number
if [ -z $WARMUP ]; then WARMUP="5"; fi
if [ -z $RUN ]; then RUN="10"; fi
if [ -z $PHASE ]; then PHASE="5"; fi

# create experiment dir and save exp config
COMMIT=$(git rev-parse --short HEAD)
if [ $GRAPHTYPE = "GENERATED" ]; then
    EXPDIR=$DATADIR/$COMMIT"-"$GRAPHTYPE"-"$LOWEDGE"-"$HIGHEDGE"-"$INTERVAL"-"$NODE
    if [ ! -z $SEED ]; then
        EXPDIR=$EXPDIR"-"$SEED
    fi
else
    EXPDIR=$DATADIR/$COMMIT"-"$GRAPHTYPE
fi
if [ ! -d $EXPDIR ]; then mkdir $EXPDIR; fi

EXP=$EXPDIR/$WARMUP"-"$RUN"-"$PHASE
if [ ! -d $EXP ]; then
    # the first run of the exp config
    mkdir $EXP
    NUMBER=1
    JSON_STR='{"commit_hash":"'"$(git rev-parse HEAD)"'",
            "graph_type":"'"$GRAPHTYPE"'",
            "num_warmup":'$WARMUP',
            "num_runs":'$RUN',
            "num_phases":'$PHASE','
    if [ $GRAPHTYPE = "GENERATED" ]; then
        JSON_STR=$JSON_STR'"low_edge":'$LOWEDGE',
                            "high_edge":'$HIGHEDGE',
                            "interval":'$INTERVAL',
                            "node":'$NODE','
        if [ ! -z $SEED ]; then
            JSON_STR=$JSON_STR'"seed":'$SEED','
        fi
    fi
    JSON_STR=$JSON_STR'"number":'$NUMBER'}'
    echo $JSON_STR > $EXP/config.json
    echo $JSON_STR | python3 -c "
import sys, json
config = json.load(sys.stdin)
for a,v in config.items():
    print(f'\t{a}: {v}')
"
    sed -i '' -e 's/"number":'$NUMBER'/"number":'$(($NUMBER+1))'/' $EXP/config.json
else
    # more runs of the exp config
    cat $EXP/config.json | python3 -c "
import sys, json
config = json.load(sys.stdin)
for a,v in config.items():
    print(f'\t{a}: {v}')
"
    NUMBER=$(cat $EXP/config.json | python3 -c "import sys, json; print(json.load(sys.stdin)['number'])")
    sed -i '' -e 's/"number":'$NUMBER'/"number":'$(($NUMBER+1))'/' $EXP/config.json
fi

EXPNUM=$EXP/$NUMBER
if [ ! -d $EXPNUM ]; then mkdir $EXPNUM; fi
echo ">>> Data is in $EXPNUM"

# build and run
cd $BUILDDIR
echo ">>> You are in $BUILDDIR"

echo ">>> building $BUILDTYPE... Check if the compiler is GNU"
cmake $PROJECT -DCMAKE_C_COMPILER=$GCC -DCMAKE_CXX_COMPILER=$GXX -DCMAKE_BUILD_TYPE=$BUILDTYPE
make -j$(nproc)

echo ">>> running..."
if [ $GRAPHTYPE = "GENERATED" ]; then
    for (( i=$(($LOWEDGE)); i<$(($HIGHEDGE)); i+=$(($INTERVAL)) ))
    do
        if [ -z $SEED ]; then
            graph="generated_"$(($i/$NODE))"_"$i
        else
            graph="generated_"$SEED"_"$(($i/$NODE))"_"$i
        fi
        echo "+ $graph"
        if [ -z $SEED ]; then
            # always generate random graph without seed
            ./graph_generation -gt $GRAPHTYPE -num_nodes $(($i/$NODE)) -num_edges $i -shuffle_edges -o $INPUTDIR/$graph.txt
        else
            # generate graph if doesn't exist
            if [ ! -f $INPUTDIR/$graph.txt ]; then
                ./graph_generation -gt $GRAPHTYPE -num_nodes $(($i/$NODE)) -num_edges $i -seed $SEED -shuffle_edges -o $INPUTDIR/$graph.txt
            fi
        fi
        ./benchmark -num_warmups $WARMUP -num_runs $RUN  -num_phases $PHASE -o $EXPNUM/$graph.csv -algorithm $ALGO -graph $INPUTDIR/$graph.txt
    done

    cd $RUNEXP
    echo "Your are in $RUNEXP"
    if [ -z $SEED ]; then
        python3 plot.py -d $EXPNUM -p $EXPNUM -n $NODE -l $LOWEDGE -r $HIGHEDGE -i $INTERVAL
    else
        python3 plot.py -d $EXPNUM -p $EXPNUM -n $NODE -l $LOWEDGE -r $HIGHEDGE -i $INTERVAL -s $SEED
    fi
else
    graph=$GRAPHTYPE
    echo "+ $graph"
    # generate graph if doesn't exist
    if [ ! -f $INPUTDIR/$graph.txt ]; then
        ./graph_generation -gt $GRAPHTYPE -shuffle_edges -seed 0 -o $INPUTDIR/$graph.txt
    fi
    ./benchmark -num_warmups $WARMUP -num_runs $RUN  -num_phases $PHASE -o $EXPNUM/$graph.csv -algorithm $ALGO -graph $INPUTDIR/$graph.txt
fi
