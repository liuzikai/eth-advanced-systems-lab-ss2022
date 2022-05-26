import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from scipy import stats
import seaborn as sns
import matplotlib as mpl
import os, argparse, sys


parser = argparse.ArgumentParser(description='argparse')
parser.add_argument('--datadir', '-d', help='data directory', required=True)
parser.add_argument('--plotdir', '-p', help='plot directory', required=True)
parser.add_argument('--degree', '-n', type=int, help='avg directed degree = node*n/100 = adj list len after cut', required=True)
parser.add_argument('--low_node', '-a', type=int, help='start node count', required=True)
parser.add_argument('--high_node', '-b', type=int, help='end node count', required=True)
parser.add_argument('--interval', '-i', type=int, help='interval (step)', required=True)
parser.add_argument('--algos', '-l', type=str, help='algos', required=True)
parser.add_argument('--seed', '-s', type=int, help='seed for random graph', required=False)
args = parser.parse_args()

DATADIR = args.datadir
PLOTDIR = args.plotdir
degree = args.degree
low_node = args.low_node
high_node = args.high_node
algos = args.algos.split(",")
interval = args.interval
seed = args.seed

# mpl.rcParams['figure.dpi'] = 300

# os.environ['PATH'] = os.environ['PATH'] + ':/Library/TeX/texbin'
# mpl.rcParams['text.usetex'] = True
# mpl.rcParams['text.latex.preamble'] = [r'\usepackage{amsmath}'] # for \textsc command


random_graphs = []
for node_count in range(low_node, high_node, interval):
    avg_d = node_count * degree // 100
    if avg_d == node_count:
        avg_d -= 1
    edge_count = node_count * avg_d // 2

    if seed:
        random_graphs.append(f"generated_{seed}_{node_count}_{edge_count}")
    else:
        random_graphs.append(f"generated_{node_count}_{edge_count}")

node_counts = []
ops_data = []
cycles_data = []
perf_data = []
for algo in algos:
    for graph_name in random_graphs:
        df = pd.read_csv(f"{DATADIR}/{algo}/{graph_name}.csv", index_col=None)

        # Take median of phrases
        cols = list(df.columns)
        cols.remove("algorithm")
        cols.remove("ops")
        cycles = np.median(df[cols], axis=1)
        perf = df["ops"] / cycles

        # edge_counts.append(int(graph_name.split("_")[-1]))
        node_counts.append(int(graph_name.split("_")[-2]))
        ops_data.append(dict(zip(df["algorithm"], df["ops"])))
        cycles_data.append(dict(zip(df["algorithm"], cycles)))
        perf_data.append(dict(zip(df["algorithm"], perf)))

# Construct the data frames
ops_df = pd.DataFrame(ops_data, index=node_counts)
cycles_df = pd.DataFrame(cycles_data, index=node_counts)
perf_df = pd.DataFrame(perf_data, index=node_counts)

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


def plot(algos, node_counts, data, degree, ylabel, title, figname):
    fig, ax = plt.subplots()
    for algo in algos:
        ax.plot(node_counts, data[algo], ".-", label=algo)
    ax.set_xlabel(f"Node Count of the Random Graph (Avg degree = node * {degree}%)")
    ax.set_ylabel(ylabel, loc="top", rotation="horizontal")
    # ax.set_ylabel(ylabel)
    ax.legend(loc='best')
    fig.suptitle(title)
    plt.savefig(figname, bbox_inches="tight")


#---op count---
plot(algos, node_counts, ops_df, degree, "ops", "Op Count", f"{PLOTDIR}/ops.png")

#---runtime cycles---
plot(algos, node_counts, cycles_df, degree, "cycles", "Runtime", f"{PLOTDIR}/cycles.png")

#---perf ops/cycle---
plot(algos, node_counts, perf_df, degree, "ops/cycle", "Performance", f"{PLOTDIR}/perf.png")
