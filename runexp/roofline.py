import numpy as np
import csv,argparse
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
import math

parser = argparse.ArgumentParser(description='argparse')
parser.add_argument('--datadir', '-d', help='data directory', required=True)
parser.add_argument('--plotdir', '-p', help='plot directory', required=False)
parser.add_argument('--algos', '-a', type=str, help='version to compare with', required=False)
args = parser.parse_args()

DATADIR = args.datadir.split(";")
PLOTDIR = args.plotdir
algos = args.algos
if algos:
	algos = algos.split(",")

def fractions(x,pos):
    step=1./2.
    if np.isclose((x/step)%(1./step),0.):
        # x is an integer, so just return that
        return '{:.0f}'.format(x)
    else:
        # this returns a latex formatted fraction
        return '$\\frac{{{:2.0f}}}{{{:2.0f}}}$'.format(x/step,1./step)

# https://stackoverflow.com/questions/18780198/how-to-rotate-matplotlib-annotation-to-match-a-line
def label_line(ax, label, x, y, tan, color='black', size=8.5):
    text = ax.annotate(label, xy=(x, y), xytext=(2,15),
                       textcoords='offset points',
                       size=size, color=color,
                       horizontalalignment='left',
                       verticalalignment='bottom')

    slope_degrees = np.degrees(np.arctan2(tan, 1))
    text.set_rotation(slope_degrees)
    return text

def print_data(all_graphs,perf,oi):
	print("---Graphs: (nodes, edges)---")
	print(all_graphs)

	print("---P(n), I(n)---")
	for k in perf.keys():
		print("P(n):",["{:.4f}".format(x) for x in perf[k]])
		print("I(n):",["{:.4f}".format(x) for x in oi[k]])

# Uncomment the second part for edge iterator
black_list_graphs = ["generated_1_5500_1512500","generated_1_6000_1800000"] #, "generated_1_4500_1012500", "generated_1_5000_1250000"]
black_list_algos = ["f_v2", "f_v3","f_v4","f_v5",]

def read_data(DATADIR):
	perf = {}
	oi = {}
	all_graphs = []
	density = 0

	for d in DATADIR:
		graphs = {}
		opoi = {}

		with open(f"{d}/data.csv", 'r') as f:
			reader = csv.DictReader(f)
			for row in reader:
				if row["graph"] in black_list_graphs:
					continue
				if row["algo"] in black_list_algos:
					continue
				graphs[row["graph"]] = 1
				if row["algo"] not in perf:
					perf[row["algo"]] = []
				perf[row["algo"]].append(float(row["perf"]))
				if row["algo"] not in opoi:
					opoi[row["algo"]] = {}
				opoi[row["algo"]][row["graph"]] = int(row["op"])

		# TODO: need actual data transfer here
		with open(f"{d}/qn.csv", 'r') as f:
			reader = csv.DictReader(f)
			for row in reader:
				if row["graph"] in black_list_graphs:
					continue
				if row["algo"] in black_list_algos:
					continue
				opoi[row["algo"]][row["graph"]] = opoi[row["algo"]][row["graph"]] / float(row["data_transfer"])
		print("asdfg")
		print(opoi)
		for algo in opoi.keys():
			if algo not in oi:
				oi[algo] = []
			oi[algo].extend(list(opoi[algo].values()))

		for g in graphs.keys():
			all_graphs.append(("{:.02f}".format(int(g.split("_")[-2])/1000),"{:.02f}".format(int(g.split("_")[-1])/1000)))
			with open(f"{d}/{g}.txt", 'r') as f:
				for line in f:
					density = int(line.strip().split(",")[1])

	print_data(all_graphs,perf,oi)

	return perf, oi, all_graphs, density

