
from analyze_common import read_prefix_to_json, read_json_to_map, general_analysis_v1
import os
import errno

directories = {
    'schewe': 'schewe/',
    'fritzwilke': 'fritzwilke/',
    'iterated_moore': 'iterated_moore/',
    'path_refinement': 'path_refinement/',
    'tremoore': 'threshold_moore/',
    'lsf': 'lsf/'
}

datasets = {t: read_prefix_to_json(directories[t]) for t in directories}

rawstats = read_json_to_map('raw/rawstats.json', 'filename')
all_automata = [f for f in rawstats]

for dir in directories.values():
    try:
        os.makedirs('analysis/' + dir)
    except OSError as e:
        if e.errno != errno.EEXIST:
            raise

names = {
    'schewe': 'Schewe',
    'fritzwilke': 'Delayed Simulation',
    'iterated_moore': 'Iterated Moore',
    'path_refinement': 'Path Refinement',
    'tremoore': 'Treshold Moore',
    'lsf': 'LSF'
}

for algo in names:
    general_analysis_v1(names[algo], directories[algo], datasets[algo], rawstats)


# Path Refinement Safra Special
# Plot detnbaut special ap1
detnbaut_ap1_safra = filelines_to_json('raw/path_refinement/detnbaut_special_ap1.json')
title = 'Path Refinement state reduction on a DPA with |Σ|=2 that was created by nbautils from an NBA, using the Safra relation.'
plot_dataset_to_pdf(detnbaut_ap1_safra, 'analysis/path_refinement/detnbaut_safra_ap1.pdf', title)

# Plot detnbaut special ap2
detnbaut_ap2_safra = filelines_to_json('raw/path_refinement/detnbaut_special_ap1.json')
title = 'Path Refinement state reduction on a DPA with |Σ|=4 that was created by nbautils from an NBA, using the Safra relation.'
plot_dataset_to_pdf(detnbaut_ap2_safra, 'analysis/path_refinement/detnbaut_safra_ap2.pdf', title)
