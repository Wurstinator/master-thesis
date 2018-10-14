
import analyze_common
import matplotlib.pyplot as pyplot

# Analyzes data from multiple algorithms to compare them to each other.
NAMES = ['schewe', 'fritzwilke', 'iterated_moore', 'path_refinement']
PRETTY_NAMES = ['Schewe 10', 'Fritz & Wilke 06', 'Iterated Moore eq.', 'Path Refinement']
all_data = {
    name: list(analyze_common.read_prefix_to_json(name + '/')) for name in NAMES
}
all_data_ap1 = {
    name: [d[0], d[3], d[5]] for name, d in all_data.items()
}
flattenend_data_ap1 = {
    name: [x for d in all_data[name] for x in d] for name in NAMES
}

# Plot the relative state reduction as a boxplot.
title = 'Relative state space reduction for |Σ|=2.'
relative_reduction_ap1 = {
    name: [(x['original_size'] - x['new_size']) / x['original_size'] for x in data] for name, data in flattenend_data_ap1.items()
}
f = pyplot.figure()
pyplot.boxplot([relative_reduction_ap1[name] for name in NAMES], labels=PRETTY_NAMES, sym='')
f.savefig('analysis/relative_reduction_boxplot.pdf')



# Plot the different runtimes as a function of the number of states.
title = 'Time for the different constructions on different DPAs and |Σ|=2.'
analyze_common.plot_time_comparison(
                    [flattenend_data_ap1[name] for name in NAMES],
                     title=title,
                     filename='analysis/compare_algorithm_time.pdf',
                     labels=PRETTY_NAMES)
