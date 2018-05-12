
import argparse
import matplotlib.pyplot as pyplot
import numpy
import scipy.optimize
import json

TITLE_FONTSIZE = 9

def fitting_function_1(xs, a, b, c):
    return [numpy.asscalar(a*numpy.square(x) + b*numpy.log(x)*x + c*x) for x in xs]

def main():
    gendet_ap1 = filelines_to_json('raw/schewe_gendet_ap1.json')
    gendet_ap2 = filelines_to_json('raw/schewe_gendet_ap2.json')
    gendet_ap3 = filelines_to_json('raw/schewe_gendet_ap3.json')
    detnbaut_ap1 = filelines_to_json('raw/schewe_detnbaut_ap1.json')
    detnbaut_ap2 = filelines_to_json('raw/schewe_detnbaut_ap2.json')
    detspot_ap1 = filelines_to_json('raw/schewe_detspot_ap1.json')
    detspot_ap2 = filelines_to_json('raw/schewe_detspot_ap2.json')

    # Plot gendet ap1
    title = 'Schewe10 state reduction on random DPAs with three colors and |Σ|=2.'
    plot_statenum_time(gendet_ap1, 'analysis/schewe_gendet_ap1_time.pdf', title, fitting_function_1)
    plot_statenum_statereduction(gendet_ap1, 'analysis/schewe_gendet_ap1_statereduction.pdf', title)
    plot_sccnum_statereduction(gendet_ap1, 'analysis/schewe_gendet_ap1_sccreduction.pdf', title)

    # Plot gendet: Time comparison for different ap.
    f = pyplot.figure()
    x = [point['original_size'] for point in gendet_ap1]
    y = [point['milliseconds'] for point in gendet_ap1]
    pyplot.plot(x, y, 'b.')
    x = [point['original_size'] for point in gendet_ap2]
    y = [point['milliseconds'] for point in gendet_ap2]
    pyplot.plot(x, y, 'g.')
    x = [point['original_size'] for point in gendet_ap3]
    y = [point['milliseconds'] for point in gendet_ap3]
    pyplot.plot(x, y, 'r.')
    pyplot.title('Time for Schewe construction on a random DPA with 3 priorities and different alphabets.', fontsize=TITLE_FONTSIZE)
    f.savefig('analysis/schewe_gendet_ap_compare_time.pdf')

    # Plot detnbaut ap1
    title = 'Schewe10 state reduction on a DPA with |Σ|=2 that was created by nbautils from an NBA.'
    plot_statenum_time(detnbaut_ap1, 'analysis/schewe_detnbaut_ap1_time.pdf', title)
    plot_statenum_statereduction(detnbaut_ap1, 'analysis/schewe_detnbaut_ap1_statereduction.pdf', title)
    plot_statenum_statereductionrel(detnbaut_ap1, 'analysis/schewe_detnbaut_ap1_statereductionrelative.pdf', title)
    plot_sccnum_statereduction(detnbaut_ap1, 'analysis/schewe_detnbaut_ap1_sccreduction.pdf', title)

    # Plot detnbaut ap2
    title = 'Schewe10 state reduction on a DPA with |Σ|=4 that was created by nbautils from an NBA.'
    plot_statenum_time(detnbaut_ap2, 'analysis/schewe_detnbaut_ap2_time.pdf', title)
    plot_statenum_statereduction(detnbaut_ap2, 'analysis/schewe_detnbaut_ap2_statereduction.pdf', title)

    # Plot detspot ap1
    title = 'Schewe10 state reduction on a DPA with |Σ|=2 that was created by Spot from an NBA.'
    plot_statenum_time(detspot_ap1, 'analysis/schewe_detspot_ap1_time.pdf', title)
    plot_statenum_statereduction(detspot_ap1, 'analysis/schewe_detspot_ap1_statereduction.pdf', title)
    plot_statenum_statereductionrel(detspot_ap1, 'analysis/schewe_detspot_ap1_statereductionrelative.pdf', title)
    plot_sccnum_statereduction(detspot_ap1, 'analysis/schewe_detspot_ap1_sccreduction.pdf', title)

    # Plot detspot ap2
    title = 'Schewe10 state reduction on a DPA with |Σ|=4 that was created by Spot from an NBA.'
    plot_statenum_time(detspot_ap2, 'analysis/schewe_detspot_ap2_time.pdf', title)
    plot_statenum_statereduction(detspot_ap2, 'analysis/schewe_detspot_ap2_statereduction.pdf', title)



# Reads a file of json objects.
def filelines_to_json(filename):
    with open(filename, 'r') as file:
        return [json.loads(line) for line in file.readlines()]

# Plots time used as a function of the number of states.
def plot_statenum_time(data, filename, title, fit_func=None):
    f = pyplot.figure()
    x = [point['original_size'] for point in data]
    y = [point['milliseconds'] for point in data]
    pyplot.plot(x, y, 'b.')
    if fit_func is not None:
        fit_params, covariance = scipy.optimize.curve_fit(fit_func, x, y)
        pyplot.plot(sorted(x), fit_func(sorted(x), *fit_params), color='red')
    pyplot.title(title, fontsize=TITLE_FONTSIZE)
    pyplot.xlabel('Number of states in the original automaton')
    pyplot.ylabel('Time taken in milliseconds')
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



main()
