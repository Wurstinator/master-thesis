
#pragma once

#include "../equivalence_relation.h"
#include "../automaton/parity.h"


namespace tollk {

// Compute the path refinement equivalence relation.
EquivalenceRelation<automaton::state_t> PathRefinementEquivalence(const automaton::DPA& dpa, const EquivalenceRelation<automaton::state_t>::EquivClass& lambda);


}