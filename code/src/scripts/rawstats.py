
from run_statistics import run_experiments, run_process_for_time
import argparse
import os


def executable_exists(filename):
    if not (os.path.isfile(filename) and os.access(filename, os.X_OK)):
        print('Error. Compile {} first.'.format(filename))
        sys.exit(1)

def execute(filename):
    cmd = '../../bin/rawstats ' + filename
    return run_process_for_time(cmd)


def main():
    executable_exists('../../bin/rawstats')
    args = parse_args()
    run_experiments(args, execute)


# Parses the command line arguments.
def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument('input', help='One or multiple file paths to HOA files.', nargs='+')
    parser.add_argument('-o', dest='output', help='Output file. If none is specified, stdout is used.')
    return parser.parse_args()


main()