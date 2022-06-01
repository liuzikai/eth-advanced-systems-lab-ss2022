import subprocess, argparse, math

parser = argparse.ArgumentParser(description='argparse')
parser.add_argument('--gt', '-g', type=str, help='graph metadata', required=True)
parser.add_argument('--num_nodes', '-n', type=str, help='data directory', required=True)
parser.add_argument('--num_edges', '-e', type=str, help='plot directory', required=True)
parser.add_argument('--shuffle_edges', '-f', help='commit hash', action='store_true')
parser.add_argument('--density', '-d', type=str, help='warmups-runs-phases', required=True)
parser.add_argument('--output', '-o', type=str, help='nth exp of the config', required=True)
parser.add_argument('--seed', '-s', type=str, help='realworld graph types', required=False)
args = parser.parse_args()


GRAPHTYPE = args.gt
i = args.num_nodes
EDGE = args.num_edges
shuffle_edges = args.shuffle_edges
density = args.density
OUTPUT = args.output
SEED = args.seed

h = str(int(math.sqrt(int(i))))
print(i, h)

if shuffle_edges:
    if SEED:
        subprocess.run(["./graph_generation", "-gt", GRAPHTYPE, "-num_nodes", i, "-num_edges", EDGE, "-seed", SEED, "-shuffle_edges", "-density", density, "-h", h, "-o", OUTPUT])
    else:
        subprocess.run(["./graph_generation", "-gt", GRAPHTYPE, "-num_nodes", i, "-num_edges", EDGE, "-shuffle_edges", "-density", density, "-h", h, "-o", OUTPUT])
else:
    print("NOT shuffling edges!")
    exit(1)
