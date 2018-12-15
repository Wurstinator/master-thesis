
#pragma once

#include "../automaton/parity.h"

namespace tollk {

/**
 * Computes a "base relation" of \equiv_\text{LSF}^k, which ignores the point of the states being in different SCCs.
 * This is done later in the implementation.
 * @param automaton The DPA.
 * @param k The priority parameter.
 * @param sim The equivalence relation parameter.
 * @return The new relation object.
 */
EquivalenceRelation<automaton::state_t> LSFBaseRelation(const automaton::DPA& automaton, automaton::parity_label_t k, const EquivalenceRelation<automaton::state_t>& sim);

/**
 * Computes a list of equivalence classes that can be used for LSF-merges.
 * @param automaton The DPA.
 * @param k The priority parameter.
 * @param sim The equivalence relation parameter.
 * @return A map. The keys are the representative states while the values are the complete sets of states.
 */
std::unordered_map<automaton::state_t, std::unordered_set<automaton::state_t>> LSFMergeClasses(const automaton::DPA& automaton, automaton::parity_label_t k, const EquivalenceRelation<automaton::state_t>& sim);

/**
 * Runs a state space reduction using the \equiv_\text{LSF}^k relation.
 * @param automaton Pointer to the DPA that is being reduced.
 * @param k The priority parameter.
 * @param sim The equivalence relation parameter.
 */
void LSFPerformMerge(automaton::DPA* automaton, automaton::parity_label_t k, const EquivalenceRelation<automaton::state_t>& sim);

}