
#pragma once

#include "swa.hh"
#include "parity.h"
#include "is_specialization_base_of.h"

namespace tollk {
namespace automaton {

// Converts a given nbautils automaton to a nondeterministic parity automaton. In general this involves copying all
// states and transitions and potentially switching around the priorities if the acceptance condition is not min-even.
// The only special case is when the SWA has multiple initial states. In that case, a new initial state is introduced
// to the resulting automaton.
template <typename T,template <typename... Args> class S>
NPA FromNbautils(const nbautils::SWA<T, S>& swa);

// Converts a given automaton to a SWA. The passed automaton model should inherit from TransitionAutomaton and/or
// LabelledAutomaton if the construction should be complete. Otherwise the resulting SWA might e.g. have no
// transitions.
template <typename AutomatonT, typename TagT=void*>
nbautils::SWA<TagT> ToNbautils(const AutomatonT& base_automaton);


}
}





#include "nbautils_bridge.impl.h"