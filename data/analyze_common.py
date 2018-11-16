
import matplotlib.pyplot as pyplot
import matplotlib.backends.backend_pdf as backend_pdf
import numpy
import json
from enum import Enum


TITLE_FONTSIZE = 9

class DataType(Enum):
    GendetAP1 = 0,
    GendetAP2 = 1,
    GendetAP3 = 2,
    DetNbautAP1 = 3,
    DetNbautAP2 = 4,
    DetSpotAP1 = 5,
    DetSpotAP2 = 6

    # From a given data type, returns the filename
    def make_filename(self):
        if self == DataType.GendetAP1:
            return 'gendet_ap1'
        elif self == DataType.GendetAP2:
            return 'gendet_ap2'
        elif self == DataType.GendetAP3:
            return 'gendet_ap3'
        elif self == DataType.DetNbautAP1:
            return 'detnbaut_ap1'
        elif self == DataType.DetNbautAP2:
            return 'detnbaut_ap2'
        elif self == DataType.DetSpotAP1:
            return 'detspot_ap1'
        elif self == DataType.DetSpotAP2:
            return 'detspot_ap2'

    # From a given data type, returns the figure title
    def make_figure_title(self, alg_name):
        if self == DataType.GendetAP1:
            suffix = ' state reduction on random DPAs with three colors and |Σ|=2.'
        elif self == DataType.GendetAP2:
            suffix = ' state reduction on random DPAs with three colors and |Σ|=4.'
        elif self == DataType.GendetAP3:
            suffix = ' state reduction on random DPAs with three colors and |Σ|=8.'
        elif self == DataType.DetNbautAP1:
            suffix = ' state reduction on a DPA with |Σ|=2 that was created by nbautils from an NBA.'
        elif self == DataType.DetNbautAP2:
            suffix = ' state reduction on a DPA with |Σ|=4 that was created by nbautils from an NBA.'
        elif self == DataType.DetSpotAP1:
            suffix = ' state reduction on a DPA with |Σ|=2 that was created by Spot from an NBA.'
        elif self == DataType.DetSpotAP2:
            suffix = ' state reduction on a DPA with |Σ|=4 that was created by Spot from an NBA.'
        return alg_name + suffix



# Reads a file of json objects.
def filelines_to_json(filename):
    with open(filename, 'r') as file:
        return [json.loads(line) for line in file.readlines()]

# Reads all "relevant" files as JSON.
def read_prefix_to_json(input_prefix):
    files = map(lambda d: 'raw/' + input_prefix + d.make_filename() + '.json', list(DataType))
    data = map(lambda f: filelines_to_json(f), files)
    return dict(zip(list(DataType), data))

# Plots time used as a function of the number of states.
def plot_statenum_time(data, title, fit_func=None):
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
    return f

# Plots state reduction as a function of the number of states.
def plot_statenum_statereduction(data, title):
    f = pyplot.figure()
    x = [point['original_size'] for point in data]
    y = [point['original_size'] - point['new_size'] for point in data]
    pyplot.plot(x, y, 'b.')
    pyplot.title(title, fontsize=TITLE_FONTSIZE)
    pyplot.xlabel('Number of states in the original automaton')
    pyplot.ylabel('Number of removed states')
    return f

# Plots state reduction as a function of the number of SCCs.
def plot_sccnum_statereduction(data, title):
    f = pyplot.figure()
    x = [point['original_number_of_sccs'] for point in data]
    y = [point['original_size'] - point['new_size'] for point in data]
    pyplot.plot(x, y, 'b.')
    pyplot.title(title, fontsize=TITLE_FONTSIZE)
    pyplot.xlabel('Number of SCCs in the original automaton')
    pyplot.ylabel('Number of removed states')
    return f

# Plots state reduction as a function of the number of priorities.
def plot_prionum_statereduction(data, title):
    f = pyplot.figure()
    x = [point['original_number_of_colors'] for point in data]
    y = [point['original_size'] - point['new_size'] for point in data]
    pyplot.plot(x, y, 'b.')
    pyplot.title(title, fontsize=TITLE_FONTSIZE)
    pyplot.xlabel('Number of Priorities in the original automaton')
    pyplot.ylabel('Number of removed states')
    return f

# Plots relative state reduction as a function of the number of states.
def plot_statenum_statereductionrel(data, title):
    f = pyplot.figure()
    x = [point['original_size'] for point in data]
    y = [(point['original_size'] - point['new_size'])/point['original_size'] for point in data]
    pyplot.plot(x, y, 'b.')
    pyplot.title(title, fontsize=TITLE_FONTSIZE)
    pyplot.xlabel('Number of states in the original automaton')
    pyplot.ylabel('Relative number of removed states')
    return f

# Plots time used as a function of the number of states. Multiple sets are drawn here in different colors.
def plot_time_comparison(datapoints, title, labels=None):
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
    return f

# Plots a histogram of the number of automata w.r.t relative state reduction.
def plot_histogram_statereduction(data, title):
    f = pyplot.figure()
    x = [(point['original_size'] - point['new_size'])/point['original_size'] for point in data]
    pyplot.hist(x, bins=10)
    pyplot.title(title, fontsize=TITLE_FONTSIZE)
    pyplot.xlabel('Relative number of removed states')
    pyplot.ylabel('Number of automata')
    return f

def fitting_function_1(xs, a, b, c):
    return [numpy.asscalar(a*numpy.square(x) + b*numpy.log(x)*x + c*x) for x in xs]

# Plots several figures and saves them all to a single PDF.
def plot_dataset_to_pdf(data, filename, title):
    pdf = backend_pdf.PdfPages(filename)
    pdf.savefig(plot_statenum_time(data, title))
    pdf.savefig(plot_statenum_statereduction(data, title))
    pdf.savefig(plot_statenum_statereductionrel(data, title))
    pdf.savefig(plot_sccnum_statereduction(data, title))
    pdf.savefig(plot_prionum_statereduction(data, title))
    pdf.savefig(plot_histogram_statereduction(data, title))
    pdf.close()


# Does a standard data analysis that consists of:
# - Time for AP1 & AP2.
# - State reduction (absolute) for AP1 & AP2.
# - State reduction (relative) for AP1.
# - State reduction to number of SCCs for AP1.
# - Time comparison between AP1, AP2, and AP3; only for gendet.
def general_analysis_v1(name, input_prefix, output_prefix):
    data_dict = read_prefix_to_json(input_prefix)
    output_folder = 'analysis/' + output_prefix

    for type, data in data_dict.items():
        plot_dataset_to_pdf(data, output_folder + type.make_filename() + '.pdf', type.make_figure_title(name))


    # Plot gendet: Time comparison for different ap.
    #title = 'Time for ' + name + ' construction on a random DPA with 3 priorities and different alphabets.'
    #plot_time_comparison([gendet_ap1, gendet_ap2, gendet_ap3],
    #                     title=title,
    #                     filename='analysis/' + output_prefix + 'gendet_ap_compare_time.pdf',
    #                     labels=['|Σ|=2', '|Σ|=4', '|Σ|=8'])
