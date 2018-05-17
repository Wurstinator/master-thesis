#!/bin/bash

# Tests the correct behaviour of the FritzWilke construction on a specific HOA file.
if [ ! -x ../../../bin/fritzwilke_main ]
then
    echo "Error. Compile fritzwilke_main first.";
    exit 1;
fi

fw_aut=$(mktemp)
../../../bin/fritzwilke_main --automaton=$1 > ${fw_aut}
filtered=$(autfilt --equivalent-to=$1 ${fw_aut})
if [ -z "${filtered// }" ]
	then
	echo -e "ERROR! THE AUTOMATA ARE NOT EQUIVALENT!! \n\n\n"
	cat $1
	echo -e "\n\n"
	cat ${fw_aut}
	echo -e "ERROR! THE AUTOMATA ARE NOT EQUIVALENT!! \n\n\n"
fi
