
#pragma once

#include "../automaton/parity.h"


// These functions define the transitions of the third component in the delayed simulation automaton.
// Each takes parameters i and j as priorities/colors, and a parameter k which is either a priority
// or nullopt/checkmark.


namespace tollk {


// Normal "delayed simulation" gamma function.
std::optional<automaton::parity_label_t> Gamma_de(automaton::parity_label_t i, automaton::parity_label_t j, std::optional<automaton::parity_label_t> k_or_not);



}