#!/bin/bash

echo 'Warning! This will rerun all experiments, overwriting the existing data files.'
read -p 'Continue?  ' -r
if [[ $REPLY =~ ^[Yy]$ ]]
then
    (set -x;
    python schewe10_random_statistics.py "../../../data/automata/gendet/ap1/*/*" > ../../../data/raw/schewe_gendet_ap1.json;
    python schewe10_random_statistics.py "../../../data/automata/gendet/ap2/*/*" > ../../../data/raw/schewe_gendet_ap2.json;
    python schewe10_random_statistics.py "../../../data/automata/gendet/ap3/*/*" > ../../../data/raw/schewe_gendet_ap3.json;
    python schewe10_random_statistics.py "../../../data/automata/detnbaut/ap1/*/*" > ../../../data/raw/schewe_detnbaut_ap1.json;
    python schewe10_random_statistics.py "../../../data/automata/detnbaut/ap2/*/*" > ../../../data/raw/schewe_detnbaut_ap2.json;
    python schewe10_random_statistics.py "../../../data/automata/detspot/ap1/*/*" > ../../../data/raw/schewe_detspot_ap1.json;
    python schewe10_random_statistics.py "../../../data/automata/detspot/ap2/*/*" > ../../../data/raw/schewe_detspot_ap2.json;
    )

fi