if __name__ == "__main__":
	markers = ["o","^","s","v","+"]
	membw = 22 * 10 ** 9
	base_freq = 3.6 * 10 ** 9
	membw_cycle = membw / base_freq
	print("bytes per cycle:", membw_cycle)
	scalar_pi = 4
	vector_pi = 32
	scalar_boundary = scalar_pi/membw_cycle
	vector_boundary = vector_pi/membw_cycle

	perf, oi, all_graphs, density = read_data(DATADIR)
	if not algos:
		algos = list(perf.keys())
		print(algos)
	# ------------- plot -------------
	fig, axes = plt.subplots(figsize=(10, 8), dpi=100)
	xmin = -6
	xmax = 9
	ymin = -4
	ymax = 6
	scalar_color = "red"
	vector_color = "green"

	# ------------- roofline -------------
	# scalar beta
	xs1 = [2**xmin,scalar_boundary]
	ys1 = [2**xmin * membw_cycle,scalar_pi]
	axes.plot(xs1, ys1, linestyle="-", linewidth=1, color="black")

	# scalar pi
	axes.plot([2**xmin,2**xmax],[scalar_pi,scalar_pi], linestyle="-", linewidth=1, color="black")
	axes.annotate(f"\u03C0_scalar ({scalar_pi} flops/cycle)", xy=(2**(xmin+0.1),scalar_pi+0.5), fontsize=8.5)

	#memory/compute bound
	axes.plot([scalar_boundary,scalar_boundary],[0,scalar_pi], linestyle="dotted", linewidth=1, color=scalar_color)
	axes.annotate("\u03C0_scalar/\u03B2\n({:.02f} flops/cycle)".format(scalar_boundary), xy=(scalar_boundary*1.1, 2**(ymin+0.1)), color=scalar_color, fontsize=8.5)

	# plt.fill_between([scalar_pi/membw_cycle,2**xmax], 0, [scalar_pi,scalar_pi], facecolor='red', alpha=0.25)

	# vector beta
	xv1 = [2**xmin,2**xmax]
	yv1 = [2**xmin * membw_cycle,2**xmax * membw_cycle]
	axes.plot(xv1, yv1, linestyle="--", linewidth=1, color="black")
	label_line(axes, "\u03B2 ({:.02f} bytes/cycle)".format(membw_cycle), xs1[-1], ys1[-1], math.log(2**xmax * membw_cycle - 2**xmin * membw_cycle, 2**xmax - 2**xmin))

	# vector pi
	axes.plot([2**xmin,2**xmax],[vector_pi,vector_pi], linestyle="--", linewidth=1, color="black")
	axes.annotate(f"\u03C0_vector ({vector_pi} flops/cycle)", xy=(2**(xmin+0.1),vector_pi+4), fontsize=8.5)

	#memory/compute bound
	axes.plot([vector_boundary,vector_boundary],[0,vector_pi], linestyle="dotted", linewidth=1, color=vector_color)
	axes.annotate("\u03C0_vector/\u03B2\n({:.02f} flops/cycle)".format(vector_boundary), xy=(vector_boundary*1.1, 2**(ymin+0.1)), color=vector_color,fontsize=8.5)

	# plt.fill_between([vector_pi/membw_cycle,2**xmax], 0, [vector_pi,vector_pi], facecolor='green', alpha=0.25)
	

	# ------------- algo points -------------
	for i,algo in enumerate(algos):
		# markeredgecolor="black",markerfacecolor="none"
		axes.plot(oi[algo], perf[algo], marker="o",markersize=3, linewidth=1)
		axes.annotate(algo, xy=(oi[algo][-1]*0.8,perf[algo][-1]*0.7), fontsize=8.5)


	# ------------- format -------------
	plt.title("Roofline Plot of Comparison Operations", fontsize=16)
	plt.xlabel(f"I upper bound = W/Q [flops/byte]\nnodes: {all_graphs[0][0]}k to {all_graphs[-1][0]}k, density {density}%", fontsize=11, labelpad=10)
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

	# plt.legend(loc='upper left')
	plt.xlim(xmin=2**xmin, xmax=2**xmax)
	plt.ylim(ymin=2**ymin, ymax=2**ymax)
	plt.grid(color="#dddddd")

	plt.show()