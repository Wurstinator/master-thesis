#!/bin/bash

echo 'Warning! This will generate a huge number of automata. The process can take hours.'
read -p 'Continue?  ' -r
if [[ $REPLY =~ ^[Yy]$ ]]
then
    (set -x;
    mkdir -p detnbaut/ap1/q1_10 detnbaut/ap1/q11_20 detnbaut/ap1/q21_40 detnbaut/ap1/q41_60 detnbaut/ap1/q61_90 detnbaut/ap1/q91_120 detnbaut/ap1/q121_160
    mkdir -p detnbaut/ap2/q1_10 detnbaut/ap2/q11_20 detnbaut/ap2/q21_40 detnbaut/ap2/q41_60 detnbaut/ap2/q61_90 detnbaut/ap2/q91_120

    python generate.py --gnbaut --ap 1 -D detnbaut/ap1/q1_10 --qmin 1 --qmax 10 -n 100
    python generate.py --gnbaut --ap 1 -D detnbaut/ap1/q11_20 --qmin 11 --qmax 20 -n 50
    python generate.py --gnbaut --ap 1 -D detnbaut/ap1/q21_40 --qmin 21 --qmax 40 -n 40
    python generate.py --gnbaut --ap 1 -D detnbaut/ap1/q41_60 --qmin 41 --qmax 60 -n 30
    python generate.py --gnbaut --ap 1 -D detnbaut/ap1/q61_90 --qmin 61 --qmax 90 -n 20
    python generate.py --gnbaut --ap 1 -D detnbaut/ap1/q91_120 --qmin 91 --qmax 120 -n 15
    python generate.py --gnbaut --ap 1 -D detnbaut/ap1/q121_160 --qmin 121 --qmax 160 -n 10
    python generate.py --gnbaut --ap 1 -D detnbaut/ap1/q161_200 --qmin 161 --qmax 200 -n 10
    python generate.py --gnbaut --ap 1 -D detnbaut/ap1/q201_300 --qmin 201 --qmax 300 -n 5
    python generate.py --gnbaut --ap 1 -D detnbaut/ap1/q301_400 --qmin 301 --qmax 400 -n 5

    python generate.py --gnbaut --ap 2 -D detnbaut/ap2/q1_10 --qmin 1 --qmax 10 -n 50
    python generate.py --gnbaut --ap 2 -D detnbaut/ap2/q11_20 --qmin 11 --qmax 20 -n 40
    python generate.py --gnbaut --ap 2 -D detnbaut/ap2/q21_40 --qmin 21 --qmax 40 -n 30
    python generate.py --gnbaut --ap 2 -D detnbaut/ap2/q41_60 --qmin 41 --qmax 60 -n 20
    python generate.py --gnbaut --ap 2 -D detnbaut/ap2/q61_90 --qmin 61 --qmax 90 -n 10
    python generate.py --gnbaut --ap 2 -D detnbaut/ap2/q91_120 --qmin 91 --qmax 120 -n 5

    )

fi
