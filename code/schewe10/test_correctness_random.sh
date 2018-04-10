#!/bin/bash

# Tests the correct behaviour of the Schewe construction on a random DPA.
if [ ! -x ../bin/schewe_main ]
then
    echo "Error. Compile schewe_main first.";
    exit 1;
fi


gen_aut=$(mktemp)
schewe_aut=$(mktemp)

randseed="$(shuf -i 0-100000 -n 1)";
randaut a -D -A 'parity min even 10' --colored -S --seed=${randseed} > ${gen_aut}

../bin/schewe_main --automaton=${gen_aut} > ${schewe_aut}

filtered=$(autfilt --equivalent-to=${gen_aut} ${schewe_aut})

if [ -z "${filtered// }" ]
then
    echo -e "ERROR! THE AUTOMATA ARE NOT EQUIVALENT!! \n\n\n"
    cat ${gen_aut}
    echo -e "\n\n"
    cat ${schewe_aut}
    echo -e "ERROR! THE AUTOMATA ARE NOT EQUIVALENT!! \n\n\n"
fi


