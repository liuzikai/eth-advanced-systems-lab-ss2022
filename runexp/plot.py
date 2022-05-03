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
for edge_count in range(low_edge, high_edge, interval):
    node_count = edge_count // n;
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

fig1, ax1 = plt.subplots()
for algo in algos:
    ax1.plot(edge_counts, ops_df[algo], ".-", label=algo)
ax1.set_xlabel(f"Edge Count of the Random Graph (Node count = edge/{n})")
ax1.set_ylabel("Ops", loc="top", rotation="horizontal")
# ax1.set_ylabel("Ops")
ax1.legend(loc='best')
fig1.suptitle("Op Count")
plt.savefig(f"{PLOTDIR}/ops.png", bbox_inches="tight")

fig2, ax2 = plt.subplots()
for algo in algos:
    ax2.plot(edge_counts, cycles_df[algo], ".-", label=algo)
ax2.set_xlabel(f"Edge Count of the Random Graph (Node count = edge/{n})")
ax2.set_ylabel("Cycles", loc="top", rotation="horizontal")
# ax2.set_ylabel("Cycles")
ax2.legend(loc='best')
fig2.suptitle("Runtime")
plt.savefig(f"{PLOTDIR}/cycles.png", bbox_inches="tight")

fig3, ax3 = plt.subplots()
for algo in algos:
    ax3.plot(edge_counts, perf_df[algo], ".-", label=algo)
ax3.set_xlabel(f"Edge Count of the Random Graph (Node count = edge/{n})")
ax3.set_ylabel("Performance [ops/cycle]")
ax3.legend(loc='best')
fig3.suptitle("Performance")
plt.savefig(f"{PLOTDIR}/perf.png", bbox_inches="tight")
