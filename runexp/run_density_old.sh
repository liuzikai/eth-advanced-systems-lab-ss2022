#!/bin/bash

#--------------------------------------------#
#| constant density = n%                    |
#| node count variable= V                   |
#| avg degree D = V * n / 100               |
#| edge count = V * D / 2 = V^2 * n / 200   |
#--------------------------------------------#

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
    -n ... n such that avg directed degree = node * (n/100), adj list len after cut
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
        a) LOWNODE=$OPTARG;;
        b) HIGHNODE=$OPTARG;;
        i) INTERVAL=$OPTARG;;
        n) DEGREE=$OPTARG;;
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
    if [ -z $LOWNODE ]; then LOWNODE="1000"; fi
    if [ -z $HIGHNODE ]; then HIGHNODE="5000"; fi
    if [ -z $INTERVAL ]; then INTERVAL="500"; fi
    if [ -z $DEGREE ]; then DEGREE="10"; fi
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
    EXPDIR=$DATADIR/"density-"$ALGO"-"$COMMIT"-"$GRAPHTYPE"-"$LOWNODE"-"$HIGHNODE"-"$INTERVAL"-"$DEGREE
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
    JSON_STR='{"algo":"'"$ALGO"'",
            "commit_hash":"'"$(git rev-parse HEAD)"'",
            "graph_type":"'"$GRAPHTYPE"'",
            "num_warmup":'$WARMUP',
            "num_runs":'$RUN',
            "num_phases":'$PHASE','
    if [ $GRAPHTYPE = "GENERATED" ]; then
        JSON_STR=$JSON_STR'"low_node":'$LOWNODE',
                            "high_node":'$HIGHNODE',
                            "interval":'$INTERVAL',
                            "degree":'$DEGREE','
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
    if [[ "$OSTYPE" == "darwin"* ]]; then
        sed -i '' -e 's/"number":'$NUMBER'/"number":'$(($NUMBER+1))'/' $EXP/config.json
    else
        sed -i 's/"number":'$NUMBER'/"number":'$(($NUMBER+1))'/' $EXP/config.json
    fi
else
    # more runs of the exp config
    cat $EXP/config.json | python3 -c "
import sys, json
config = json.load(sys.stdin)
for a,v in config.items():
    print(f'\t{a}: {v}')
"
    NUMBER=$(cat $EXP/config.json | python3 -c "import sys, json; print(json.load(sys.stdin)['number'])")
    if [[ "$OSTYPE" == "darwin"* ]]; then
        sed -i '' -e 's/"number":'$NUMBER'/"number":'$(($NUMBER+1))'/' $EXP/config.json
    else
        sed -i 's/"number":'$NUMBER'/"number":'$(($NUMBER+1))'/' $EXP/config.json
    fi
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
    for (( i=$(($LOWNODE)); i<$(($HIGHNODE)); i+=$(($INTERVAL)) ))
    do
        AVG_D=$(($i * $DEGREE / 100))
        if [ $AVG_D = $i ]; then
            AVG_D=$(($AVG_D - 1))
        fi
        EDGE=$(($i * $AVG_D / 2))
        if [ -z $SEED ]; then
            graph="generated_"$i"_"$EDGE
        else
            graph="generated_"$SEED"_"$i"_"$EDGE
        fi
        echo "+ $graph"
        if [ -z $SEED ]; then
            # always generate random graph without seed
            ./graph_generation -gt $GRAPHTYPE -num_nodes $i -num_edges $EDGE -shuffle_edges -density EXIST -o $INPUTDIR/$graph.txt
        else
            # generate graph if doesn't exist
            if [ ! -f $INPUTDIR/$graph.txt ]; then
                ./graph_generation -gt $GRAPHTYPE -num_nodes $i -num_edges $EDGE -seed $SEED -shuffle_edges -density EXIST -o $INPUTDIR/$graph.txt
            fi
        fi
        ./benchmark -pre_cut -num_warmups $WARMUP -num_runs $RUN  -num_phases $PHASE -o $EXPNUM/$graph.csv -algorithm $ALGO -graph $INPUTDIR/$graph.txt
    done

    cd $RUNEXP
    echo "Your are in $RUNEXP"
    if [ -z $SEED ]; then
        python3 plot_density.py -d $EXPNUM -p $EXPNUM -n $DEGREE -l $LOWNODE -r $HIGHNODE -i $INTERVAL
    else
        python3 plot_density.py -d $EXPNUM -p $EXPNUM -n $DEGREE -l $LOWNODE -r $HIGHNODE -i $INTERVAL -s $SEED
    fi
else
    graph=$GRAPHTYPE
    echo "+ $graph"
    # generate graph if doesn't exist
    if [ ! -f $INPUTDIR/$graph.txt ]; then
        ./graph_generation -gt $GRAPHTYPE -shuffle_edges -seed 0 -o $INPUTDIR/$graph.txt
    fi
    ./benchmark -pre_cut -num_warmups $WARMUP -num_runs $RUN  -num_phases $PHASE -o $EXPNUM/$graph.csv -algorithm $ALGO -graph $INPUTDIR/$graph.txt
fi
