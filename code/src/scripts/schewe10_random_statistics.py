import argparse
import os
import sys
import subprocess
import multiprocessing
import threading
import queue
import glob

statistics_exe = '../../bin/schewe_statistics'


def main():
    if not (os.path.isfile(statistics_exe) and os.access(statistics_exe, os.X_OK)):
        print('Error. Compile schewe_statistics first.')
        sys.exit(1)

    args = parse_args()
    run_experiments(args)


# Parses the command line arguments.
def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument('input', help='One or multiple file paths to HOA files.', nargs='+')
    parser.add_argument('-t', dest='timeout', help='Time limit in seconds for each analysis.')
    return parser.parse_args()


# Run the specified experiments in parallel.
def run_experiments(args):
    # Initialize the worker pool and necessary variables.
    pool = multiprocessing.Pool(os.cpu_count() - 1)
    data_queue = multiprocessing.Queue()
    counter_lock = threading.Lock()
    threads_todo = 0

    # This function is executed after each successful experiment.
    def apply_finished(data):
        if data is None:
            return
        data_queue.put(data)
        with counter_lock:
            nonlocal threads_todo
            threads_todo -= 1

    # Start the pool.
    with counter_lock:
        for path in args.input:
            for filename in glob.glob(path):
                pool.apply_async(collect_data, (filename, args.timeout), callback=apply_finished)
                threads_todo += 1
    pool.close()

    # Write the data to stdout until all workers terminate or a SIGINT is received.
    while threads_todo > 0 or not data_queue.empty():
        try:
            data = data_queue.get(True, 1)
        except queue.Empty:
            data = None
        if data is not None:
            s = data.decode('utf-8')
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


def collect_data(filename, timeout):
    construction_cmd = statistics_exe
    construction_cmd += ' --output=json --automaton=' + filename
    return run_process_for_time(construction_cmd, timeout=timeout)


main()
