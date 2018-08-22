
import argparse
import matplotlib.pyplot as pyplot
import numpy
import scipy.optimize
import json
from analyze_common import *

def fitting_function_1(xs, a, b, c):
    return [numpy.asscalar(a*numpy.square(x) + b*numpy.log(x)*x + c*x) for x in xs]

def main():
    gendet_ap1 = filelines_to_json('raw/iterated_moore_gendet_ap1.json')
    gendet_ap2 = filelines_to_json('raw/iterated_moore_gendet_ap2.json')
    gendet_ap3 = filelines_to_json('raw/iterated_moore_gendet_ap3.json')
    detnbaut_ap1 = filelines_to_json('raw/iterated_moore_detnbaut_ap1.json')
    detnbaut_ap2 = filelines_to_json('raw/iterated_moore_detnbaut_ap2.json')
    detspot_ap1 = filelines_to_json('raw/iterated_moore_detspot_ap1.json')
    detspot_ap2 = filelines_to_json('raw/iterated_moore_detspot_ap2.json')

    # Plot gendet ap1
    title = 'Iterated Moore state reduction on random DPAs with three colors and |Σ|=2.'
    plot_statenum_time(gendet_ap1, 'analysis/iterated_moore_gendet_ap1_time.pdf', title)#, fitting_function_1)
    plot_statenum_statereduction(gendet_ap1, 'analysis/iterated_moore_gendet_ap1_statereduction.pdf', title)
    plot_sccnum_statereduction(gendet_ap1, 'analysis/iterated_moore_gendet_ap1_sccreduction.pdf', title)

    # Plot gendet: Time comparison for different ap.
    f = pyplot.figure()
    x = [point['original_size'] for point in gendet_ap1]
    y = [point['milliseconds']/1000 for point in gendet_ap1]
    pyplot.plot(x, y, 'b.')
    x = [point['original_size'] for point in gendet_ap2]
    y = [point['milliseconds']/1000 for point in gendet_ap2]
    pyplot.plot(x, y, 'g.')
    x = [point['original_size'] for point in gendet_ap3]
    y = [point['milliseconds']/1000 for point in gendet_ap3]
    pyplot.plot(x, y, 'r.')
    pyplot.xlabel('Number of states in the original automaton')
    pyplot.ylabel('Time taken in seconds')
    pyplot.title('Time for Iterated Moore construction on a random DPA with 3 priorities and different alphabets.', fontsize=TITLE_FONTSIZE)
    f.savefig('analysis/iterated_moore_gendet_ap_compare_time.pdf')

    # Plot detnbaut ap1
    title = 'Iterated Moore state reduction on a DPA with |Σ|=2 that was created by nbautils from an NBA.'
    plot_statenum_time(detnbaut_ap1, 'analysis/iterated_moore_detnbaut_ap1_time.pdf', title)
    plot_statenum_statereduction(detnbaut_ap1, 'analysis/iterated_moore_detnbaut_ap1_statereduction.pdf', title)
    plot_statenum_statereductionrel(detnbaut_ap1, 'analysis/iterated_moore_detnbaut_ap1_statereductionrelative.pdf', title)
    plot_sccnum_statereduction(detnbaut_ap1, 'analysis/iterated_moore_detnbaut_ap1_sccreduction.pdf', title)

    # Plot detnbaut ap2
    title = 'Iterated Moore state reduction on a DPA with |Σ|=4 that was created by nbautils from an NBA.'
    plot_statenum_time(detnbaut_ap2, 'analysis/iterated_moore_detnbaut_ap2_time.pdf', title)
    plot_statenum_statereduction(detnbaut_ap2, 'analysis/iterated_moore_detnbaut_ap2_statereduction.pdf', title)

    # Plot detspot ap1
    title = 'Iterated Moore state reduction on a DPA with |Σ|=2 that was created by Spot from an NBA.'
    plot_statenum_time(detspot_ap1, 'analysis/iterated_moore_detspot_ap1_time.pdf', title)
    plot_statenum_statereduction(detspot_ap1, 'analysis/iterated_moore_detspot_ap1_statereduction.pdf', title)
    plot_statenum_statereductionrel(detspot_ap1, 'analysis/iterated_moore_detspot_ap1_statereductionrelative.pdf', title)
    plot_sccnum_statereduction(detspot_ap1, 'analysis/iterated_moore_detspot_ap1_sccreduction.pdf', title)

    # Plot detspot ap2
    title = 'Iterated Moore state reduction on a DPA with |Σ|=4 that was created by Spot from an NBA.'
    plot_statenum_time(detspot_ap2, 'analysis/iterated_moore_detspot_ap2_time.pdf', title)
    plot_statenum_statereduction(detspot_ap2, 'analysis/iterated_moore_detspot_ap2_statereduction.pdf', title)




main()
