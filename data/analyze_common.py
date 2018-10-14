
import matplotlib.pyplot as pyplot
import numpy
import json


TITLE_FONTSIZE = 9

# Reads a file of json objects.
def filelines_to_json(filename):
    with open(filename, 'r') as file:
        return [json.loads(line) for line in file.readlines()]

# Reads all "relevant" files as JSON.
def read_prefix_to_json(input_prefix):
    suffixes = ['gendet_ap1.json', 'gendet_ap2.json', 'gendet_ap3.json', 'detnbaut_ap1.json', 'detnbaut_ap2.json', 'detspot_ap1.json', 'detspot_ap2.json']
    return map(lambda suf: filelines_to_json('raw/' + input_prefix + suf), suffixes)

# Plots time used as a function of the number of states.
def plot_statenum_time(data, filename, title, fit_func=None):
    f = pyplot.figure()
    x = [point['original_size'] for point in data]
    y = [point['milliseconds'] / 1000 for point in data]
    pyplot.plot(x, y, 'b.')
    if fit_func is not None:
        fit_params, covariance = scipy.optimize.curve_fit(fit_func, x, y)
        pyplot.plot(sorted(x), fit_func(sorted(x), *fit_params), color='red')
    pyplot.title(title, fontsize=TITLE_FONTSIZE)
    pyplot.xlabel('Number of states in the original automaton')
    pyplot.ylabel('Time taken in seconds')
    f.savefig(filename)

# Plots state reduction as a function of the number of states.
def plot_statenum_statereduction(data, filename, title):
    f = pyplot.figure()
    x = [point['original_size'] for point in data]
    y = [point['original_size'] - point['new_size'] for point in data]
    pyplot.plot(x, y, 'b.')
    pyplot.title(title, fontsize=TITLE_FONTSIZE)
    pyplot.xlabel('Number of states in the original automaton')
    pyplot.ylabel('Number of removed states')
    f.savefig(filename)

# Plots state reduction as a function of the number of SCCs.
def plot_sccnum_statereduction(data, filename, title):
    f = pyplot.figure()
    x = [point['original_number_of_sccs'] for point in data]
    y = [point['original_size'] - point['new_size'] for point in data]
    pyplot.plot(x, y, 'b.')
    pyplot.title(title, fontsize=TITLE_FONTSIZE)
    pyplot.xlabel('Number of SCCs in the original automaton')
    pyplot.ylabel('Number of removed states')
    f.savefig(filename)

# Plots relative state reduction as a function of the number of states.
def plot_statenum_statereductionrel(data, filename, title):
    f = pyplot.figure()
    x = [point['original_size'] for point in data]
    y = [(point['original_size'] - point['new_size'])/point['original_size'] for point in data]
    pyplot.plot(x, y, 'b.')
    pyplot.title(title, fontsize=TITLE_FONTSIZE)
    pyplot.xlabel('Number of states in the original automaton')
    pyplot.ylabel('Relative number of removed states')
    f.savefig(filename)

# Plots time used as a function of the number of states. Multiple sets are drawn here in different colors.
def plot_time_comparison(datapoints, title, filename, labels=None):
    f = pyplot.figure()
    nolabels = labels is None
    if nolabels:
        labels = ['' for _ in datapoints]
    for data, label in zip(datapoints, labels):
        x = [point['original_size'] for point in data]
        y = [point['milliseconds']/1000 for point in data]
        pyplot.plot(x, y, '.', label=label)
    pyplot.xlabel('Number of states in the original automaton')
    pyplot.ylabel('Time taken in seconds')
    pyplot.title(title, fontsize=TITLE_FONTSIZE)
    if not nolabels:
        pyplot.legend(loc='upper left')
    f.savefig(filename)


def fitting_function_1(xs, a, b, c):
    return [numpy.asscalar(a*numpy.square(x) + b*numpy.log(x)*x + c*x) for x in xs]

# Does a standard data analysis that consists of:
# - Time for AP1 & AP2.
# - State reduction (absolute) for AP1 & AP2.
# - State reduction (relative) for AP1.
# - State reduction to number of SCCs for AP1.
# - Time comparison between AP1, AP2, and AP3; only for gendet.
def general_analysis_v1(name, input_prefix, output_prefix):
    gendet_ap1, gendet_ap2, gendet_ap3, detnbaut_ap1, detnbaut_ap2, detspot_ap1, detspot_ap2 = read_prefix_to_json(input_prefix)

    # Plot gendet ap1
    title = name + ' state reduction on random DPAs with three colors and |Σ|=2.'
    plot_statenum_time(gendet_ap1, 'analysis/' + output_prefix + 'gendet_ap1_time.pdf', title)#, fitting_function_1)
    plot_statenum_statereduction(gendet_ap1, 'analysis/' + output_prefix + 'gendet_ap1_statereduction.pdf', title)
    plot_sccnum_statereduction(gendet_ap1, 'analysis/' + output_prefix + 'gendet_ap1_sccreduction.pdf', title)

    # Plot gendet: Time comparison for different ap.
    title = 'Time for ' + name + ' construction on a random DPA with 3 priorities and different alphabets.'
    plot_time_comparison([gendet_ap1, gendet_ap2, gendet_ap3],
                         title=title,
                         filename='analysis/' + output_prefix + 'gendet_ap_compare_time.pdf',
                         labels=['|Σ|=2', '|Σ|=4', '|Σ|=8'])

    # Plot detnbaut ap1
    title = name + ' state reduction on a DPA with |Σ|=2 that was created by nbautils from an NBA.'
    plot_statenum_time(detnbaut_ap1, 'analysis/' + output_prefix + 'detnbaut_ap1_time.pdf', title)
    plot_statenum_statereduction(detnbaut_ap1, 'analysis/' + output_prefix + 'detnbaut_ap1_statereduction.pdf', title)
    plot_statenum_statereductionrel(detnbaut_ap1, 'analysis/' + output_prefix + 'detnbaut_ap1_statereductionrelative.pdf', title)
    plot_sccnum_statereduction(detnbaut_ap1, 'analysis/' + output_prefix + 'detnbaut_ap1_sccreduction.pdf', title)

    # Plot detnbaut ap2
    title = name + ' state reduction on a DPA with |Σ|=4 that was created by nbautils from an NBA.'
    plot_statenum_time(detnbaut_ap2, 'analysis/' + output_prefix + 'detnbaut_ap2_time.pdf', title)
    plot_statenum_statereduction(detnbaut_ap2, 'analysis/' + output_prefix + 'detnbaut_ap2_statereduction.pdf', title)

    # Plot detspot ap1
    title = name + ' state reduction on a DPA with |Σ|=2 that was created by Spot from an NBA.'
    plot_statenum_time(detspot_ap1, 'analysis/' + output_prefix + 'detspot_ap1_time.pdf', title)
    plot_statenum_statereduction(detspot_ap1, 'analysis/' + output_prefix + 'detspot_ap1_statereduction.pdf', title)
    plot_statenum_statereductionrel(detspot_ap1, 'analysis/' + output_prefix + 'detspot_ap1_statereductionrelative.pdf', title)
    plot_sccnum_statereduction(detspot_ap1, 'analysis/' + output_prefix + 'detspot_ap1_sccreduction.pdf', title)

    # Plot detspot ap2
    title = name + ' state reduction on a DPA with |Σ|=4 that was created by Spot from an NBA.'
    plot_statenum_time(detspot_ap2, 'analysis/' + output_prefix + 'detspot_ap2_time.pdf', title)
    plot_statenum_statereduction(detspot_ap2, 'analysis/' + output_prefix + 'detspot_ap2_statereduction.pdf', title)
