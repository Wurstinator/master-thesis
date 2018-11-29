
import matplotlib.pyplot as pyplot
import matplotlib.backends.backend_pdf as backend_pdf
import numpy
import json
from enum import Enum
from common_plots import *


TITLE_FONTSIZE = 9


# Plots a histogram of a json attribute.
def plot_histogram_priorities(data):
    f = pyplot.figure()
    x = [point['priorities'] for point in data]
    counts, bins, patches = pyplot.hist(x, bins=range(1, max(x)))
    pyplot.title('Histogram of the number of priorities of all automata.', fontsize=TITLE_FONTSIZE)
    pyplot.xlabel('Priorities')
    pyplot.ylabel('Number of automata')
    return f


def plot_rawstats(data, filename, title):
    select_statenum = (lambda f: data[f]['states'])
    select_prionum = (lambda f: data[f]['priorities'])
    select_sccnum = (lambda f: data[f]['sccs'])
    select_ntrivlang = (lambda f: data[f]['nontrivial_language_classes'])
    select_langclas_avgsize = (lambda f: data[f]['average_language_class_size'])

    alabel_automata = 'Number of automata'
    alabel_statenum = 'Number of states'
    alabel_prionum = 'Number of priorities'
    alabel_sccnum = 'Number of SCCs'
    alabel_ntrivlang = 'Number of nontrivial language classes'
    alabel_langclas_avgsize = 'Average size of language classes'

    pdf = backend_pdf.PdfPages(filename)
    pdf.savefig(plot_histogram(data, select_statenum, title, alabel_statenum, alabel_automata))
    pdf.savefig(plot_histogram(data, select_prionum, title, alabel_prionum, alabel_automata, singlebins=True))
    pdf.savefig(plot_histogram(data, select_sccnum, title, alabel_sccnum, alabel_automata, maxx=10))
    pdf.savefig(plot_histogram(data, select_ntrivlang, title, alabel_ntrivlang, alabel_automata))
    pdf.savefig(plot_histogram(data, select_langclas_avgsize, title, alabel_langclas_avgsize, alabel_automata, maxx=10))
    pdf.close()


def main():
    data = read_json_to_map('raw/rawstats.json', 'filename')
    data_gendet = {k: v for k, v in data.items() if 'gendet' in k}
    data_detspot = {k: v for k, v in data.items() if 'detspot' in k}
    data_detnbaut = {k: v for k, v in data.items() if 'detnbaut' in k}
    plot_rawstats(data_gendet, 'analysis/rawstats_gendet.pdf', 'Generated DPAs')
    plot_rawstats(data_detspot, 'analysis/rawstats_detspot.pdf', 'Determinized with Spot')
    plot_rawstats(data_detnbaut, 'analysis/rawstats_detnbaut.pdf', 'Determinized with nbautils')


main()

