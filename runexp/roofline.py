import numpy as np
import csv,argparse
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
import math,json
import seaborn as sns

parser = argparse.ArgumentParser(description='argparse')
parser.add_argument('--datadir', '-d', help='data directory', required=True)
parser.add_argument('--plotdir', '-p', help='plot directory', required=False)
parser.add_argument('--algos', '-a', type=str, help='algos', required=False)
parser.add_argument('--rename', '-m', help='rename the algos', action='store_true')
parser.add_argument('--versions', '-v', type=str, help='versioning', required=False)
args = parser.parse_args()

DATADIR = args.datadir.split(";")
PLOTDIR = args.plotdir
algos_to_plot = args.algos
rename = args.rename
rename = True
if rename:
    with open("rename.json", "r") as f:
        rename_table = json.load(f)
versions = args.versions
if versions:
    versions = versions.split(",")


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

def fractions(x,pos):
    step=1./2.
    if np.isclose((x/step)%(1./step),0.):
        # x is an integer, so just return that
        return '{:.0f}'.format(x)
    else:
        # this returns a latex formatted fraction
        return '$\\frac{{{:2.0f}}}{{{:2.0f}}}$'.format(x/step,1./step)

# https://stackoverflow.com/questions/18780198/how-to-rotate-matplotlib-annotation-to-match-a-line
def label_line(ax, label, x, y, tan, color='black', size=10):
    text = ax.annotate(label, xy=(x, y), xytext=(3,16),
                       textcoords='offset points',
                       size=size, color=color,
                       horizontalalignment='left',
                       verticalalignment='bottom')

    slope_degrees = np.degrees(np.arctan2(tan*0.9, 1))
    text.set_rotation(slope_degrees)
    return text

def print_data(all_graphs,perf,oi):
	print("---Graphs: (nodes, edges)---")
	print(all_graphs)

	print("---P(n), I(n)---")
	for k in perf.keys():
		print(k)
		print("P(n):",["{:.4f}".format(x) for x in perf[k]])
		print("I(n):",["{:.4f}".format(x) for x in oi[k]])

# Uncomment the first part for edge iterator, fh
black_list_graphs = ["generated_1_5500_1512500","generated_1_6000_1800000", "generated_1_4500_1012500", "generated_1_5000_1250000"]
# black_list_graphs = ["generated_1_5500_1512500","generated_1_6000_1800000"]

def get_label(algo):
    algo = algo.split("-")
    ar = rename_table[algo[0]]
    if versions:
        return ar + " " + versions[int(algo[1])]
    else:
        return ar

def read_data(DATADIR):
	perf = {}
	oi = {}
	all_graphs = []
	density = 0
	gather_algos = {}
	all_algos = set()

	for d in DATADIR:
		graphs = {}
		opoi = {}
		exist_algo = {}
		with open(f"{d}/data.csv", 'r') as f:
			reader = csv.DictReader(f)
			for row in reader:
				ar = row["algo"]
				if versions:
					if exist_algo.get(ar, False):
						algo = ar + "-" + str(gather_algos[ar])
					else:
						gather_algos[ar] = gather_algos.get(ar, 0)
						exist_algo[ar] = True
						algo = ar + "-" + str(gather_algos[ar])
				else:
					algo = ar
				all_algos.add(algo)

				if row["graph"] in black_list_graphs:
					continue

				graphs[row["graph"]] = 1
				if algo not in perf:
					perf[algo] = []
				perf[algo].append(float(row["perf"]))
				if algo not in opoi:
					opoi[algo] = {}
				opoi[algo][row["graph"]] = int(row["op"])

		# TODO: need actual data transfer here
		with open(f"{d}/qn.csv", 'r') as f:
			reader = csv.DictReader(f)
			for row in reader:
				if versions:
					algo = row["algo"] + "-" + str(gather_algos[row["algo"]])
				else:
					algo = row["algo"]
				if row["graph"] in black_list_graphs:
					continue

				opoi[algo][row["graph"]] = (opoi[algo][row["graph"]] / float(row["data_transfer"]))
		print("asdfg")
		# print(opoi)
		for algo in opoi.keys():
			if algo not in oi:
				oi[algo] = []
			oi[algo].extend(list(opoi[algo].values()))
		print(algo, opoi[algo])

		for g in graphs.keys():
			all_graphs.append(("{:.02f}".format(int(g.split("_")[-2])/1000),"{:.02f}".format(int(g.split("_")[-1])/1000)))
			with open(f"{d}/{g}.txt", 'r') as f:
				for line in f:
					density = int(line.strip().split(",")[1])

		for k in exist_algo.keys():
			gather_algos[k] += 1

	print_data(all_graphs,perf,oi)
	return perf, oi, all_graphs, density, gather_algos, all_algos

