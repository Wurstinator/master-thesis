
#pragma once


#include <boost/bimap.hpp>
#include "../automaton/parity.h"

namespace tollk {


struct PathRefinementAutomatonState {
    automaton::state_t p;
    automaton::state_t q;
    automaton::parity_label_t k;
    enum LEG {
        Less, Equal, Greater
    };
    LEG obligation;
};

int operator<(const PathRefinementAutomatonState& lhs, const PathRefinementAutomatonState& rhs);

// Takes in a DPA and constructs the path refinement automaton. The initial state with ID 0 always corresponds to \perp.
// If the bimap state_indices is not NULL, it is updated with the internal IDs of the given states.
automaton::DeterministicAutomaton PathRefinementAutomaton(const automaton::DPA& automaton,
                                                          const std::set<automaton::state_t>& lambda,
                                                          boost::bimap<automaton::state_t, PathRefinementAutomatonState>* state_indices = nullptr);


// Returns the initial state q_{0,PR}^{p,q}.
PathRefinementAutomatonState PathRefinementInitialState(const automaton::DPA& automaton, automaton::state_t p, automaton::state_t q);



}