
import os
import subprocess

if input('Warning! This will generate a huge number of automata. The process can take hours. Continue? (y/n) ') not in ['y', 'yes']:
    exit(0)


def generate_set(method, ap, qmin, qmax, n):
    try:
        os.makedirs(method + '/ap{}/q{}_{}/'.format(ap, qmin, qmax))
    except os.error as e:
        pass

    flag_name = {
        'gendet': 'grand',
        'detspot': 'gspot',
        'detnbaut': 'gnbaut'
    }

    cmd = 'python generate.py --{} --ap {} -D {}/ap{}/q{}_{} --qmin {} --qmax {} -n {}'.format(flag_name[method], ap, method, ap, qmin, qmax, qmin, qmax, n)
    print(cmd)
    subprocess.run(cmd, shell=True)



state_sets = [ #(qmin, qmax, n)
    (1, 10, 25),
    (11, 20, 40),
    (21, 40, 55),
    (41, 60, 70),
    (61, 90, 55),
    (91, 120, 40),
    (121, 160, 25)
]

for method in ['gendet', 'detspot', 'detnbaut']:
    aps = [1, 2] if method != 'gendet' else [1, 2, 3]
    for ap in aps:
        for ssn in state_sets:
            generate_set(method, ap, *ssn)


generate_set('detnbaut', 1, 161, 200, 20)
generate_set('detnbaut', 1, 201, 300, 20)
generate_set('detnbaut', 1, 301, 400, 20)
