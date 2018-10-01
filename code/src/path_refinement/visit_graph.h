
#pragma once

#include <boost/bimap.hpp>
#include "../automaton/deterministic_automaton.h"
#include "../automaton/parity.h"

namespace tollk {

struct VisitGraphState {
    automaton::state_t state;
    automaton::parity_label_t tracker;
    automaton::parity_label_t class_diff; // states not in lambda have class_diff == 0
};
bool operator<(const VisitGraphState& lhs, const VisitGraphState& rhs);

// Constructs the graph A^\lambda_visit from a given DPA A.
automaton::DeterministicAutomaton
PriorityVisitGraph(const automaton::DPA& automaton, const EquivalenceRelation<automaton::state_t>::EquivClass& lambda,
                   boost::bimap<automaton::state_t, VisitGraphState>* state_indices = nullptr);

// Given the graph A^\lambda_visit, constructs the equivalence relation "V".
EquivalenceRelation<automaton::state_t>
VisitGraphVRelation(const automaton::DPA& dpa,
                    const boost::bimap<automaton::state_t, VisitGraphState>& state_indices,
                    const EquivalenceRelation<automaton::state_t>::EquivClass& lambda);


// Compute the path refinement equivalence relation using the priority visit graph.
EquivalenceRelation<automaton::state_t> PathRefinementEquivalence_VIS(const automaton::DPA& dpa,
                                                                      const EquivalenceRelation<automaton::state_t>::EquivClass& lambda);

// Given an equivalence class of a congruence relation, performs state reduction via path refinement.
void PathRefinementMerge_VIS(automaton::DPA* dpa, const EquivalenceRelation<automaton::state_t>::EquivClass& lambda);

}