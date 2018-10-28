
#pragma once

#include "../util/equivalence_relation.h"
#include "../automaton/parity.h"

namespace tollk {

// Compute the equivalence relation on states of delayed simulation.
EquivalenceRelation<automaton::state_t> DelayedSimulationEquivalence(const automaton::DPA& dpa, bool resetAtSCCs = false);

}