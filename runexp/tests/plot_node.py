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
parser.add_argument('--node', '-n', type=int, help='node_count=edge_count/n', required=True)
parser.add_argument('--low_edge', '-l', type=int, help='start edge count', required=True)
parser.add_argument('--high_edge', '-r', type=int, help='end edge count', required=True)
parser.add_argument('--interval', '-i', type=int, help='interval (step)', required=True)
parser.add_argument('--seed', '-s', type=int, help='seed for random graph', required=False)
args = parser.parse_args()

DATADIR = args.datadir
PLOTDIR = args.plotdir
n = args.node
low_edge = args.low_edge
high_edge = args.high_edge
interval = args.interval
seed = args.seed

# mpl.rcParams['figure.dpi'] = 300

# os.environ['PATH'] = os.environ['PATH'] + ':/Library/TeX/texbin'
# mpl.rcParams['text.usetex'] = True
# mpl.rcParams['text.latex.preamble'] = [r'\usepackage{amsmath}'] # for \textsc command


random_graphs = []
for node_count in range(low_edge, high_edge, interval):
    edge_count = node_count *n;
    if seed:
        random_graphs.append(f"generated_{seed}_{node_count}_{edge_count}")
    else:
        random_graphs.append(f"generated_{node_count}_{edge_count}")

edge_counts = []
ops_data = []
cycles_data = []
perf_data = []
for graph_name in random_graphs:
    df = pd.read_csv(f"{DATADIR}/{graph_name}.csv", index_col=None)

    # Take median of phrases
    cols = list(df.columns)
    cols.remove("algorithm")
    cols.remove("ops")
    cycles = np.median(df[cols], axis=1)
    perf = df["ops"] / cycles

    edge_counts.append(int(graph_name.split("_")[-1]))
    ops_data.append(dict(zip(df["algorithm"], df["ops"])))
    cycles_data.append(dict(zip(df["algorithm"], cycles)))
    perf_data.append(dict(zip(df["algorithm"], perf)))

# Construct the data frames
ops_df = pd.DataFrame(ops_data, index=edge_counts)
cycles_df = pd.DataFrame(cycles_data, index=edge_counts)
perf_df = pd.DataFrame(perf_data, index=edge_counts)

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


def plot(algos, edge_counts, data, n, ylabel, title, figname):
    fig, ax = plt.subplots()
    for algo in algos:
        ax.plot(edge_counts, data[algo], ".-", label=algo)
    ax.set_xlabel(f"Edge Count of the Random Graph (Avg degree {n*2})")
    ax.set_ylabel(ylabel, loc="top", rotation="horizontal")
    # ax.set_ylabel(ylabel)
    ax.legend(loc='best')
    fig.suptitle(title)
    plt.savefig(figname, bbox_inches="tight")


#---op count---
plot(algos, edge_counts, ops_df, n, "ops", "Op Count", f"{PLOTDIR}/ops.png")

#---runtime cycles---
plot(algos, edge_counts, cycles_df, n, "cycles", "Runtime", f"{PLOTDIR}/cycles.png")

#---perf ops/cycle---
plot(algos, edge_counts, perf_df, n, "ops/cycle", "Performance", f"{PLOTDIR}/perf.png")
