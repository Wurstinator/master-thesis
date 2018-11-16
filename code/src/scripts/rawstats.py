
from run_statistics import run_experiments, run_process_for_time
import argparse
import os

executable = '../../bin/rawstats'

class ExecRawstats:
    def __init__(self, timeout):
        self.timeout = timeout

    def __call__(self, filename):
        cmd = '{} {}'.format(executable, filename)
        return run_process_for_time(cmd, timeout=self.timeout)


def main():
    if not (os.path.isfile(executable) and os.access(executable, os.X_OK)):
        print('Error. Compile rawstats first.')
        sys.exit(1)

    args = parse_args()
    run_experiments(args, ExecRawstats(args.timeout))


# Parses the command line arguments.
def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument('input', help='One or multiple file paths to HOA files.', nargs='+')
    parser.add_argument('-t', dest='timeout', help='Time limit in seconds for each analysis.', type=int)
    parser.add_argument('-o', dest='output', help='Output file. If none is specified, stdout is used.')
    return parser.parse_args()


main()