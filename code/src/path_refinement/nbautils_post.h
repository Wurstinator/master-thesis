
#pragma once

/**
 * Used only for post-processing determinized automata from nbautils. The additional information in the form of
 * state labels is used to compute a relation on the states that implies language equivalence.
 */

#include "../equivalence_relation.h"
#include "../automaton/finite_automaton.h"
#include <map>

namespace tollk {


EquivalenceRelation<automaton::state_t> NbautilsEquivalence(const std::map<automaton::state_t, std::string>& state_labels);

}