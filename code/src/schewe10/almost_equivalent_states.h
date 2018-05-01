
#pragma once

#include "../equivalence_relation.h"
#include "../automaton/finite_automaton.h"


namespace tollk {

template<typename AutomatonT>
tollk::EquivalenceRelation<tollk::automaton::state_t> PriorityAlmostEquivalence(const AutomatonT& automaton);

}

#include "almost_equivalent_states.impl.h"