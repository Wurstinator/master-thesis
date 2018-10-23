#!/bin/bash

echo 'Warning! This will rerun all experiments, overwriting the existing data files.'
read -p 'Continue?  ' -r
if [[ $REPLY =~ ^[Yy]$ ]]
then
    (set -x;
    mkdir -p ../../../data/raw
    mkdir -p ../../../data/raw/path_refinement/
    python path_refinement_statistics.py -t 150 -o ../../../data/raw/path_refinement/gendet_ap1.json "../../../data/automata/gendet/ap1/*/*"
    python path_refinement_statistics.py -t 150 -o ../../../data/raw/path_refinement/gendet_ap2.json "../../../data/automata/gendet/ap2/*/*"
    python path_refinement_statistics.py -t 150 -o ../../../data/raw/path_refinement/gendet_ap3.json "../../../data/automata/gendet/ap3/*/*"
    python path_refinement_statistics.py -t 150 -o ../../../data/raw/path_refinement/detnbaut_ap1.json "../../../data/automata/detnbaut/ap1/*/*"
    python path_refinement_statistics.py -t 150 -o ../../../data/raw/path_refinement/detnbaut_ap2.json "../../../data/automata/detnbaut/ap2/*/*"
    python path_refinement_statistics.py -t 150 -o ../../../data/raw/path_refinement/detspot_ap1.json "../../../data/automata/detspot/ap1/*/*"
    python path_refinement_statistics.py -t 150 -o ../../../data/raw/path_refinement/detspot_ap2.json "../../../data/automata/detspot/ap2/*/*"

    python path_refinement_statistics.py -t 150 -o ../../../data/raw/path_refinement/detnbaut_special_ap1.json --nbautils "../../../data/automata/detnbaut/ap1/*/*"
    python path_refinement_statistics.py -t 150 -o ../../../data/raw/path_refinement/detnbaut_special_ap2.json --nbautils "../../../data/automata/detnbaut/ap2/*/*"
    )

fi
