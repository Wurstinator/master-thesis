
#pragma once

#include "../automaton/parity.h"


namespace tollk {

// Computes a Schewe automaton of a given deterministic parity automaton. The function performs minimization neither
// before nor after the construction.
void ScheweAutomaton(automaton::DPA* automaton);

}
