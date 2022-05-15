import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from scipy import stats
import seaborn as sns
import matplotlib as mpl
import os, argparse, sys
import json


parser = argparse.ArgumentParser(description='argparse')
parser.add_argument('--graph_metadata', '-m', help='graph metadata', required=True)
parser.add_argument('--datadir', '-d', help='data directory', required=True)
parser.add_argument('--plotdir', '-p', help='plot directory', required=True)
parser.add_argument('--commit_hash', '-c', help='commit hash', required=True)
parser.add_argument('--config', '-f', help='warmups-runs-phases', required=True)
parser.add_argument('--number', '-n', help='nth exp of the config', required=True)
parser.add_argument('--graphs', '-g', help='realworld graph types', default="GERMAN_ROAD_NETWORK,ACTOR_MOVIE_GRAPH,COMP_SCIENCE_AUTHORS,GOOGLE_CONTEST,HEP_LITERATURE,ROUTER_NETWORK,WWW_NOTRE_DAME,US_PATENTS", required=False)
args = parser.parse_args()


DATADIR = args.datadir
PLOTDIR = args.plotdir
commit_hash = args.commit_hash
config = args.config
number = args.number
graphs = args.graphs.split(",")

with open(args.graph_metadata, 'r') as file:
    graph_metadata=json.load(file)

# sort graphs by node count then by edge count
gne=[]
for i in range(len(graphs)):
    gne.append((graph_metadata[graphs[i]]["num_node"], graph_metadata[graphs[i]]["num_edge"], graphs[i]))
gne.sort()
num_nodes,num_edges,gs=zip(*gne)
for i in range(len(gne)):
    gne[i] = f"{gne[i][2]}\n{gne[i][0]},{gne[i][1]}"

# mpl.rcParams['figure.dpi'] = 300

# os.environ['PATH'] = os.environ['PATH'] + ':/Library/TeX/texbin'
# mpl.rcParams['text.usetex'] = True
# mpl.rcParams['text.latex.preamble'] = [r'\usepackage{amsmath}'] # for \textsc command

datafiles = []
for graph in gs:
    datafiles.append(os.path.join(DATADIR, f"{commit_hash}-{graph}", config, number, f"{graph}.csv"))

ops_data = []
cycles_data = []
perf_data = []
for data in datafiles:
    df = pd.read_csv(data, index_col=None)

    # Take median of phrases
    cols = list(df.columns)
    cols.remove("algorithm")
    cols.remove("ops")
    cycles = np.median(df[cols], axis=1)
    perf = df["ops"] / cycles

    ops_data.append(dict(zip(df["algorithm"], df["ops"])))
    cycles_data.append(dict(zip(df["algorithm"], cycles)))
    perf_data.append(dict(zip(df["algorithm"], perf)))

# Construct the data frames
ops_df = pd.DataFrame(ops_data, index=gs)
cycles_df = pd.DataFrame(cycles_data, index=gs)
perf_df = pd.DataFrame(perf_data, index=gs)

algos = list(cycles_df.columns)


# ----- plot -----
sns.set(font='Times New Roman',
        rc={
 'axes.axisbelow': False,
 'axes.edgecolor': 'lightgrey',
 'axes.facecolor': 'None',
 'axes.grid': False,
 'axes.labelcolor': 'dimgrey',
 'axes.spines.right': False,
 'axes.spines.top': False,
 'figure.facecolor': 'white',
 'lines.solid_capstyle': 'round',
 'patch.edgecolor': 'w',
 'patch.force_edgecolor': True,
 'text.color': 'dimgrey',
 'xtick.bottom': False,
 'xtick.color': 'dimgrey',
 'xtick.direction': 'out',
 'xtick.top': False,
 'ytick.color': 'dimgrey',
 'ytick.direction': 'out',
 'ytick.left': False,
 'ytick.right': False})
# sns.set_context("notebook", rc={"font.size":16,
#                                 "axes.titlesize":20,
#                                 "axes.labelsize":18})
sns.set_style('darkgrid') # darkgrid, white grid, dark, white and ticks
# plt.rc('axes', titlesize=18)     # fontsize of the axes title
# plt.rc('axes', labelsize=14)    # fontsize of the x and y labels
# plt.rc('xtick', labelsize=13)    # fontsize of the tick labels
# plt.rc('ytick', labelsize=13)    # fontsize of the tick labels
# plt.rc('legend', fontsize=13)    # legend fontsize
# plt.rc('font', size=13)          # controls default text sizes
sns.color_palette('pastel')


def plot(x, width, algos, data, ylabel, gne, title, figname, yscale="linear"):
    fig, ax = plt.subplots(figsize=(12, 6))
    rects=[]
    for i in range(len(algos)):
        rects.append(ax.bar(x + (-1+i)*width, data[algos[i]].values.tolist(), width, label=algos[i]))

    ax.set_xlabel("Graphs\nnodes,edges")
    ax.set_ylabel(ylabel, loc="top", rotation="horizontal")
    ax.set_xticks(x)
    ax.set_xticklabels(gne)
    ax.set_title(title)
    ax.set_yscale(yscale)
    # for i in range(len(algos)):
    #     ax.bar_label(rects2[i], padding=3)
    fig.tight_layout()
    # ax.set_axisbelow(True)
    # ax.grid(color='gray', linestyle='-', linewidth=0.5)
    ax.legend(loc='best')
    plt.savefig(figname, bbox_inches="tight")
    

#---x axis---
x = np.arange(len(gs))  # the label locations
width = len(gs)/30  # the width of the bars

#---op count---
plot(x, width, algos, ops_df, "ops", gne, 'Op Count', f"{PLOTDIR}/large_graphs_ops.png", yscale="log")

#---runtime cycles---
plot(x, width, algos, cycles_df, "cycles", gne, 'Runtime', f"{PLOTDIR}/large_graphs_cycles.png", yscale="log")

#---perf ops/cycle---
plot(x, width, algos, perf_df, "ops/cycle", gne, 'Performance', f"{PLOTDIR}/large_graphs_perf.png", yscale="log")
