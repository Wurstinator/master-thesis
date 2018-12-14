import matplotlib.pyplot as pyplot
import json
import math

TITLE_FONTSIZE = 9


# Reads a file of json objects.
def filelines_to_json(filename):
    with open(filename, 'r') as file:
        return [json.loads(line) for line in file.readlines()]


# Reads a text file in which each line is a JSON struct. The structs must have a (key) field which is used to initialize
# a map to the rest of the values.
def read_json_to_map(filename, key):
    lines = filelines_to_json(filename)

    def delkey(d, k):
        c = dict(d)
        del c[k]
        return c

    return {j[key]: delkey(j, key) for j in lines}



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


# Draws a point plot and returns the figure.
# Each point corresponds to an element in [data]. Each such point is given to [x_select] and [y_select]
# to obtain the x and y value for said point.
def plot_points(data, x_select, y_select, title, xlabel, ylabel):
    f = pyplot.figure()
    x = [x_select(point) for point in data]
    y = [y_select(point) for point in data]
    pyplot.plot(x, y, 'b.')
    pyplot.title(title, fontsize=TITLE_FONTSIZE)
    pyplot.xlabel(xlabel)
    pyplot.ylabel(ylabel)
    return f

# Draws a histogram by computing [select] on each point in [data].
def plot_histogram(data, select, title, xlabel, ylabel, singlebins=False, maxx=None):
    f = pyplot.figure()
    x = [select(f) for f in data]
    if maxx is not None:
        x = [y for y in x if y <= maxx]
    if singlebins:
        counts, bins, patches = pyplot.hist(x, bins=range(int(math.floor(min(x))), int(math.ceil(max(x))) + 2))
        pyplot.xticks([(x + y) / 2 for x, y in zip(bins, bins[1:])], [int(x) for x in bins])
    else:
        counts, bins, patches = pyplot.hist(x)
        pyplot.xticks(bins)
    pyplot.title(title, fontsize=TITLE_FONTSIZE)
    pyplot.xlabel(xlabel)
    pyplot.ylabel(ylabel)
    return f
