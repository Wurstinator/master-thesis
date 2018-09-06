
#pragma once

#include "parity.h"

namespace tollk {
namespace automaton {

// Refines a given equivalence relation to a congruence relation on the automaton.
// TODO This algorithm can be optimized using a partition refinement structure, if necessary.
// Right now it's an implementation of https://en.wikipedia.org/wiki/DFA_minimization.
template<typename RT1, typename RT2>
void RefineToCongruence(EquivalenceRelation<state_t>* relation, const TransitionAutomaton<RT1, RT2>& automaton);

// Uses congruence refinement to reduce the state space of a DPA. (Hopcroft's algorithm)
void Hopcroft(DPA* automaton);

// Merges all states that are equivalent in a given relation into one. For each merged set, the "merge_labels"
// function is called and the label of the merged state is set to the return value.
template<typename AutomatonT, typename LabelT>
void QuotientAutomatonLabelled(AutomatonT* automaton, const EquivalenceRelation<state_t>& relation,
                       const std::function<LabelT(
                               const EquivalenceRelation<state_t>::EquivClass&)>& merge_labels);

// Given a Büchi automaton, returns a set of those states from which no word can be accepted.
template <typename AutomatonT>
std::unordered_set<state_t> BuchiEmptyStates(const AutomatonT& automaton);

// Given a deterministic parity automaton, normalizes the priority function. (returns the new priorities)
std::map<state_t, parity_label_t> NormalizePriorities(DPA dpa);


}  // namespace automaton
}  // namespace tollk


#include "parity_util.impl.h"