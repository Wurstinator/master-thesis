
import argparse
import matplotlib.pyplot as pyplot
import numpy
import json


TITLE_FONTSIZE = 9

# Reads a file of json objects.
def filelines_to_json(filename):
    with open(filename, 'r') as file:
        return [json.loads(line) for line in file.readlines()]

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
