
#pragma once

#include "../automaton/util.h"

namespace tollk {

// Computes the "iterated Moore equivalence" of a given DPA and returns the newly assigned priorities as a map.
// If you want to compute the quotient automaton, update your DPA accordingly to the map and then build the usual
// quotient automaton via Moore equivalence.
std::unordered_map<automaton::state_t, automaton::parity_label_t> IteratedMooreLabels(const automaton::DPA& automaton);

// Computes the "iterated Moore equivalence" and builds the quotient automaton.
void IteratedMooreQuotient(automaton::DPA* automaton);

}