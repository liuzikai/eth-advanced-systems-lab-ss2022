import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from scipy import stats
import seaborn as sns
import matplotlib as mpl
import os, argparse, sys
import csv, re, json


parser = argparse.ArgumentParser(description='argparse')
parser.add_argument('--datadir', '-d', help='data directory', required=True)
parser.add_argument('--plotdir', '-p', help='plot directory', required=True)
parser.add_argument('--degree', '-n', type=str, help='node_count=edge_count/n', required=True)
parser.add_argument('--low', '-l', type=int, help='start edge count', required=False)
parser.add_argument('--high', '-r', type=int, help='end edge count', required=False)
parser.add_argument('--interval', '-i', type=int, help='interval (step)', required=False)
parser.add_argument('--seed', '-s', type=int, help='seed for random graph', required=False)
# parser.add_argument('--type', '-t', help='is density experiment set', action='store_true')
parser.add_argument('--base', '-b', type=str, help='version to compare with', required=False)
parser.add_argument('--algos', '-a', type=str, help='version to compare with', required=False)
parser.add_argument('--rename', '-m', help='rename the algos', action='store_true')
parser.add_argument('--versions', '-v', type=str, help='versioning', required=False)
args = parser.parse_args()

DATADIR = args.datadir.split(";")
PLOTDIR = args.plotdir
n = args.degree.split(",")
n = [int(x) for x in n]
low = args.low
high = args.high
interval = args.interval
seed = args.seed
# t = args.type
t = True
base = args.base
algos_to_plot = args.algos
rename = args.rename
rename = True
if rename:
    with open("rename.json", "r") as f:
        rename_table = json.load(f)
versions = args.versions
if versions:
    versions = versions.split(",")

def read_data(random_graphs):
    ops = {}
    cycles = {}
    perfs = {}
    # gather_algos = {}
    all_algos = set()
    for i, graph_name in enumerate(random_graphs):
        edge_count = int(graph_name.split("_")[-1])
        if edge_count not in ops:
            ops[edge_count] = {}
        if edge_count not in cycles:
            cycles[edge_count] = {}
        if edge_count not in perfs:
            perfs[edge_count] = {}
        # ops = {}
        # cycles = {}
        # perfs = {}
        # gather_algos = {}

        for v,d in enumerate(DATADIR):
            if os.path.isfile(f"{d}/{graph_name}.csv"):
                with open(f"{d}/{graph_name}.csv", 'r') as f:
                    reader = csv.DictReader(f)
                    for row in reader:
                        ar = row["algorithm"]
                        if algos_to_plot:
                            if ar not in algos_to_plot:
                                continue
                        if versions:
                            algo = ar + "-" + str(v)
                            # gather_algos[ar] = gather_algos.get(ar, 0) + 1
                        else:
                            # if gather_algos.get(ar, 0):
                            #     print(f"ERROR: duplicate algo data {row['algorithm']}, provide version names!")
                            #     exit(1)
                            # else:
                            algo = ar
                                # gather_algos[ar] = 1
                        all_algos.add(algo)

                        op = int(row["ops"])
                        del row["algorithm"]
                        del row["ops"]
                        cycle = np.median([int(x) for x in row.values()])
                        perf = op / cycle
                        ops[edge_count][algo] = op
                        cycles[edge_count][algo] = cycle
                        perfs[edge_count][algo] = perf
        # ops_data.append(ops)
        # cycles_data.append(cycles)
        # perfs_data.append(perfs)
    ops_data = []
    cycles_data = []
    perfs_data = []
    for v in ops.values():
        ops_data.append(v)
    for v in cycles.values():
        cycles_data.append(v)
    for v in perfs.values():
        perfs_data.append(v)

    return list(ops.keys()), ops_data, cycles_data, perfs_data, all_algos

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
            else:
                speedup[algo] = 1
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

def get_label(algo):
    algo = algo.split("-")
    ar = rename_table[algo[0]]
    if versions:
        return ar + ", " + versions[int(algo[1])]
    else:
        return ar


def plot(algos, node_counts, data, n, xlabel, ylabel, title, figname):
    fig, ax = plt.subplots(figsize=(9, 6), dpi=300)
    for algo in algos:
        ax.plot(node_counts, data[algo], ".-", label=get_label(algo))
    ax.set_xlabel(xlabel)
    ax.set_ylabel(ylabel, loc="top", rotation="horizontal")
    # ax.legend(loc='best')
    # box = ax.get_position()
    # ax.set_position([box.x0, box.y0 + box.height * 0.1,
    #              box.width, box.height * 0.9])
    # handles, labels = ax.get_legend_handles_labels()
    # labels, handles = zip(*sorted(zip(labels, handles), key=lambda t: t[0]))
    # ax.legend(handles, labels, loc='upper center', bbox_to_anchor=(0.5, -0.15), ncol=2)
    box = ax.get_position()
    ax.set_position([box.x0, box.y0, box.width * 0.8, box.height])
    handles, labels = ax.get_legend_handles_labels()
    # sort both labels and handles by labels
    labels, handles = zip(*sorted(zip(labels, handles), key=lambda t: t[0]))
    ax.legend(handles, labels, loc='center left', bbox_to_anchor=(1, 0.5))
    fig.suptitle(title)
    plt.savefig(figname, bbox_inches="tight")

