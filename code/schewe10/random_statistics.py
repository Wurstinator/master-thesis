import argparse
import os
import sys
import tempfile
import random
from multiprocessing import Pool


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

    # TODO parallelize
    for i in range(args.autnum):
        collect_data(args)


def collect_data(args):
    exe = '../bin/schewe_statistics'

    atomic_propositions = ' '.join(['a' + str(i) for i in range(1, args.ap + 1)])

    if args.generation == 'generate_deterministic':
        generated_automaton_file = tempfile.NamedTemporaryFile()
        automaton_seed = random.randint(0, 100000)
        state_num = random.randint(1, 150)
        os.system('randaut ' + atomic_propositions + ' -Q ' + str(
            state_num) + ' -D -A \'parity min even 3\' --colored -S --seed=' + str(
            automaton_seed) + ' > ' + generated_automaton_file.name)
        automaton_file = generated_automaton_file

    elif args.generation == 'determinize_nbautils':
        generated_automaton_file = tempfile.NamedTemporaryFile()
        automaton_seed = random.randint(0, 100000)
        state_num = random.randint(10, 200)
        os.system('randaut ' + atomic_propositions + ' -Q ' + str(state_num) + ' -A Buchi --colored -S --seed=' + str(
            automaton_seed) + ' > ' + generated_automaton_file.name)

        determinized_file = tempfile.NamedTemporaryFile()
        os.system(
            '../nbautils/build/bin/nbadet -d -s -n -a -b -c -t -p -m -u1 ' +
            generated_automaton_file.name + ' > ' + determinized_file.name)
        automaton_file = determinized_file

    elif args.generation == 'determinize_spot':
        generated_automaton_file = tempfile.NamedTemporaryFile()
        automaton_seed = random.randint(0, 100000)
        state_num = random.randint(1, 100)
        os.system('randaut ' + atomic_propositions + ' -Q ' + str(state_num) + ' -A Buchi --colored -S --seed=' + str(
            automaton_seed) + ' > ' + generated_automaton_file.name)

        determinized_file = tempfile.NamedTemporaryFile()
        os.system(
            'autfilt --parity=\'min even\' -S -D --low -C ' +
            generated_automaton_file.name + ' > ' + determinized_file.name)
        automaton_file = determinized_file

    second_call = exe
    if args.make_complete:
        second_call += ' -c'
    if args.minimize:
        second_call += ' -m'
    if args.hopcroft:
        second_call += ' --hop'
    if args.remove_unreachable:
        second_call += ' --ru'
    second_call += ' --automaton=' + automaton_file.name
    os.system(second_call)


main()