import argparse
import os
import sys
import tempfile
import random
import subprocess
import multiprocessing
import threading

statistics_exe = '../../bin/schewe_statistics'


def main():
    random.seed()

    parser = argparse.ArgumentParser()
    parser.add_argument('-n', default=1, dest='autnum', help='Number of automata to generate and test.', type=int)
    parser.add_argument('-g', '--generation', dest='generation', help='Defines the technique to generate the DPAs.',
                        choices=['generate_deterministic', 'determinize_nbautils', 'determinize_spot'],
                        default='generate_deterministic')
    parser.add_argument('--ap', help='Number of atomic propositions.', type=int, default=1)

    if not (os.path.isfile(statistics_exe) and os.access(statistics_exe, os.X_OK)):
        print('Error. Compile schewe_statistics first.')
        sys.exit(1)

    args = parser.parse_args()

    pool = multiprocessing.Pool(os.cpu_count() - 1)
    data_queue = multiprocessing.Queue()
    counter_lock = threading.Lock()
    threads_finished = 0

    def apply_finished(data):
        nonlocal data_queue
        data_queue.put(data)
        nonlocal counter_lock
        with counter_lock:
            nonlocal threads_finished
            threads_finished += 1

    for i in range(args.autnum):
        pool.apply_async(collect_data, (args,), callback=apply_finished)
    pool.close()

    sys.stdout.write("[\n")
    while threads_finished < args.autnum or not data_queue.empty():
        data = data_queue.get()
        if data is not None:
            s = data.decode('utf-8')
            sys.stdout.write(s)
            sys.stdout.flush()
    sys.stdout.write("]")


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
        max_states = 10
        determinization_timeout = 45
        automaton_seed = random.randint(0, 100000)

        generate_cmd = 'randaut {} -Q{}..{} -A Buchi -S --seed={}'.format(atomic_propositions, min_states,
                                                                                    max_states, automaton_seed)

        generated_automaton_file = tempfile.NamedTemporaryFile()
        run_process_for_time(generate_cmd, outfile=generated_automaton_file)

        determinize_cmd = '../../nbautils/build/bin/nbadet -d -s -n -a -b -c -t -p -m -u1 {}'.format(
            generated_automaton_file.name)

        determinized_file = tempfile.NamedTemporaryFile()
        if run_process_for_time(determinize_cmd, outfile=determinized_file, timeout=determinization_timeout) is None:
            return

        automaton_file = determinized_file

    elif args.generation == 'determinize_spot':
        min_states = 1
        max_states = 30
        determinization_timeout = 45
        automaton_seed = random.randint(0, 100000)

        generate_cmd = 'randaut {} -Q{}..{} -A Buchi -S --seed={}'.format(atomic_propositions, min_states,
                                                                                    max_states, automaton_seed)

        generated_automaton_file = tempfile.NamedTemporaryFile()
        run_process_for_time(generate_cmd, outfile=generated_automaton_file)

        determinize_cmd = 'autfilt --colored-parity=\'min even\' --small --high -S -D -C {}'.format(generated_automaton_file.name)

        determinized_file = tempfile.NamedTemporaryFile()
        if run_process_for_time(determinize_cmd, outfile=determinized_file, timeout=determinization_timeout) is None:
            return

        automaton_file = determinized_file

    construction_cmd = statistics_exe
    construction_cmd += ' --output=json --automaton=' + automaton_file.name
    return run_process_for_time(construction_cmd)


main()
