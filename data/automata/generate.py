# This script can generate a batch of DPAs in different ways and save them as
# HOA files.

import argparse
import math
import multiprocessing
import os
import random
import re
import subprocess
import sys
import tempfile
import threading

import progressbar

# How many priorities the DPAs should have, if the option to generate them
# directly with spot is chosen.
GENDET_PRIORITIES = 4

# Time in seconds after which a generation process is terminated forcefully.
GEN_TIMEOUT = 60

# Path to nbautils: nbadet executable.
NBAUTILS_NBADET_PATH = '../../code/nbautils/build/bin/nbadet'

# Path to (transition->state based) executable.
PRIORITY_BASE_CONVERTER_PATH = '../../code/bin/transition_based_converter'


def main():
    random.seed()
    args = parse_args()
    filenames = generate_filenames(args.directory)

    if args.determinize_nbautils:
        if not (os.path.isfile(NBAUTILS_NBADET_PATH) and os.access(NBAUTILS_NBADET_PATH, os.X_OK)):
            print('Error. Compile nbautils first or adapt path variable.')
            sys.exit(1)

    written_files_lock = threading.Lock()
    written_files = 0

    def apply_finished(result):
        if result:
            with written_files_lock:
                nonlocal written_files
                written_files += 1
                progress_bar.update(written_files)

    progress_bar = progressbar.ProgressBar(maxval=args.gen_num)
    progress_bar.start()
    progress_bar.update(0)

    tries = 0
    while written_files < args.gen_num and (args.max_tries is None or tries < args.max_tries):
        # Try to generate k many automata, where k is chosen maximal such that
        # neither the targeted number of generated automata is surpassed, nor
        # the maximal number of tries (if specified).
        pool = multiprocessing.Pool(os.cpu_count() - 1)
        map_size = args.gen_num - written_files
        if args.max_tries is not None:
            map_size = min(map_size, args.max_tries - tries)
        for i in range(map_size):
            pool.apply_async(generate_one_automaton, (next(filenames), args), callback=apply_finished)
        pool.close()
        pool.join()


def parse_args():
    parser = argparse.ArgumentParser(description="Generate a batch of DPAs and save them as HOA files.")

    parser.add_argument('-n', dest='gen_num', help='Number of automata to generate.', type=int, default=1)
    parser.add_argument('--max_tries', dest='max_tries',
                        help='Maximal number of tries to generate the requested number of automata. The process simply exits if this number is reached.',
                        type=int)
    parser.add_argument('--ap', help='Number of atomic propositions used by the automata.', type=int, default=1)
    parser.add_argument('-D', dest='directory', help='Output directory.', required=True)
    parser.add_argument('--qmin', dest='min_states', help='Minimal number of states the output automata must have.',
                        required=True, type=int)
    parser.add_argument('--qmax', dest='max_states', help='Maximal number of states the output automata can have.',
                        required=True, type=int)

    group_generation = parser.add_mutually_exclusive_group(required=True)
    group_generation.add_argument('--grand', action='store_true', dest='generate_random',
                                  help='Uses Spot library to generate a random DPA with {} priorities.'.format(
                                      GENDET_PRIORITIES))
    group_generation.add_argument('--gnbaut', action='store_true', dest='determinize_nbautils',
                                  help='Uses Spot library to generate a random NBA and determinize it with nbautils.')
    group_generation.add_argument('--gspot', action='store_true', dest='determinize_spot',
                                  help='Uses Spot library to generate a random NBA and determinize it with Spot.')

    return parser.parse_args()


# Generates an iterable of filenames to be used for the automata.
def generate_filenames(directory):
    i = 0
    while True:
        filename = os.path.join(directory, 'a{}.hoa'.format(i))
        if not os.path.isfile(filename):
            yield filename
        i += 1


# Given a HOA file, reads the number of states of the stored automaton.
def hoa_num_of_states(filename):
    with open(filename, 'r') as file:
        x = file.read()
        re_match = re.search(r"States: (\d+)", x, re.MULTILINE | re.IGNORECASE)
        return int(re_match[1])


# Runs "command" as a subprocess and returns the STDOUT as a string. If time is specified, it is interpreted as a number
# in seconds. If the subprocess does not terminate after that time, it is exited forcefully and None is returned.
# If outfile is specified, the output is written to that file/stream instead.
def run_process_for_time(command, timeout=None, outfile=subprocess.PIPE):
    try:
        completed_process = subprocess.run(command, shell=True, stdout=outfile, timeout=timeout)
        if outfile == subprocess.PIPE:
            return completed_process.stdout
        else:
            return True
    except subprocess.TimeoutExpired:
        print('Process exceeded timeout and is cancelled.', file=sys.stderr)
        return None


# Generates one automaton according to the passed arguments and writes it to
# the specified file. The generation process is adapted to hopefully generate
# an automaton with 'goal_num_of_states' states or similar.
def generate_one_automaton(filename, args):
    if args.generate_random:
        cmd = 'randaut {} -Q{}..{} -D -A \'parity min even {}\' --colored -S --seed={}'
        cmd = cmd.format(atomic_propositions(args.ap), args.min_states, args.max_states, GENDET_PRIORITIES,
                         random.randint(0, 1000000))
        with open(filename, 'w') as file:
            gen_result = run_process_for_time(cmd, outfile=file)
        return True

    elif args.determinize_nbautils:
        with tempfile.NamedTemporaryFile() as nbadet_file:
            with generate_random_nba(args) as generated_automaton_file:
                cmd = '{} -d -s -n -a -b -c -t -m -u1 {}'.format(NBAUTILS_NBADET_PATH, generated_automaton_file.name)
                det_result = determinize(nbadet_file, cmd, args)
            if not det_result:
                return False
            cmd = '{} -A {}'.format(PRIORITY_BASE_CONVERTER_PATH, nbadet_file.name)
            with open(filename, 'w') as file:
                run_process_for_time(cmd, outfile=file)
            return True


    elif args.determinize_spot:
        with generate_random_nba(args) as generated_automaton_file:
            cmd = 'autfilt --colored-parity=\'min even\' --small --high -S -D -C {}'.format(
                generated_automaton_file.name)
            with open(filename, 'w') as file:
                det_result = determinize(file, cmd, args)
            if not det_result:
                os.remove(filename)
            return det_result

    return False


# Generates an array of atomic propositions ('a0', 'a1', ...).
def atomic_propositions(aps):
    return ' '.join(['a' + str(i) for i in range(1, aps + 1)])


# Write random NBA to a HOA file and return a handler to that file.
def generate_random_nba(args):
    qmin = max(1, int(math.log(args.min_states, 2)))
    qmax = max(1, int(math.sqrt(args.max_states)))
    generate_cmd = 'randaut {} -Q{}..{} -A Buchi -S --seed={}'.format(atomic_propositions(args.ap), qmin, qmax,
                                                                      random.randint(0, 1000000))
    generated_automaton_file = tempfile.NamedTemporaryFile()
    run_process_for_time(generate_cmd, outfile=generated_automaton_file)
    return generated_automaton_file


# Determinizes an NBA with a given command. The command should be a string
# that contains exactly one {} which will be replaced by the target output
# file.
def determinize(outfile, command, args):
    if run_process_for_time(command, outfile=outfile, timeout=GEN_TIMEOUT) is None:
        return False
    q = hoa_num_of_states(outfile.name)
    return (q >= args.min_states and q <= args.max_states)


main()
