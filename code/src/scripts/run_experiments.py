
import os
import subprocess
import argparse

constr_choices = ['schewe10', 'fritzwilke06', 'iterated_moore', 'path_refinement', 'tremoore', 'lsf', 'skipper', 'hopcroft']

def parse_args():
    parser = argparse.ArgumentParser()

    parser.add_argument('-c', dest='construction', help='Type of construction to perform on the input.', choices=constr_choices + ['all'], required=True)
    parser.add_argument('-t', dest='timeout', help='Timeout for each individual automaton.', type=int)
    parser.add_argument('--scommon', dest='scommon', help='Test the gendet, detnbaut, and detspot automata', action='store_true')
    parser.add_argument('--snbaut', dest='snbaut', help='Test the detnbaut special cases with Safra.', action='store_true')
    parser.add_argument('--sspecial', dest='sspecial', help='Test the special classes of automata.', action='store_true')
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
    'schewe10': 'schewe10_statistics.py'
}


def perform_experiments(construction, requested_sets, timeout):
    # Make directorties
    try:
        os.makedirs(directories[construction])
    except os.error as e:
        pass

    # Set up commands
    timeout_str = '' if timeout is None else '-t {}'.format(timeout)
    commands = []
    if 'common' in requested_sets:
        commands += [
            'python {} {} -o {}gendet_ap1.json "../../../data/automata/gendet/ap1/*/*.hoa"',
            'python {} {} -o {}gendet_ap2.json "../../../data/automata/gendet/ap2/*/*.hoa"',
            'python {} {} -o {}gendet_ap3.json "../../../data/automata/gendet/ap3/*/*.hoa"',
            'python {} {} -o {}detnbaut_ap1.json "../../../data/automata/detnbaut/ap1/*/*.hoa"',
            'python {} {} -o {}detnbaut_ap2.json "../../../data/automata/detnbaut/ap2/*/*.hoa"',
            'python {} {} -o {}detspot_ap1.json "../../../data/automata/detspot/ap1/*/*.hoa"',
            'python {} {} -o {}detspot_ap2.json "../../../data/automata/detspot/ap2/*/*.hoa"'
        ]

    if 'nbaut' in requested_sets and construction in ['path_refinement', 'lsf']:
        commands += [
            'python {} {} -o {}detnbaut_special_ap1.json --nbautils "../../../data/automata/detnbaut/ap1/*/*"',
            'python {} {} -o {}detnbaut_special_ap2.json --nbautils "../../../data/automata/detnbaut/ap2/*/*"'
        ]

    if 'special' in requested_sets:
        commands.append('python {} {} -o {}maxmichelle.json "../../../data/automata/special/maxmichelle*"')

    # Run commands
    for unformat_cmd in commands:
        cmd = unformat_cmd.format(statistics_py[construction], timeout_str, directories[construction])
        print(cmd)
        subprocess.run(cmd, shell=True)


def parse_requested_sets(args):
    rs = []
    if args.scommon:
        rs.append('common')
    if args.snbaut:
        rs.append('nbaut')
    if args.sspecial:
        rs.append('special')
    return rs


args = parse_args()

if args.construction == 'all':
    for c in constr_choices:
        perform_experiments(c, parse_requested_sets(args), args.timeout)
else:
    perform_experiments(args.construction, parse_requested_sets(args), args.timeout)
