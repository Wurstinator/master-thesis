import matplotlib.pyplot as pyplot
import numpy

TITLE_FONTSIZE = 9

# Plots time used as a function of the number of states. Multiple sets are drawn here in different colors.
def plot_time_comparison(datapoints, title, labels=None):
    f = pyplot.figure()
    nolabels = labels is None
    if nolabels:
        labels = ['' for _ in datapoints]
    for data, label in zip(datapoints, labels):
        x = [point['original_size'] for point in data]
        y = [point['milliseconds'] / 1000 for point in data]
        pyplot.plot(x, y, '.', label=label)
    pyplot.xlabel('Number of states in the original automaton')
    pyplot.ylabel('Time taken in seconds')
    pyplot.title(title, fontsize=TITLE_FONTSIZE)
    if not nolabels:
        pyplot.legend(loc='upper left')
    return f
