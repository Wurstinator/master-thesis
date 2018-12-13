
#pragma once

#include "../util/equivalence_relation.h"
#include "finite_automaton.h"


namespace tollk {
namespace automaton {

template<typename AutomatonT>
tollk::EquivalenceRelation<tollk::automaton::state_t> PriorityAlmostEquivalence(const AutomatonT& automaton);

}
}

#include "almost_equivalent_states.impl.h"