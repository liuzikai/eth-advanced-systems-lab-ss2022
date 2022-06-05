import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from scipy import stats
import seaborn as sns
import matplotlib as mpl
import os, argparse, sys
import json, csv, re


parser = argparse.ArgumentParser(description='argparse')
parser.add_argument('--graph_metadata', '-m', help='graph metadata', required=True)
parser.add_argument('--datadir', '-d', help='data directory', required=True)
parser.add_argument('--plotdir', '-p', help='plot directory', required=True)
parser.add_argument('--commit_hash', '-c', help='commit hash', required=True)
parser.add_argument('--config', '-f', help='warmups-runs-phases', required=True)
parser.add_argument('--number', '-n', help='nth exp of the config', required=True)
parser.add_argument('--graphs', '-g', help='realworld graph types', default="GERMAN_ROAD_NETWORK,COMP_SCIENCE_AUTHORS,GOOGLE_CONTEST,HEP_LITERATURE,ROUTER_NETWORK,WWW_NOTRE_DAME,US_PATENTS", required=False)
parser.add_argument('--base', '-b', type=str, help='version to compare with', required=False)
args = parser.parse_args()


DATADIR = args.datadir
PLOTDIR = args.plotdir
commit_hash = args.commit_hash
config = args.config
number = args.number
graphs = args.graphs.split(",")
base = args.base


def get_datafile_names():
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

    datafiles = []
    for graph in gs:
        datafiles.append(os.path.join(DATADIR, f"{commit_hash}-{graph}", config, number, f"{graph}.csv"))

    return datafiles, gs, gne

def read_data(datafiles):
    ops_data = []
    cycles_data = []
    perfs_data = []
    for data in datafiles:
        with open(data, 'r') as f:
            reader = csv.DictReader(f)
            ops = {}
            cycles = {}
            perfs = {}
            for row in reader:
                algo = row["algorithm"]
                op = int(row["ops"])
                del row["algorithm"]
                del row["ops"]
                cycle = np.median([int(x) for x in row.values()])
                perf = op / cycle
                ops[algo] = op
                cycles[algo] = cycle
                perfs[algo] = perf

            ops_data.append(ops)
            cycles_data.append(cycles)
            perfs_data.append(perfs)
    return ops_data, cycles_data, perfs_data

def find_base_algo(algos, base):
    base_algo = ""
    for algo in algos:
        pattern = r'.*_'+base
        is_base = re.match(pattern, algo)
        if is_base:
            base_algo = algo
            break
    return base_algo

def calc_speedup(base_algo, gs, cycles_data):
    speedup_data = []
    for i in range(len(gs)):
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

def plot_separate(x, width, algos, data, ylabel, gne, title, figname, format=".png", yscale="linear"):
    for i in range(len(algos)):
        fig, ax = plt.subplots(figsize=(12, 6))
        rects=[]

        rects.append(ax.bar(x, data[algos[i]].values.tolist(), width, label=algos[i]))

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
        plt.savefig(f"{figname}-{algos[i]}{format}", bbox_inches="tight")

def plot_speedup(x, width, algos, data, ylabel, gne, title, figname, base_algo, yscale="linear"):
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



if __name__ and "__main__":
    datafiles, gs, gne = get_datafile_names()

    ops_data, cycles_data, perfs_data = read_data(datafiles)

    # Construct the data frames
    ops_df = pd.DataFrame(ops_data, index=gs)
    cycles_df = pd.DataFrame(cycles_data, index=gs)
    perf_df = pd.DataFrame(perfs_data, index=gs)

    algos = list(cycles_df.columns)
    print(algos)


    #---x axis---
    x = np.arange(len(gs))  # the label locations
    width = len(gs)/40  # the width of the bars

    #---op count---
    plot(x, width, algos, ops_df, "ops", gne, 'Op Count', f"{PLOTDIR}/large_graphs_ops.png", yscale="log")

    #---runtime cycles---
    plot(x, width, algos, cycles_df, "cycles", gne, 'Runtime', f"{PLOTDIR}/large_graphs_cycles.png", yscale="log")

    #---perf ops/cycle---
    plot_separate(x, width, algos, perf_df, "ops/cycle", gne, 'Performance', f"{PLOTDIR}/large_graphs_perf", ".png", yscale="log")

    #---speedup---
    if base:
        base_algo = find_base_algo(algos, base)
        if base_algo:
            print("version to compare with:", base_algo)
            speedup_data = calc_speedup(base_algo, gs, cycles_data)
            speedup_df = pd.DataFrame(speedup_data, index=gs)

            non_base_algos = []
            for algo in algos:
                if algo != base_algo:
                    non_base_algos.append(algo)
            plot_speedup(x, width, non_base_algos, speedup_df, "ops", gne, 'Speedup', f"{PLOTDIR}/speedup.png", base_algo)
        else:
            print("no base version")
