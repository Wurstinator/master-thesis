
#pragma once


#include <boost/bimap.hpp>
#include "../automaton/parity.h"

namespace tollk {


struct DelayedSimulationAutomatonState {
    automaton::state_t p;
    automaton::state_t q;
    std::optional<automaton::parity_label_t> obligation; // checkmark equals empty optional
};
int operator<(const DelayedSimulationAutomatonState& lhs, const DelayedSimulationAutomatonState& rhs);


using Desim_Gamma = std::function<std::optional<automaton::parity_label_t>(automaton::parity_label_t, automaton::parity_label_t, std::optional<automaton::parity_label_t>)>;

// Given a DPA, returns the delayed simulation automaton. If the bimap state_indices is not NULL, it is updated with
// the internal IDs of the given state triples. gamma defines the transitions of the third component. See "gamma.h"
// for more detail.
automaton::DPA DelayedSimulationAutomaton(const automaton::DPA& automaton, const Desim_Gamma& gamma, bool resetAtSCCs, boost::bimap<automaton::state_t, DelayedSimulationAutomatonState>* state_indices = nullptr);


}