#!/bin/bash

echo 'Warning! This will rerun all experiments, overwriting the existing data files.'
read -p 'Continue?  ' -r
if [[ $REPLY =~ ^[Yy]$ ]]
then
    (set -x;
    mkdir -p ../../../data/raw
    mkdir -p ../../../data/raw/iterated_moore/
    python iterated_moore_statistics.py -t 90 -o ../../../data/raw/iterated_moore/gendet_ap1.json "../../../data/automata/gendet/ap1/*/*"
    python iterated_moore_statistics.py -t 90 -o ../../../data/raw/iterated_moore/gendet_ap2.json "../../../data/automata/gendet/ap2/*/*"
    python iterated_moore_statistics.py -t 90 -o ../../../data/raw/iterated_moore/gendet_ap3.json "../../../data/automata/gendet/ap3/*/*"
    python iterated_moore_statistics.py -t 90 -o ../../../data/raw/iterated_moore/detnbaut_ap1.json "../../../data/automata/detnbaut/ap1/*/*"
    python iterated_moore_statistics.py -t 90 -o ../../../data/raw/iterated_moore/detnbaut_ap2.json "../../../data/automata/detnbaut/ap2/*/*"
    python iterated_moore_statistics.py -t 90 -o ../../../data/raw/iterated_moore/detspot_ap1.json "../../../data/automata/detspot/ap1/*/*"
    python iterated_moore_statistics.py -t 90 -o ../../../data/raw/iterated_moore/detspot_ap2.json "../../../data/automata/detspot/ap2/*/*"
    )

fi
