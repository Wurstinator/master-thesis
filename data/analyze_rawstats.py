
import matplotlib.pyplot as pyplot
import matplotlib.backends.backend_pdf as backend_pdf
import numpy
import json
from enum import Enum


TITLE_FONTSIZE = 9

# Reads a file of json objects.
def filelines_to_json(filename):
    with open(filename, 'r') as file:
        return [json.loads(line) for line in file.readlines()]

# Plots a histogram of a json attribute.
def plot_histogram_priorities(data):
    f = pyplot.figure()
    x = [point['priorities'] for point in data]
    counts, bins, patches = pyplot.hist(x, bins=range(1, max(x)))
    pyplot.title('Histogram of the number of priorities of all automata.', fontsize=TITLE_FONTSIZE)
    pyplot.xlabel('Priorities')
    pyplot.ylabel('Number of automata')
    return f

def main():
    data = filelines_to_json('raw/rawstats.json')
    pdf = backend_pdf.PdfPages('analysis/rawstats.pdf')
    pdf.savefig(plot_histogram_priorities(data))
    pdf.close()

main()
