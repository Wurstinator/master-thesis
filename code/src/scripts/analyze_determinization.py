import argparse
import itertools
import multiprocessing
import os
import random
import re
import subprocess
import sys
import tempfile


def main():
    random.seed()

    parser = argparse.ArgumentParser()
    parser.add_argument('-n', default=1, dest='autnum', help='Number of automata to generate and test.', type=int)
    parser.add_argument('--Qmin', help='Minimal number of states.', type=int)
    parser.add_argument('--Qmax', help='Maximal number of states.', type=int)
    parser.add_argument('--ap', help='Number of atomic propositions.', type=int, default=1)

    args = parser.parse_args()

    pool = multiprocessing.Pool(os.cpu_count() - 1)
    data = pool.map(collect_data, itertools.repeat(args, args.autnum))
    for d in data:
        if d is not None:
            s = ','.join([str(x) for x in d])
            sys.stdout.write(s)
            sys.stdout.write('\n')
    sys.stdout.flush()


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


def collect_data(args):
    # Create random NBA.
    atomic_propositions = ' '.join(['a' + str(i) for i in range(1, args.ap + 1)])
    automaton_seed = random.randint(0, 100000)
    generate_cmd = 'randaut {} -Q{}..{} -A Buchi -S --seed={}'.format(atomic_propositions, args.Qmin, args.Qmax, automaton_seed)
    generated_automaton_file = tempfile.NamedTemporaryFile()
    run_process_for_time(generate_cmd, outfile=generated_automaton_file)

    # Determinize with nbautils.
    determinization_timeout_1 = 45
    determinize_cmd_1 = '../nbautils/build/bin/nbadet -d -s -n -a -b -c -t -p -m -u1 {}'.format(generated_automaton_file.name)
    determinized_file_1 = tempfile.NamedTemporaryFile()
    if run_process_for_time(determinize_cmd_1, outfile=determinized_file_1, timeout=determinization_timeout_1) is None:
        return

    # Determinize with Spot.
    determinization_timeout_2 = 45
    determinize_cmd_2 = 'autfilt --colored-parity=\'min even\' -S -D -C {}'.format(generated_automaton_file.name)
    determinized_file_2 = tempfile.NamedTemporaryFile()
    if run_process_for_time(determinize_cmd_2, outfile=determinized_file_2, timeout=determinization_timeout_2) is None:
        return

    # Make sure that the automata are equivalent.
    det1_correct = run_process_for_time("autfilt --equivalent-to={} {}".format(generated_automaton_file.name, determinized_file_1.name))
    det2_correct = run_process_for_time("autfilt --equivalent-to={} {}".format(generated_automaton_file.name, determinized_file_2.name))

    if det1_correct == b'':
        sys.stderr.write('nbautils did not determinize correctly!')
    if det2_correct == b'':
        sys.stderr.write('Spot did not determinize correctly!')

    dat = [hoa_states(generated_automaton_file), hoa_states(determinized_file_1), hoa_states(determinized_file_2)]
    return dat


# Read the number of states from a given HOA file.
def hoa_states(file):
    file.seek(0)
    states_regex = re.compile('States: (\d+)')
    lines = [bstr.decode('utf-8') for bstr in file.readlines()]
    lines = filter(states_regex.match, lines)
    match = states_regex.search(next(lines))
    return int(match.group(1))


main()