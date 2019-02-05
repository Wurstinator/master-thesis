
import os.path
import subprocess

timeout = 30

timeout_str = '' if timeout is None else '-t {}'.format(timeout)

cmds = [
    #'python3 fritzwilke06_statistics.py {} -o {} "../../../data/automata/syntcomp/dpas_small/*.hoa"',
    'python3 path_refinement_statistics.py --nbautils {} -o {} "../../../data/automata/syntcomp/dpas_small/*.hoa"',
    'python3 tremoore_statistics.py --nbautils {} -o {} "../../../data/automata/syntcomp/dpas_small/*.hoa"',
    'python3 lsf_statistics.py --nbautils {} -o {} "../../../data/automata/syntcomp/dpas_small/*.hoa"',
    'python3 everything_safra_statistics.py {} -o {} "../../../data/automata/syntcomp/dpas_small/*.hoa"',
]

outfiles = [
   # '../../../data/raw/fritzwilke/syntcomp.json',
    '../../../data/raw/path_refinement/syntcomp.json',
    '../../../data/raw/threshold_moore/syntcomp.json',
    '../../../data/raw/lsf/syntcomp.json',
    '../../../data/raw/everything_safra/syntcomp.json',
]

for cmd, outfile in zip(cmds, outfiles):
    if not os.path.isfile(outfile):
        subprocess.run(cmd.format(timeout_str, outfile), shell=True)
    else:
        print('Skipped. {} already exists.'.format(outfile))
