import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from scipy import stats
import seaborn as sns
import matplotlib as mpl
import os, argparse, sys
import csv, re


parser = argparse.ArgumentParser(description='argparse')
parser.add_argument('--datadir', '-d', help='data directory', required=True)
parser.add_argument('--plotdir', '-p', help='plot directory', required=True)
parser.add_argument('--degree', '-n', type=int, help='node_count=edge_count/n', required=True)
parser.add_argument('--low', '-l', type=int, help='start edge count', required=True)
parser.add_argument('--high', '-r', type=int, help='end edge count', required=True)
parser.add_argument('--interval', '-i', type=int, help='interval (step)', required=True)
parser.add_argument('--seed', '-s', type=int, help='seed for random graph', required=False)
parser.add_argument('--algos', '-a', type=str, help='version to compare with', required=False)
parser.add_argument('--type', '-t', help='is density experiment set', action='store_true')
parser.add_argument('--base', '-b', type=str, help='version to compare with', required=False)
args = parser.parse_args()

DATADIR = args.datadir.split(";")
PLOTDIR = args.plotdir
n = args.degree
low = args.low
high = args.high
interval = args.interval
seed = args.seed
t = args.type
algos = args.algos.split(",")
base = args.base

def read_data(random_graphs):
    node_counts = []
    ops_data = []
    cycles_data = []
    perfs_data = []
    for i, graph_name in enumerate(random_graphs):
        node_counts.append(int(graph_name.split("_")[-2]))
        v = 0
        ops = {}
        cycles = {}
        perfs = {}
        for d in DATADIR:
            print(v, d.split("/")[-1])
            with open(f"{d}/{graph_name}.csv", 'r') as f:
                reader = csv.DictReader(f)
                for row in reader:
                    algo = row["algorithm"]+str(v)
                    op = int(row["ops"])
                    del row["algorithm"]
                    del row["ops"]
                    cycle = np.median([int(x) for x in row.values()])
                    perf = op / cycle
                    ops[algo] = op
                    cycles[algo] = cycle
                    perfs[algo] = perf
            v += 1
        ops_data.append(ops)
        cycles_data.append(cycles)
        perfs_data.append(perfs)
    return node_counts, ops_data, cycles_data, perfs_data, v

def find_base_algo(algos, base):
    base_algo = ""
    for algo in algos:
        pattern = r'.*_'+base
        is_base = re.match(pattern, algo)
        if is_base:
            base_algo = algo
            break
    return base_algo

def calc_speedup(base_algo, node_counts, cycles_data):
    speedup_data = []
    for i in range(len(node_counts)):
        speedup = {}
        for algo in algos:
            if algo != base_algo:
                ratio = cycles_data[i][base_algo]/cycles_data[i][algo]
                speedup[algo] = ratio
        speedup_data.append(speedup)
    return speedup_data

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
sns.set_style('darkgrid') # darkgrid, white grid, dark, white and ticks
sns.color_palette('pastel')


def plot(algos, node_counts, data, n, xlabel, ylabel, title, figname):
    fig, ax = plt.subplots()
    for algo in algos:
        ax.plot(node_counts, data[algo], ".-", label=algo)
    ax.set_xlabel(xlabel)
    ax.set_ylabel(ylabel, loc="top", rotation="horizontal")
    ax.legend(loc='best')
    fig.suptitle(title)
    plt.savefig(figname, bbox_inches="tight")

def plot_separate(algos, node_counts, data, n, xlabel, ylabel, title, figname, format, v):
    for algo in algos:
        fig, ax = plt.subplots()
        for vi in range(v):
            ax.plot(node_counts, data[algo+str(vi)], ".-", label=algo+str(vi))
        ax.set_xlabel(xlabel)
        ax.set_ylabel(ylabel, loc="top", rotation="horizontal")
        ax.legend(loc='best')
        fig.suptitle(f"{title}, {algo}")
        plt.savefig(f"{figname}-{algo}{format}", bbox_inches="tight")

def plot_speedup(algos, node_counts, data, n, xlabel, ylabel, title, figname, base_algo):
    fig, ax = plt.subplots()
    for algo in algos:
        if algo != base_algo:
            ax.plot(node_counts, data[algo], ".-", label=algo)
    ax.set_xlabel(xlabel)
    ax.set_ylabel(ylabel, loc="top", rotation="horizontal")
    ax.legend(loc='best')
    fig.suptitle(title)
    plt.savefig(figname, bbox_inches="tight")


if __name__ == "__main__":
    random_graphs = []

    if t:
        for node_count in range(low, high, interval):
            avg_d = node_count * n // 100
            if avg_d == node_count:
                avg_d -= 1
            edge_count = node_count * avg_d // 2
            if seed:
                random_graphs.append(f"generated_{seed}_{node_count}_{edge_count}")
            else:
                random_graphs.append(f"generated_{node_count}_{edge_count}")
    else:
        for edge_count in range(low, high, interval):
            node_count = edge_count // n;
            if seed:
                random_graphs.append(f"generated_{seed}_{node_count}_{edge_count}")
            else:
                random_graphs.append(f"generated_{node_count}_{edge_count}")

    node_counts, ops_data, cycles_data, perfs_data, v = read_data(random_graphs)
    temp = []
    for vi in range(v):
        for algo in algos:
            temp.append(algo+str(vi))
    algos_original = algos
    algos = temp

    # Construct the data frames
    ops_df = pd.DataFrame(ops_data, index=node_counts)
    cycles_df = pd.DataFrame(cycles_data, index=node_counts)
    perfs_df = pd.DataFrame(perfs_data, index=node_counts)
    # algos = list(cycles_df.columns)

    if t:
        xlabel = f"Node Count (Avg degree = |V| * {n}%)"
    else:
        xlabel = f"Node Count (|E| = |V| * {n})"
    #---op count---
    plot(algos, node_counts, ops_df, n, xlabel, "ops", "Random Graph: Op Count", f"{PLOTDIR}/ops.png")
    #---runtime cycles---
    plot(algos, node_counts, cycles_df, n, xlabel, "cycles", "Random Graph: Runtime", f"{PLOTDIR}/cycles.png")
    #---perf ops/cycle---
    plot_separate(algos_original, node_counts, perfs_df, n, xlabel, "ops/cycle", "Random Graph: Performance", f"{PLOTDIR}/perf", ".png", v)

    #---speedup---
    if base:
        base_algo = find_base_algo(algos, base)
        if base_algo:
            print("version to compare with:", base_algo)
            speedup_data = calc_speedup(base_algo, node_counts, cycles_data)
            speedup_df = pd.DataFrame(speedup_data, index=node_counts)
            plot_speedup(algos, node_counts, speedup_df, n, xlabel, "x", "Speedup", f"{PLOTDIR}/speedup.png", base_algo)
        else:
            print("no base version")
