#!/bin/bash

# Tests the correct behaviour of the FritzWilke construction on a random DPA.
if [ ! -x ../../../bin/fritzwilke_main ]
then
    echo "Error. Compile fritzwilke_main first.";
    exit 1;
fi

if [ $# -eq 0 ];
then
    n=1
else
    n=$1
fi


for ((i=1; i<=n; i++)); do
    gen_aut=$(mktemp)
    fw_aut=$(mktemp)

    randseed="$(shuf -i 0-100000 -n 1)";
    randaut a1 -Q5..40 -D -A 'parity min even 5' --colored -S --seed=${randseed} > ${gen_aut}

    ../../../bin/fritzwilke_main "${@:2}" --automaton=${gen_aut} > ${fw_aut}

    filtered=$(autfilt --equivalent-to=${gen_aut} ${fw_aut})

    if [ -z "${filtered// }" ]
    then
        echo -e "ERROR! THE AUTOMATA ARE NOT EQUIVALENT!! \n\n\n"
        cat ${gen_aut}
        echo -e "\n\n"
        cat ${fw_aut}
        echo -e "ERROR! THE AUTOMATA ARE NOT EQUIVALENT!! \n\n\n"
    fi
done
