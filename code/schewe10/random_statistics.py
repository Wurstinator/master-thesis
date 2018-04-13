import argparse
import os
import sys
import tempfile
import random
import subprocess
import multiprocessing
import itertools


def main():
    random.seed()

    parser = argparse.ArgumentParser()
    parser.add_argument('-n', default=1, dest='autnum', help='Number of automata to generate and test.', type=int)
    parser.add_argument('-c', action='store_true', dest='make_complete',
                        help='Make the automaton complete by introducing a sink state.')
    parser.add_argument('-m', action='store_true', dest='minimize',
                        help='Minimize the automaton before and/or after applying the construction.')
    parser.add_argument('--hop', action='store_true', dest='hopcroft',
                        help='Perform the Hopcroft algorithm for minimization.')
    parser.add_argument('--ru', action='store_true', dest='remove_unreachable',
                        help='Remove unreachable states for minimization.')
    parser.add_argument('-g', '--generation', dest='generation', help='Defines the technique to generate the DPAs.',
                        choices=['generate_deterministic', 'determinize_nbautils', 'determinize_spot'],
                        default='generate_deterministic')
    parser.add_argument('--ap', help='Number of atomic propositions.', type=int, default=1)

    exe = '../bin/schewe_statistics'
    if not (os.path.isfile(exe) and os.access(exe, os.X_OK)):
        print('Error. Compile schewe_statistics first.')
        sys.exit(1)

    args = parser.parse_args()

    pool = multiprocessing.Pool(os.cpu_count() - 1)
    data = pool.map(collect_data, itertools.repeat(args, args.autnum))
    for d in data:
        if d is not None:
            s = d.decode('utf-8')
            sys.stdout.write(s)
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
    exe = '../bin/schewe_statistics'

    atomic_propositions = ' '.join(['a' + str(i) for i in range(1, args.ap + 1)])

    if args.generation == 'generate_deterministic':
        min_states = 2
        max_states = 150
        colors = 3
        automaton_seed = random.randint(0, 100000)

        cmd = 'randaut {} -Q{}..{} -D -A \'parity min even {}\' --colored -S --seed={}'.format(atomic_propositions,
                                                                                               min_states, max_states,
                                                                                               colors, automaton_seed)
        generated_automaton_file = tempfile.NamedTemporaryFile()
        run_process_for_time(cmd, outfile=generated_automaton_file)

        automaton_file = generated_automaton_file

    elif args.generation == 'determinize_nbautils':
        min_states = 1
        max_states = 100
        determinization_timeout = 45
        automaton_seed = random.randint(0, 100000)

        generate_cmd = 'randaut {} -Q{}..{} -A Buchi --colored -S --seed={}'.format(atomic_propositions, min_states,
                                                                                    max_states, automaton_seed)

        generated_automaton_file = tempfile.NamedTemporaryFile()
        run_process_for_time(generate_cmd, outfile=generated_automaton_file)

        determinize_cmd = '../nbautils/build/bin/nbadet -d -s -n -a -b -c -t -p -m -u1 {}'.format(
            generated_automaton_file.name)

        determinized_file = tempfile.NamedTemporaryFile()
        if run_process_for_time(determinize_cmd, outfile=determinized_file, timeout=determinization_timeout) is None:
            return

        automaton_file = determinized_file

    elif args.generation == 'determinize_spot':  # TODO
        min_states = 2
        max_states = 50
        automaton_seed = random.randint(0, 100000)

        generate_cmd = 'randaut {} -Q{}..{} -A Buchi --colored -S --seed={}'.format(atomic_propositions, min_states,
                                                                                    max_states, automaton_seed)

        generated_automaton_file = tempfile.NamedTemporaryFile()
        run_process_for_time(generate_cmd, outfile=generated_automaton_file)

        determinize_cmd = 'autfilt --parity=\'min even\' -S -D --low -C {}'.format(generated_automaton_file.name)

        determinized_file = tempfile.NamedTemporaryFile()
        run_process_for_time(determinize_cmd, outfile=determinized_file)

        automaton_file = determinized_file

    construction_cmd = exe
    if args.make_complete:
        construction_cmd += ' -c'
    if args.minimize:
        construction_cmd += ' -m'
    if args.hopcroft:
        construction_cmd += ' --hop'
    if args.remove_unreachable:
        construction_cmd += ' --ru'
    construction_cmd += ' --automaton=' + automaton_file.name
    return run_process_for_time(construction_cmd)


main()
