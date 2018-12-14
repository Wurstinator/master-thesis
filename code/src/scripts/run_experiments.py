
import os
import subprocess
import argparse

def parse_args():
    parser = argparse.ArgumentParser()
    constr_choices = ['schewe10', 'fritzwilke06', 'fritzwilke06_reset', 'iterated_moore', 'path_refinement', 'tremoore', 'lsf', 'skipper', 'hopcroft']
    parser.add_argument('-c', dest='construction', help='Type of construction to perform on the input.', choices=constr_choices)
    parser.add_argument('-t', dest='timeout', help='Timeout for each individual automaton.', type=int)
    return parser.parse_args()


directories = {
    'hopcroft': '../../../data/raw/hopcroft/',
    'skipper': '../../../data/raw/skipper/',
    'fritzwilke06': '../../../data/raw/fritzwilke/',
    'iterated_moore': '../../../data/raw/iterated_moore/',
    'path_refinement': '../../../data/raw/path_refinement/',
    'tremoore': '../../../data/raw/threshold_moore/',
    'lsf': '../../../data/raw/lsf/',
    'schewe10': '../../../data/raw/schewe/'
}

statistics_py = {
    'hopcroft': 'hopcroft_statistics.py',
    'skipper': 'skipper_statistics.py',
    'fritzwilke06': 'fritzwilke06_statistics.py',
    'iterated_moore': 'iterated_moore_statistics.py',
    'path_refinement': 'path_refinement_statistics.py',
    'tremoore': 'tremoore_statistics.py',
    'lsf': 'lsf_statistics.py',
    'schewe10': 'schewe_statistics.py'
}

args = parse_args()

# Make directorties
try:
    os.makedirs(directories[args.construction])
except os.error as e:
    pass

# Run experiments
commands = [
    'python {} -t {} -o {}gendet_ap1.json "../../../data/automata/gendet/ap1/*/*.hoa"',
    'python {} -t {} -o {}gendet_ap2.json "../../../data/automata/gendet/ap2/*/*.hoa"',
    'python {} -t {} -o {}gendet_ap3.json "../../../data/automata/gendet/ap3/*/*.hoa"',
    'python {} -t {} -o {}detnbaut_ap1.json "../../../data/automata/detnbaut/ap1/*/*.hoa"',
    'python {} -t {} -o {}detnbaut_ap2.json "../../../data/automata/detnbaut/ap2/*/*.hoa"',
    'python {} -t {} -o {}detspot_ap1.json "../../../data/automata/detspot/ap1/*/*.hoa"',
    'python {} -t {} -o {}detspot_ap2.json "../../../data/automata/detspot/ap2/*/*.hoa"',
    'python {} -t {} -o {}maxmichelle.json "../../../data/automata/special/maxmichelle*"'
]

for cmd in commands:
    cmd_formatted = cmd.format(statistics_py[args.construction], args.timeout, directories[args.construction])
    print(cmd_formatted)
    subprocess.run(cmd_formatted, shell=True)

if (args.construction == 'path_refinement'):
    cmd = 'python path_refinement_statistics.py -t {} -o ../../../data/raw/path_refinement/detnbaut_special_ap1.json --nbautils "../../../data/automata/detnbaut/ap1/*/*"'.format(args.timeout)
    print(cmd)
    subprocess.run(cmd, shell=True)
    cmd = 'python path_refinement_statistics.py -t {} -o ../../../data/raw/path_refinement/detnbaut_special_ap2.json --nbautils "../../../data/automata/detnbaut/ap2/*/*"'.format(args.timeout)
    print(cmd)
    subprocess.run(cmd, shell=True)
