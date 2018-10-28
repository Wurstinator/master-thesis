
#pragma once

#include "../automaton/parity.h"

namespace tollk {

/**
 * Computes \equiv_M^{\leq k}.
 * @param automaton The DPA to compute the relation on.
 * @param k
 * @return The relation.
 */
EquivalenceRelation<automaton::state_t> Moore_LTK(const automaton::DPA& automaton, automaton::parity_label_t k);

}