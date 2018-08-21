
#pragma once

#include "../automaton/util.h"

namespace tollk {

// Computes the "iterated Moore equivalence" of a given DPA and returns it as a relation. No states are merged
// at this point.
EquivalenceRelation<automaton::state_t> IteratedMooreRelation(const automaton::DPA& automaton);

}