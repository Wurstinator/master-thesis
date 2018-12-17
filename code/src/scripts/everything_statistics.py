

import argparse
import os
import sys
from run_statistics import run_experiments, run_process_for_time

everything_executable = '../../bin/everything_statistics'


class ExecEverything:
    def __init__(self, timeout):
        self.timeout = timeout

    def __call__(self, filename):
        cmd = '{} --output=json --automaton={}'.format(everything_executable, filename)
        return run_process_for_time(cmd, timeout=self.timeout)


def main():
    if not (os.path.isfile(everything_executable) and os.access(everything_executable, os.X_OK)):
        print('Error. Compile everything_statistics first.')
        sys.exit(1)

    args = parse_args()
    run_experiments(args, ExecEverything(args.timeout))


# Parses the command line arguments.
def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument('input', help='One or multiple file paths to HOA files.', nargs='+')
    parser.add_argument('-t', dest='timeout', help='Time limit in seconds for each analysis.', type=int)
    parser.add_argument('-o', dest='output', help='Output file. If none is specified, stdout is used.')
    return parser.parse_args()


main()
