
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


generate_set('gendet', 1,   1,  10, 50)
generate_set('gendet', 1,  11,  20, 50)
generate_set('gendet', 1,  21,  40, 40)
generate_set('gendet', 1,  41,  60, 30)
generate_set('gendet', 1,  61,  90, 20)
generate_set('gendet', 1,  91, 120, 15)
generate_set('gendet', 1, 121, 160, 10)
generate_set('gendet', 1, 161, 200,  5)

generate_set('gendet', 2,   1,  10, 50)
generate_set('gendet', 2,  11,  20, 50)
generate_set('gendet', 2,  21,  40, 40)
generate_set('gendet', 2,  41,  60, 30)
generate_set('gendet', 2,  61,  90, 20)
generate_set('gendet', 2,  91, 120, 15)
generate_set('gendet', 2, 121, 160, 10)

generate_set('gendet', 3,   1,  10, 50)
generate_set('gendet', 3,  11,  20, 50)
generate_set('gendet', 3,  21,  40, 40)
generate_set('gendet', 3,  41,  60, 30)
generate_set('gendet', 3,  61,  90, 20)
generate_set('gendet', 3,  91, 120, 15)
generate_set('gendet', 3, 121, 160, 10)


generate_set('detspot', 1,   1,  10, 50)
generate_set('detspot', 1,  11,  20, 50)
generate_set('detspot', 1,  21,  40, 40)
generate_set('detspot', 1,  41,  60, 30)
generate_set('detspot', 1,  61,  90, 20)
generate_set('detspot', 1,  91, 120, 15)
generate_set('detspot', 1, 121, 160, 10)

generate_set('detspot', 2,   1,  10, 50)
generate_set('detspot', 2,  11,  20, 40)
generate_set('detspot', 2,  21,  40, 30)
generate_set('detspot', 2,  41,  60, 20)
generate_set('detspot', 2,  61,  90, 10)
generate_set('detspot', 2,  91, 120,  5)


generate_set('detnbaut', 1,   1,  10, 100)
generate_set('detnbaut', 1,  11,  20,  50)
generate_set('detnbaut', 1,  21,  40,  40)
generate_set('detnbaut', 1,  41,  60,  30)
generate_set('detnbaut', 1,  61,  90,  20)
generate_set('detnbaut', 1,  91, 120,  15)
generate_set('detnbaut', 1, 121, 160,  10)
generate_set('detnbaut', 1, 161, 200,  10)
generate_set('detnbaut', 1, 201, 300,   5)
generate_set('detnbaut', 1, 301, 400,   5)

generate_set('detnbaut', 2,   1,  10, 50)
generate_set('detnbaut', 2,  11,  20, 40)
generate_set('detnbaut', 2,  21,  40, 30)
generate_set('detnbaut', 2,  41,  60, 20)
generate_set('detnbaut', 2,  61,  90, 10)
generate_set('detnbaut', 2,  91, 120,  5)
