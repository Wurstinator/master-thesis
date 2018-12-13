
#pragma once

#include "../automaton/parity.h"


namespace tollk {

// Computes a skip merge automaton of a given deterministic parity automaton.
void SkipperAutomaton(automaton::DPA* automaton);

}
