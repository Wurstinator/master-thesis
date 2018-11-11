#!/bin/bash

echo 'Warning! This will generate a huge number of automata. The process can take hours.'
read -p 'Continue?  ' -r
if [[ $REPLY =~ ^[Yy]$ ]]
then
    (set -x;
    mkdir -p gendet/ap1/q1_10 gendet/ap1/q11_20 gendet/ap1/q21_40 gendet/ap1/q41_60 gendet/ap1/q61_90 gendet/ap1/q91_120 gendet/ap1/q121_160 gendet/ap1/q161_200
    mkdir -p gendet/ap2/q1_10 gendet/ap2/q11_20 gendet/ap2/q21_40 gendet/ap2/q41_60 gendet/ap2/q61_90 gendet/ap2/q91_120
    mkdir -p gendet/ap3/q1_10 gendet/ap3/q11_20 gendet/ap3/q21_40 gendet/ap3/q41_60 gendet/ap3/q61_90 gendet/ap3/q91_120

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

    python generate.py --grand --ap 3 -D gendet/ap3/q1_10 --qmin 1 --qmax 10 -n 50
    python generate.py --grand --ap 3 -D gendet/ap3/q11_20 --qmin 11 --qmax 20 -n 40
    python generate.py --grand --ap 3 -D gendet/ap3/q21_40 --qmin 21 --qmax 40 -n 30
    python generate.py --grand --ap 3 -D gendet/ap3/q41_60 --qmin 41 --qmax 60 -n 20
    python generate.py --grand --ap 3 -D gendet/ap3/q61_90 --qmin 61 --qmax 90 -n 10
    python generate.py --grand --ap 3 -D gendet/ap3/q91_120 --qmin 91 --qmax 120 -n 5

    )

fi
