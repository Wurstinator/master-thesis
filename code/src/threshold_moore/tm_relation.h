#pragma once

#include "../automaton/parity.h"

namespace tollk {

/**
 * Computes \equiv_\text{TM}^R of an automaton.
 * @param automaton The DPA.
 * @param R The equivalence relation that implies language equivalence.
 * @return The new relation object.
 */
EquivalenceRelation<automaton::state_t> TMRelation(const automaton::DPA& automaton, const EquivalenceRelation<automaton::state_t>& R);

}