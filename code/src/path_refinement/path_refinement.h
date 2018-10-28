
#pragma once

#include "../util/equivalence_relation.h"
#include "../automaton/parity.h"


namespace tollk {

// Compute the path refinement equivalence relation.
EquivalenceRelation<automaton::state_t> PathRefinementEquivalence(const automaton::DPA& dpa, const EquivalenceRelation<automaton::state_t>::EquivClass& lambda);

// Given an equivalence class of a congruence relation, performs state reduction via path refinement.
void PathRefinementMerge(automaton::DPA* dpa, const EquivalenceRelation<automaton::state_t>::EquivClass& lambda);

}