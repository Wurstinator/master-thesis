#!/bin/bash

echo 'Warning! This will generate a huge number of automata. The process can take hours.'
read -p 'Continue?  ' -r
if [[ $REPLY =~ ^[Yy]$ ]]
then
    (set -x;
    mkdir -p gendet/ap1/q1_10 gendet/ap1/q11_20 gendet/ap1/q21_40 gendet/ap1/q41_60 gendet/ap1/q61_90 gendet/ap1/q91_120 gendet/ap1/q121_160 gendet/ap1/q161_200
    mkdir -p gendet/ap2/q1_10 gendet/ap2/q11_20 gendet/ap2/q21_40 gendet/ap2/q41_60 gendet/ap2/q61_90 gendet/ap2/q91_120
    mkdir -p detnbaut/ap1/q1_10 detnbaut/ap1/q11_20 detnbaut/ap1/q21_40 detnbaut/ap1/q41_60 detnbaut/ap1/q61_90 detnbaut/ap1/q91_120 detnbaut/ap1/q121_160
    mkdir -p detnbaut/ap2/q1_10 detnbaut/ap2/q11_20 detnbaut/ap2/q21_40 detnbaut/ap2/q41_60 detnbaut/ap2/q61_90 detnbaut/ap2/q91_120
    mkdir -p detspot/ap1/q1_10 detspot/ap1/q11_20 detspot/ap1/q21_40 detspot/ap1/q41_60 detspot/ap1/q61_90 detspot/ap1/q91_120 detspot/ap1/q121_160
    mkdir -p detspot/ap2/q1_10 detspot/ap2/q11_20 detspot/ap2/q21_40 detspot/ap2/q41_60 detspot/ap2/q61_90 detspot/ap2/q91_120

    python generate.py --grand --ap 1 -D gendet/ap1/q1_10 --qmin 1 --qmax 10 -n 50
    python generate.py --grand --ap 1 -D gendet/ap1/q11_20 --qmin 11 --qmax 20 -n 50
    python generate.py --grand --ap 1 -D gendet/ap1/q21_40 --qmin 21 --qmax 40 -n 40
    python generate.py --grand --ap 1 -D gendet/ap1/q41_60 --qmin 41 --qmax 60 -n 30
    python generate.py --grand --ap 1 -D gendet/ap1/q61_90 --qmin 61 --qmax 90 -n 20
    python generate.py --grand --ap 1 -D gendet/ap1/q91_120 --qmin 91 --qmax 120 -n 15
    python generate.py --grand --ap 1 -D gendet/ap1/q121_160 --qmin 121 --qmax 160 -n 10
    python generate.py --grand --ap 1 -D gendet/ap1/q161_200 --qmin 161 --qmax 200 -n 5

    python generate.py --grand --ap 2 -D gendet/ap2/q1_10 --qmin 1 --qmax 10 -n 50
    python generate.py --grand --ap 2 -D gendet/ap2/q11_20 --qmin 11 --qmax 20 -n 40
    python generate.py --grand --ap 2 -D gendet/ap2/q21_40 --qmin 21 --qmax 40 -n 30
    python generate.py --grand --ap 2 -D gendet/ap2/q41_60 --qmin 41 --qmax 60 -n 20
    python generate.py --grand --ap 2 -D gendet/ap2/q61_90 --qmin 61 --qmax 90 -n 10
    python generate.py --grand --ap 2 -D gendet/ap2/q91_120 --qmin 91 --qmax 120 -n 5

    python generate.py --gnbaut --ap 1 -D detnbaut/ap1/q1_10 --qmin 1 --qmax 10 -n 50
    python generate.py --gnbaut --ap 1 -D detnbaut/ap1/q11_20 --qmin 11 --qmax 20 -n 50
    python generate.py --gnbaut --ap 1 -D detnbaut/ap1/q21_40 --qmin 21 --qmax 40 -n 40
    python generate.py --gnbaut --ap 1 -D detnbaut/ap1/q41_60 --qmin 41 --qmax 60 -n 30
    python generate.py --gnbaut --ap 1 -D detnbaut/ap1/q61_90 --qmin 61 --qmax 90 -n 20
    python generate.py --gnbaut --ap 1 -D detnbaut/ap1/q91_120 --qmin 91 --qmax 120 -n 15
    python generate.py --gnbaut --ap 1 -D detnbaut/ap1/q121_160 --qmin 121 --qmax 160 -n 10

    python generate.py --gnbaut --ap 2 -D detnbaut/ap2/q1_10 --qmin 1 --qmax 10 -n 50
    python generate.py --gnbaut --ap 2 -D detnbaut/ap2/q11_20 --qmin 11 --qmax 20 -n 40
    python generate.py --gnbaut --ap 2 -D detnbaut/ap2/q21_40 --qmin 21 --qmax 40 -n 30
    python generate.py --gnbaut --ap 2 -D detnbaut/ap2/q41_60 --qmin 41 --qmax 60 -n 20
    python generate.py --gnbaut --ap 2 -D detnbaut/ap2/q61_90 --qmin 61 --qmax 90 -n 10
    python generate.py --gnbaut --ap 2 -D detnbaut/ap2/q91_120 --qmin 91 --qmax 120 -n 5

    python generate.py --gspot --ap 1 -D detspot/ap1/q1_10 --qmin 1 --qmax 10 -n 50
    python generate.py --gspot --ap 1 -D detspot/ap1/q11_20 --qmin 11 --qmax 20 -n 50
    python generate.py --gspot --ap 1 -D detspot/ap1/q21_40 --qmin 21 --qmax 40 -n 40
    python generate.py --gspot --ap 1 -D detspot/ap1/q41_60 --qmin 41 --qmax 60 -n 30
    python generate.py --gspot --ap 1 -D detspot/ap1/q61_90 --qmin 61 --qmax 90 -n 20
    python generate.py --gspot --ap 1 -D detspot/ap1/q91_120 --qmin 91 --qmax 120 -n 15
    python generate.py --gspot --ap 1 -D detspot/ap1/q121_160 --qmin 121 --qmax 160 -n 10

    python generate.py --gspot --ap 2 -D detspot/ap2/q1_10 --qmin 1 --qmax 10 -n 50
    python generate.py --gspot --ap 2 -D detspot/ap2/q11_20 --qmin 11 --qmax 20 -n 40
    python generate.py --gspot --ap 2 -D detspot/ap2/q21_40 --qmin 21 --qmax 40 -n 30
    python generate.py --gspot --ap 2 -D detspot/ap2/q41_60 --qmin 41 --qmax 60 -n 20
    python generate.py --gspot --ap 2 -D detspot/ap2/q61_90 --qmin 61 --qmax 90 -n 10
    python generate.py --gspot --ap 2 -D detspot/ap2/q91_120 --qmin 91 --qmax 120 -n 5
    )

fi
