#!/bin/bash

echo 'Warning! This will rerun all experiments, overwriting the existing data files.'
read -p 'Continue?  ' -r
if [[ $REPLY =~ ^[Yy]$ ]]
then
    (set -x;
    mkdir -p ../../../data/raw
    python schewe10_statistics.py -t 120 -o ../../../data/raw/schewe_gendet_ap1.json "../../../data/automata/gendet/ap1/*/*"
    python schewe10_statistics.py -t 120 -o ../../../data/raw/schewe_gendet_ap2.json "../../../data/automata/gendet/ap2/*/*"
    python schewe10_statistics.py -t 120 -o ../../../data/raw/schewe_gendet_ap3.json "../../../data/automata/gendet/ap3/*/*"
    python schewe10_statistics.py -t 120 -o ../../../data/raw/schewe_detnbaut_ap1.json "../../../data/automata/detnbaut/ap1/*/*"
    python schewe10_statistics.py -t 120 -o ../../../data/raw/schewe_detnbaut_ap2.json "../../../data/automata/detnbaut/ap2/*/*"
    python schewe10_statistics.py -t 120 -o ../../../data/raw/schewe_detspot_ap1.json "../../../data/automata/detspot/ap1/*/*"
    python schewe10_statistics.py -t 120 -o ../../../data/raw/schewe_detspot_ap2.json "../../../data/automata/detspot/ap2/*/*"
    )

fi
