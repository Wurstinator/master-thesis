
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

/**
 * Computes a relation \equiv s.t. p \equiv q iff p \equiv_M^{\leq k} q and c(p) = c(q) = k.
 * @param automaton The DPA to compute the relation on.
 * @return The relation.
 */
EquivalenceRelation<automaton::state_t> AllMoore_LTK(const automaton::DPA& automaton);

}