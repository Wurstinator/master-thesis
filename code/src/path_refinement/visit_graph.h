
#pragma once

#include <boost/bimap.hpp>
#include "../automaton/deterministic_automaton.h"
#include "../automaton/parity.h"

namespace tollk {

// Constructs the graph A^\lambda_visit from a given DPA A.
automaton::DeterministicAutomaton
PriorityVisitGraph(const automaton::DPA& automaton, const EquivalenceRelation<automaton::state_t>::EquivClass& lambda,
                   boost::bimap<automaton::state_t, std::pair<automaton::state_t, automaton::parity_label_t>>* state_indices = nullptr);


// Compute the path refinement equivalence relation using the priority visit graph.
EquivalenceRelation<automaton::state_t> PathRefinementEquivalence_VIS(const automaton::DPA& dpa,
                                                                      const EquivalenceRelation<automaton::state_t>::EquivClass& lambda);

}