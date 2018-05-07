#!/bin/bash

echo 'Warning! This will rerun all experiments, overwriting the existing data files.'
read -p 'Continue?  ' -r
if [[ $REPLY =~ ^[Yy]$ ]]
then
    (set -x;
    python schewe10_random_statistics.py --ap 1 --generation=generate_deterministic -n 200 > ../../../data/schewe_gendet_ap1.json;
    python schewe10_random_statistics.py --ap 2 --generation=generate_deterministic -n 200 > ../../../data/schewe_gendet_ap2.json;
    python schewe10_random_statistics.py --ap 3 --generation=generate_deterministic -n 150 > ../../../data/schewe_gendet_ap3.json;
    python schewe10_random_statistics.py --ap 1 --generation=determinize_nbautils -n 150 > ../../../data/schewe_detnbaut_ap1.json;
    python schewe10_random_statistics.py --ap 2 --generation=determinize_nbautils -n 25 > ../../../data/schewe_detnbaut_ap2.json;
    python schewe10_random_statistics.py --ap 1 --generation=determinize_spot -n 150 > ../../../data/schewe_detspot_ap1.json;
    python schewe10_random_statistics.py --ap 2 --generation=determinize_spot -n 25 > ../../../data/schewe_detspot_ap2.json;
    )

fi

