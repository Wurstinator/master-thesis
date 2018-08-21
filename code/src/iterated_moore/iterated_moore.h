
#pragma once

#include "../automaton/util.h"

namespace tollk {

// Computes the "iterated Moore equivalence" of a given DPA and returns it as a relation. No states are merged
// at this point.
// If "new_labels" is not NULL, it is replaced with a map that contains information about those states for which
// the priority was changed during the process.
EquivalenceRelation<automaton::state_t> IteratedMooreRelation(const automaton::DPA& automaton, std::unordered_map<automaton::state_t, automaton::parity_label_t>* new_labels = nullptr);

// Computes the "iterated Moore equivalence" and builds the quotient automaton.
void IteratedMooreQuotient(automaton::DPA* automaton);

}