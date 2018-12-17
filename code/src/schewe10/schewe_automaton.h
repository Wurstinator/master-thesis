
#pragma once

#include "../automaton/parity.h"


namespace tollk {

// Computes a Schewe automaton of a given deterministic parity automaton. The function performs minimization neither
// before nor after the construction. Uses "priority almost equivalence" as the relation.
void ScheweAutomaton(automaton::DPA* automaton);


// Computes a Schewe automaton of a given deterministic parity automaton. The function performs minimization neither
// before nor after the construction.
void ScheweAutomaton(automaton::DPA* automaton, const EquivalenceRelation<automaton::state_t>& equiv_relation);

}
