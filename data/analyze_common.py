
from enum import Enum

import matplotlib.backends.backend_pdf as backend_pdf

from common_plots import *

class DataType(Enum):
    GendetAP1 = 0,
    GendetAP2 = 1,
    GendetAP3 = 2,
    DetNbautAP1 = 3,
    DetNbautAP2 = 4,
    DetSpotAP1 = 5,
    DetSpotAP2 = 6,
    MaxMichelle = 7

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
        elif self == DataType.MaxMichelle:
            return 'maxmichelle'

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
        elif self == DataType.MaxMichelle:
            suffix = ' state reduction on the Max Michelle DBA.'
        return alg_name + suffix



# Reads all "relevant" files as JSON.
def read_prefix_to_json(input_prefix):
    files = map(lambda d: 'raw/' + input_prefix + d.make_filename() + '.json', list(DataType))
    data = map(lambda f: read_json_to_map(f, 'filename'), files)
    return dict(zip(list(DataType), data))


def analysis_sub(outfile, title, automata, data, rawstats):
    select_statenum = (lambda f: rawstats[f]['states'])
    select_statered = (lambda f: rawstats[f]['states'] - data[f]['new_size'])
    select_statered_relative = (lambda f: (rawstats[f]['states'] - data[f]['new_size']) / rawstats[f]['states'])
    select_sccnum = (lambda f: rawstats[f]['sccs'])
    select_prionum = (lambda f: rawstats[f]['priorities'])
    select_time_seconds = (lambda f: data[f]['milliseconds'] / 1000)

    alabel_automata = 'Number of automata'
    alabel_statenum = 'Number of states in the original automaton'
    alabel_statered = 'Number of removed states'
    alabel_statered_relative = 'Relative number of removed states'
    alabel_sccnum = 'Number of SCCs in the original automaton'
    alabel_prionum = 'Number of priorities in the original automaton'
    alabel_time_seconds = 'Time taken in seconds'

    pdf = backend_pdf.PdfPages(outfile)
    pdf.savefig(plot_points(automata, select_statenum, select_time_seconds, title, alabel_statenum, alabel_time_seconds))
    pdf.savefig(plot_points(automata, select_statenum, select_statered, title, alabel_statenum, alabel_statered))
    pdf.savefig(plot_points(automata, select_statenum, select_statered_relative, title, alabel_statenum, alabel_statered_relative))
    pdf.savefig(plot_points(automata, select_sccnum, select_statered_relative, title, alabel_sccnum, alabel_statered_relative))
    pdf.savefig(plot_points(automata, select_prionum, select_statered_relative, title, alabel_prionum, alabel_statered_relative))
    pdf.savefig(plot_histogram(automata, select_statered_relative, title, alabel_statered_relative, alabel_automata))
    pdf.close()

# Does a standard data analysis.
def general_analysis_v1(name, output_prefix, dataset, rawstats):
    output_folder = 'analysis/' + output_prefix

    for type, data in dataset.items():
        title = type.make_figure_title(name)
        automata = set(data.keys()).intersection(rawstats.keys())
        analysis_sub(output_folder + type.make_filename() + '.pdf', title, automata, data, rawstats)

    # Plot gendet: Time comparison for different ap.
    #title = 'Time for ' + name + ' construction on a random DPA with 3 priorities and different alphabets.'
    #f = plot_time_comparison([dataset[DataType.GendetAP1], dataset[DataType.GendetAP2], dataset[DataType.GendetAP3]],
    #                     title=title, labels=['|Σ|=2', '|Σ|=4', '|Σ|=8'])
    #f.savefig(output_folder + 'gendet_ap_compare_time.pdf')
