
from analyze_common import read_prefix_to_json, read_json_to_map, general_analysis_v1, analysis_sub, DataType
from common_plots import plot_multipoints
import os
import errno
import matplotlib.backends.backend_pdf as backend_pdf

directories = {
    'hopcroft': 'hopcroft/',
    'skipper': 'skipper/',
    'schewe': 'schewe/',
    'fritzwilke': 'fritzwilke/',
    'iterated_moore': 'iterated_moore/',
    'path_refinement': 'path_refinement/',
    'tremoore': 'threshold_moore/',
    'lsf': 'lsf/',
    'everything': 'everything/',
}

datasets = {t: read_prefix_to_json(directories[t]) for t in directories}

rawstats = read_json_to_map('raw/rawstats.json', 'filename')
all_automata = [f for f in rawstats]

def makedir_ifpos(name):
    try:
        os.makedirs(name)
    except OSError as e:
        if e.errno != errno.EEXIST:
            raise

for dir in directories.values():
    makedir_ifpos('analysis/' + dir)
makedir_ifpos('analysis/everything_safra')


names = {
    'hopcroft': 'Hopcroft',
    'skipper': 'Skip Merger',
    'schewe': 'Schewe',
    'fritzwilke': 'Delayed Simulation',
    'iterated_moore': 'Iterated Moore',
    'path_refinement': 'Path Refinement',
    'tremoore': 'Treshold Moore',
    'lsf': 'LSF',
    'everything': 'Everything',
}

#for algo in names:
#    general_analysis_v1(names[algo], directories[algo], datasets[algo], rawstats)


# Safra Special
data = read_json_to_map('raw/path_refinement/detnbaut_special_ap1.json', 'filename')
analysis_sub(
    'analysis/path_refinement/detnbaut_safra_ap1.pdf',
    'Path Refinement state reduction on a DPA with |Σ|=2 that was created by nbautils from an NBA, using the Safra relation.',
    set(data.keys()).intersection(rawstats.keys()),
    data,
    rawstats)

data = read_json_to_map('raw/path_refinement/detnbaut_special_ap2.json', 'filename')
analysis_sub(
    'analysis/path_refinement/detnbaut_safra_ap2.pdf',
    'Path Refinement state reduction on a DPA with |Σ|=4 that was created by nbautils from an NBA, using the Safra relation.',
    set(data.keys()).intersection(rawstats.keys()),
    data,
    rawstats)

data = read_json_to_map('raw/lsf/detnbaut_special_ap1.json', 'filename')
analysis_sub(
    'analysis/lsf/detnbaut_safra_ap1.pdf',
    'LSF state reduction on a DPA with |Σ|=2 that was created by nbautils from an NBA, using the Safra relation.',
    set(data.keys()).intersection(rawstats.keys()),
    data,
    rawstats)

data = read_json_to_map('raw/lsf/detnbaut_special_ap2.json', 'filename')
analysis_sub(
    'analysis/lsf/detnbaut_safra_ap2.pdf',
    'LSF state reduction on a DPA with |Σ|=4 that was created by nbautils from an NBA, using the Safra relation.',
    set(data.keys()).intersection(rawstats.keys()),
    data,
    rawstats)


# syntcomp
data = read_json_to_map('raw/everything_safra/syntcomp.json', 'filename')
analysis_sub(
    'analysis/everything_safra/syntcomp.pdf',
    'Reduction of Syntcomp automata (with all techniques)',
    set(data.keys()).intersection(rawstats.keys()),
    data,
    rawstats)

data = read_json_to_map('raw/path_refinement/syntcomp.json', 'filename')
analysis_sub(
    'analysis/path_refinement/syntcomp.pdf',
    'Reduction of Syntcomp automata (with PR)',
    set(data.keys()).intersection(rawstats.keys()),
    data,
    rawstats)

data = read_json_to_map('raw/threshold_moore/syntcomp.json', 'filename')
analysis_sub(
    'analysis/threshold_moore/syntcomp.pdf',
    'Reduction of Syntcomp automata (with TM)',
    set(data.keys()).intersection(rawstats.keys()),
    data,
    rawstats)

data = read_json_to_map('raw/lsf/syntcomp.json', 'filename')
analysis_sub(
    'analysis/lsf/syntcomp.pdf',
    'Reduction of Syntcomp automata (with LSF)',
    set(data.keys()).intersection(rawstats.keys()),
    data,
    rawstats)


# "Everything" time comparison
pdf = backend_pdf.PdfPages('analysis/' + directories['everything'] + 'ap_time_comparison.pdf')
select_statenum = (lambda f: rawstats[f]['states'])
automata1 = set(rawstats.keys()).intersection(set(datasets['everything'][DataType.GendetAP1].keys()))
automata2 = set(rawstats.keys()).intersection(set(datasets['everything'][DataType.GendetAP2].keys()))
automata3 = set(rawstats.keys()).intersection(set(datasets['everything'][DataType.GendetAP3].keys()))
select_time_seconds_typed = lambda t: (lambda f: datasets['everything'][t][f]['milliseconds'] / 1000)
select_statered_relative_typed = lambda t: (lambda f: (rawstats[f]['states'] - datasets['everything'][t][f]['new_size']) / rawstats[f]['states'])
pdf.savefig(
    plot_multipoints(
        [automata1, automata2, automata3],
        [select_statenum]*3,
        [select_time_seconds_typed(DataType.GendetAP1), select_time_seconds_typed(DataType.GendetAP2), select_time_seconds_typed(DataType.GendetAP3)],
        'Time comparison for different sizes of Σ.',
        'Number of states in the original automaton',
        'Time taken in seconds',
        ['|Σ|=2', '|Σ|=4', '|Σ|=8']
    )
)
automata1 = set(rawstats.keys()).intersection(set(datasets['everything'][DataType.DetNbautAP1].keys()))
automata2 = set(rawstats.keys()).intersection(set(datasets['everything'][DataType.DetNbautAP2].keys()))
pdf.savefig(
    plot_multipoints(
        [automata1, automata2],
        [select_statenum]*2,
        [select_statered_relative_typed(DataType.DetNbautAP1), select_statered_relative_typed(DataType.DetNbautAP2)],
        'Reduction comparison for different sizes of Σ.',
        'Number of states in the original automaton',
        'Relative number of removed states',
        ['|Σ|=2', '|Σ|=4']
    )
)
pdf.close()