if __name__ == "__main__":
	markers = ["o","^","s","v","+"]
	membw = 22 * 10 ** 9
	base_freq = 3.6 * 10 ** 9
	membw_cycle = membw / base_freq
	print("bytes per cycle:", membw_cycle)
	scalar_pi = 4
	vector_pi = 32
	# f list
	# vector_pi = 6.910896404
	# f count
	# vector_pi = 7.914160744
	# eiv3
	# vector_pi = 1.882352941
	# vector_pi = 1.882352941

	scalar_boundary = scalar_pi/membw_cycle
	vector_boundary = vector_pi/membw_cycle

	perf, oi, all_graphs, density, gather_algos, all_algos = read_data(DATADIR)
	if algos_to_plot:
		algos = []
		for algo in algos_to_plot.split(","):
			if versions:
				for vi in range(gather_algos[algo]):
					algos.append(algo + "-" + str(vi))
			else:
				algos.append(algo)
	else:
		algos = list(all_algos)

	# ------------- plot -------------
	fig, axes = plt.subplots(figsize=(12, 8), dpi=300)
	if algos[0][:2] == "fh":
		xmin = -8
		xmax = 7
		ymin = -12
		ymax = 6
	else:
		xmin = -4
		xmax = 7
		ymin = -4
		ymax = 4
	scalar_color = "black"
	vector_color = "black"

	# ------------- roofline -------------
	# scalar beta
	xs1 = [2**xmin,scalar_boundary]
	ys1 = [2**xmin * membw_cycle,scalar_pi]
	axes.plot(xs1, ys1, linestyle="-", linewidth=1, color="black")

	# scalar pi
	axes.plot([2**xmin,2**xmax],[scalar_pi,scalar_pi], linestyle="-", linewidth=1, color="black")
	axes.annotate(f"\u03C0_scalar ({scalar_pi} flops/cycle)", xy=(2**(xmin+0.1),scalar_pi*1.05), fontsize=10)

	#memory/compute bound
	axes.plot([scalar_boundary,scalar_boundary],[0,scalar_pi], linestyle="-.", linewidth=1, color=scalar_color, label="\u03C0_scalar/\u03B2 ({:.02f} flops/cycle)".format(scalar_boundary))
	# axes.annotate("\u03C0_scalar/\u03B2\n({:.02f} flops/cycle)".format(scalar_boundary), xy=(scalar_boundary*1.1, 2**(ymin+0.1)), color=scalar_color, fontsize=8.5)

	# plt.fill_between([scalar_pi/membw_cycle,2**xmax], 0, [scalar_pi,scalar_pi], facecolor='red', alpha=0.25)

	# vector beta
	xv1 = [2**xmin,2**xmax]
	yv1 = [2**xmin * membw_cycle,2**xmax * membw_cycle]
	axes.plot(xv1, yv1, linestyle="--", linewidth=1, color="black")
	label_line(axes, "\u03B2 ({:.02f} bytes/cycle)".format(membw_cycle), xs1[0], ys1[0], math.log(2**xmax * membw_cycle - 2**xmin * membw_cycle, 2**xmax - 2**xmin))

	# vector pi
	axes.plot([2**xmin,2**xmax],[vector_pi,vector_pi], linestyle="--", linewidth=1, color="black")
	axes.annotate("\u03C0_vector ({:.02f} flops/cycle)".format(vector_pi), xy=(2**(xmin+0.1),vector_pi*1.05), fontsize=10)

	#memory/compute bound
	axes.plot([vector_boundary,vector_boundary],[0,vector_pi], linestyle="dotted", linewidth=1, color=vector_color, label="\u03C0_vector/\u03B2 ({:.02f} flops/cycle)".format(vector_boundary))
	# axes.annotate("\u03C0_vector/\u03B2\n({:.02f} flops/cycle)".format(vector_boundary), xy=(vector_boundary*1.1, 2**(ymin+0.1)), color=vector_color,fontsize=8.5)

	# plt.fill_between([vector_pi/membw_cycle,2**xmax], 0, [vector_pi,vector_pi], facecolor='green', alpha=0.25)


	# ------------- algo points -------------
	for i,algo in enumerate(algos):
		# markeredgecolor="black",markerfacecolor="none"
		algo_label = get_label(algo)
		if "vec" in algo_label:
			axes.plot(oi[algo], perf[algo], marker="v", markersize=6, markeredgecolor="white", markeredgewidth=0.2, linewidth=1.5, label=algo_label)
		else:
			axes.plot(oi[algo], perf[algo], marker="o", markersize=5, markeredgecolor="white", markeredgewidth=0.2, linewidth=1.5, label=algo_label)
		# axes.annotate(algo, xy=(oi[algo][-1]*0.8,perf[algo][-1]*0.7), fontsize=8.5)


	# ------------- format -------------
	box = axes.get_position()
	axes.set_position([box.x0, box.y0, box.width * 0.8, box.height])
	handles, labels = axes.get_legend_handles_labels()
	# sort both labels and handles by labels
	labels, handles = zip(*sorted(zip(labels, handles), key=lambda t: t[0]))
	axes.legend(handles, labels, loc='center left', bbox_to_anchor=(1, 0.5))
	if algos[0][:2] == "ei":
		algo_name = "Edge Iterator"
	elif algos[0][:2] == "f_":
		algo_name = "Forward"
	else:
		algo_name = "Forward Hashed"
	plt.title(f"Roofline Plot of Comparison Operations\n{algo_name}", fontsize=16)
	plt.xlabel(f"I(n) = W/Q [flops/byte]\nnodes: {all_graphs[0][0]}k to {all_graphs[-1][0]}k, density {density}%", fontsize=11, labelpad=10)
	plt.ylabel("P = W/T [flops/cycle]", fontsize=11, rotation='horizontal', horizontalalignment='left', y=1.03)

	ax = plt.gca()
	ax.tick_params(axis='both', which='major', labelsize=11)
	ax.set_xscale('log')
	ax.set_xticks([2**x for x in range(xmin,xmax)])
	ax.set_yscale('log')
	ax.set_yticks([2**y for y in range(ymin,ymax)])
	ax.get_xaxis().set_major_formatter(
		ticker.FixedFormatter(
			['$\\frac{{{:2.0f}}}{{{:2.0f}}}$'.format(1,2**x) for x in range(-xmin,0,-1)]
			+ [2 ** x for x in range(0,xmax)]))
	ax.get_yaxis().set_major_formatter(
		ticker.FixedFormatter(
			['$\\frac{{{:2.0f}}}{{{:2.0f}}}$'.format(1,2**y) for y in range(-ymin,0,-1)]
			+ [2 ** y for y in range(0,ymax)]))

	plt.xlim(xmin=2**xmin, xmax=2**xmax)
	plt.ylim(ymin=2**ymin, ymax=2**ymax)
	# plt.grid(color="#FFFFFF")
	# ax.set_facecolor('#EFECEF')

	# plt.show()
	if algos_to_plot:
		plt.savefig(f"{PLOTDIR}/roofline_{algos_to_plot}.png")
	else:
		plt.savefig(f"{PLOTDIR}/roofline_{algo_name}.png")
