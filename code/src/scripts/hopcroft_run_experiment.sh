#!/bin/bash

echo 'Warning! This will rerun all experiments, overwriting the existing data files.'
read -p 'Continue?  ' -r
if [[ $REPLY =~ ^[Yy]$ ]]
then
    (set -x;
    mkdir -p ../../../data/raw
    mkdir -p ../../../data/raw/hopcroft
    python hopcroft_statistics.py -t 120 -o ../../../data/raw/hopcroft/gendet_ap1.json "../../../data/automata/gendet/ap1/*/*"
    python hopcroft_statistics.py -t 120 -o ../../../data/raw/hopcroft/gendet_ap2.json "../../../data/automata/gendet/ap2/*/*"
    python hopcroft_statistics.py -t 120 -o ../../../data/raw/hopcroft/gendet_ap3.json "../../../data/automata/gendet/ap3/*/*"
    python hopcroft_statistics.py -t 120 -o ../../../data/raw/hopcroft/detnbaut_ap1.json "../../../data/automata/detnbaut/ap1/*/*"
    python hopcroft_statistics.py -t 120 -o ../../../data/raw/hopcroft/detnbaut_ap2.json "../../../data/automata/detnbaut/ap2/*/*"
    python hopcroft_statistics.py -t 120 -o ../../../data/raw/hopcroft/detspot_ap1.json "../../../data/automata/detspot/ap1/*/*"
    python hopcroft_statistics.py -t 120 -o ../../../data/raw/hopcroft/detspot_ap2.json "../../../data/automata/detspot/ap2/*/*"
    )

fi
