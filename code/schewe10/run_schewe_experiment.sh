#!/bin/bash

echo 'Warning! This will rerun all experiments, overwriting the existing data files.'
read -p 'Continue?  ' -r
if [[ $REPLY =~ ^[Yy]$ ]]
then
    (set -x;
    python random_statistics.py -m --hop --ru --ap 1 --generation=generate_deterministic -n 200 > ../data/schewe_gendet_ap1.csv;
    python random_statistics.py -m --hop --ru --ap 2 --generation=generate_deterministic -n 200 > ../data/schewe_gendet_ap2.csv;
    python random_statistics.py -m --hop --ru --ap 3 --generation=generate_deterministic -n 150 > ../data/schewe_gendet_ap3.csv;
    python random_statistics.py -m --hop --ru --ap 1 --generation=determinize_nbautils -n 150 > ../data/schewe_detnbaut_ap1.csv;
    python random_statistics.py -m --hop --ru --ap 2 --generation=determinize_nbautils -n 25 > ../data/schewe_detnbaut_ap2.csv;
    python random_statistics.py -m --hop --ru --ap 1 --generation=determinize_spot -n 150 > ../data/schewe_detspot_ap1.csv;
    python random_statistics.py -m --hop --ru --ap 2 --generation=determinize_spot -n 25 > ../data/schewe_detspot_ap2.csv;
    )

fi