def plot_separate(algos, node_counts, data, n, xlabel, ylabel, title, figname, format):
    for algo in algos:
        fig, ax = plt.subplots(figsize=(9, 6),dpi=300)
        algo_label = get_label(algo).replace("\n", "")
        ax.plot(node_counts, data[algo], ".-", label=algo_label)
        ax.set_xlabel(xlabel)
        ax.set_ylabel(ylabel, loc="top", rotation="horizontal")
        # ax.legend(loc='best')
        # ax.legend(loc='upper center', bbox_to_anchor=(0.5, -0.15), ncol=2)
        fig.suptitle(f"{title}\n{algo_label}")
        plt.savefig(f"{figname}-{algo}{format}", bbox_inches="tight")

def plot_speedup(algos, node_counts, data, n, xlabel, ylabel, title, figname, base_algo):
    fig, ax = plt.subplots(figsize=(9, 6), dpi=300)
    for algo in algos:
        # if algo != base_algo:
        ax.plot(node_counts, data[algo], ".-", label=get_label(algo))
    ax.set_xlabel(xlabel)
    ax.set_ylabel(ylabel, loc="top", rotation="horizontal")
    # ax.legend(loc='best')
    # box = ax.get_position()
    # ax.set_position([box.x0, box.y0 + box.height * 0.1,
    #              box.width, box.height * 0.9])
    # handles, labels = ax.get_legend_handles_labels()
    # labels, handles = zip(*sorted(zip(labels, handles), key=lambda t: t[0]))
    # ax.legend(handles, labels, loc='upper center', bbox_to_anchor=(0.5, -0.15), ncol=2)
    box = ax.get_position()
    ax.set_position([box.x0, box.y0, box.width * 0.8, box.height])
    handles, labels = ax.get_legend_handles_labels()
    # sort both labels and handles by labels
    labels, handles = zip(*sorted(zip(labels, handles), key=lambda t: t[0]))
    ax.legend(handles, labels, loc='center left', bbox_to_anchor=(1, 0.5))
    fig.suptitle(title)
    plt.savefig(figname, bbox_inches="tight")


if __name__ == "__main__":
    if "edge-" in DATADIR[0]:
        t = False
    random_graphs = []
    if not low:
        for file in os.listdir(DATADIR[0]):
            if file.endswith(".csv") and "_" in file:
                random_graphs.append(file)
        nodes = []
        for graph in random_graphs:
            nodes.append(int(graph.split("_")[2]))
        nodes, random_graphs = zip(*sorted(zip(nodes, random_graphs), key=lambda t: t[0]))
    else:
        if t:
            for node_count in range(low, high, interval):
                for x in n:
                    avg_d = node_count * x // 100
                    if avg_d == node_count:
                        avg_d -= 1
                    edge_count = node_count * avg_d // 2
                    if seed:
                        random_graphs.append(f"generated_{seed}_{node_count}_{edge_count}")
                    else:
                        random_graphs.append(f"generated_{node_count}_{edge_count}")
        else:
            for edge_count in range(low, high, interval):
                for x in n:
                    node_count = edge_count // x;
                    if seed:
                        random_graphs.append(f"generated_{seed}_{node_count}_{edge_count}")
                    else:
                        random_graphs.append(f"generated_{node_count}_{edge_count}")

    edge_counts, ops_data, cycles_data, perfs_data, all_algos = read_data(random_graphs)
    # Construct the data frames
    ops_df = pd.DataFrame(ops_data, index=edge_counts)
    cycles_df = pd.DataFrame(cycles_data, index=edge_counts)
    perfs_df = pd.DataFrame(perfs_data, index=edge_counts)
    algos = list(cycles_df.columns)

    # if algos_to_plot:
    #     algos = []
    #     for algo in algos_to_plot.split(","):
    #         if versions:
    #             for vi in range(gather_algos[algo]):
    #                 algos.append(algo + "-" + str(vi))
    #         else:
    #             algos.append(algo)
    # else:
    # algos = list(all_algos)
    print(algos)
    if algos[0][:2] == "ei":
        algo_name = "Edge Iterator"
    elif algos[0][:2] == "f_":
        algo_name = "Forward"
    else:
        algo_name = "Forward Hashed"

    if t:
        xlabel = f"Node Count (Avg degree = |V| * {n}%)"
    else:
        xlabel = f"Node Count (|E| = |V| * {n})"
    #---op count---
    plot(algos, edge_counts, ops_df, n, xlabel, "ops", f"Random Graph: Op Count\n{algo_name}", f"{PLOTDIR}/ops.png")
    #---runtime cycles---
    plot(algos, edge_counts, cycles_df, n, xlabel, "cycles", f"Random Graph: Runtime\n{algo_name}", f"{PLOTDIR}/cycles.png")
    #---perf ops/cycle---
    plot_separate(algos, edge_counts, perfs_df, n, xlabel, "ops/cycle", f"Random Graph: Performance\n{algo_name}", f"{PLOTDIR}/perf", ".png")

    #---speedup---
    if base:
        base_algo = find_base_algo(algos, base)
        if base_algo:
            print("version to compare with:", base_algo)
            speedup_data = calc_speedup(base_algo, edge_counts, cycles_data)
            speedup_df = pd.DataFrame(speedup_data, index=edge_counts)
            plot_speedup(algos, edge_counts, speedup_df, n, xlabel, "x", f"Speedup\n{algo_name}", f"{PLOTDIR}/speedup.png", base_algo)
        else:
            print("no base version")
