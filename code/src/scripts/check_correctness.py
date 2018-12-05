
# Runs a construction on a given automaton and then tests whether the
# output still recognizes the same language as before.

import argparse
import os
import sys
from run_statistics import run_experiments, run_process_for_time
import tempfile
import subprocess

autfilt = 'autfilt'
executable_schewe10 = '../../bin/schewe_main'
executable_fritzwilke06 = '../../bin/fritzwilke_main'
executable_iterated_moore = '../../bin/iterated_moore_main'
executable_path_refinement = '../../bin/path_refinement_main'
executable_threshold_moore = '../../bin/tremoore_main'
executable_lsf = '../../bin/lsf_main'
executable_simplelangeq = '../../bin/simplelange_main'

def executable_exists(filename):
    if not (os.path.isfile(filename) and os.access(filename, os.X_OK)):
        print('Error. Compile {} first.'.format(filename))
        sys.exit(1)

class Execute:
    def __init__(self, timeout, construction):
        self.timeout = timeout
        self.construction = construction

    def __call__(self, filename):
        if self.construction == 'schewe10':
            executable = executable_schewe10
            cmd = executable
        elif self.construction == 'fritzwilke06':
            executable = executable_fritzwilke06
            cmd = executable
        elif self.construction == 'fritzwilke06_reset':
            executable = executable_fritzwilke06
            cmd = executable + ' --reset_opt'
        elif self.construction == 'iterated_moore':
            executable = executable_iterated_moore
            cmd = executable
        elif self.construction == 'path_refinement':
            executable = executable_path_refinement
            cmd = executable
        elif self.construction == 'tremoore':
            executable = executable_threshold_moore
            cmd = executable
        elif self.construction == 'lsf':
            executable = executable_lsf
            cmd = executable
        elif self.construction == 'slange':
            executable = executable_simplelangeq
            cmd = executable
        executable_exists(executable)
        cmd = cmd + ' --automaton=' + filename
        return run_process_for_time(cmd, timeout=self.timeout)


def test_correctness(verbose, filename_in, data_out):
    outfile = tempfile.NamedTemporaryFile(mode='w')
    outfile.write(data_out)
    outfile.flush()
    cmd = autfilt + " --equivalent-to=" + filename_in + " " + outfile.name
    process = subprocess.run(cmd, shell=True, capture_output=True)
    process_stdout = process.stdout.decode('utf-8')
    if process_stdout != '':
        if verbose:
            return 'Good. ' + filename_in + '\n'
        else:
            return ''
    else:
        return 'ERROR! ' + filename_in + '\n'


def main():
    args = parse_args()
    run_experiments(args, Execute(args.timeout, args.construction), (lambda x, y: test_correctness(args.verbose, x, y)))


# Parses the command line arguments.
def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument('input', help='One or multiple file paths to HOA files.', nargs='+')
    parser.add_argument('-t', dest='timeout', help='Time limit in seconds for each analysis.', type=int)
    parser.add_argument('-o', dest='output', help='Output file. If none is specified, stdout is used.')
    constr_choices = ['schewe10', 'fritzwilke06', 'fritzwilke06_reset', 'iterated_moore', 'path_refinement', 'tremoore', 'lsf', 'slange']
    parser.add_argument('-c', dest='construction', help='Type of construction to perform on the input.', choices=constr_choices)
    parser.add_argument('-v', dest='verbose', help='Prints a message even if there was no error.', action='store_true')
    return parser.parse_args()


main()
