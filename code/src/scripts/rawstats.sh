#!/bin/bash

echo 'Warning! This will rerun all experiments, overwriting the existing data files.'
read -p 'Continue?  ' -r
if [[ $REPLY =~ ^[Yy]$ ]]
then
    (set -x;
    mkdir -p ../../../data/raw
    python rawstats.py -t 180 -o ../../../data/raw/rawstats.json "../../../data/automata/*/*/*/*.hoa"
    )

fi
