
# Provides functionality that is shared by all "random_statistics" scripts.


import sys
import subprocess
import multiprocessing
import threading
import os
import queue
import glob
import progressbar


# Run the specified experiments in parallel. 'callable' is the test runner. It is called with for each input file with
# the filename as an argument. It needs to be multiprocessing-compatible, i.e. no lambdas.
def run_experiments(args, callable):
    # Initialize the worker pool and necessary variables.
    pool = multiprocessing.Pool(os.cpu_count() - 1)
    data_queue = multiprocessing.Queue()
    counter_lock = threading.Lock()  # lock for "threads_todo"
    threads_todo = 0

    # This function is executed after each successful experiment.
    def apply_finished(data):
        if data is not None:
            data_queue.put(data)
        with counter_lock:
            nonlocal threads_todo
            threads_todo -= 1
            # Only use a progress bar if the output is to a file.
            if args.output is not None:
                progress_bar.update(threads_todo_max - threads_todo)

    # Start the pool.
    with counter_lock:
        for path in args.input:
            for filename in glob.glob(path):
                pool.apply_async(callable, (filename,), callback=apply_finished)
                threads_todo += 1
        # Only use a progress bar if the output is to a file.
        if args.output is not None:
            threads_todo_max = threads_todo
            progress_bar = progressbar.ProgressBar(max_value=threads_todo_max)
            progress_bar.update(0)
    pool.close()

    # Write the data to stdout until all workers terminate or a SIGINT is received.
    if args.output is not None:
        outfile = open(args.output, 'w')
    while threads_todo > 0 or not data_queue.empty():
        try:
            data = data_queue.get(True, 1)
        except queue.Empty:
            data = None
        if data is not None:
            outstream = sys.stdout if args.output is None else outfile
            outstream.write(data.decode('utf-8'))
            outstream.flush()
    if args.output is not None:
        outfile.close()


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
