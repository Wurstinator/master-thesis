
#pragma once

#include "../automaton/parity.h"


// These functions define the transitions of the third component in the delayed simulation automaton.
// Each takes parameters i and j as priorities/colors, and a parameter k which is either a priority
// or nullopt/checkmark.


namespace tollk {

// Compares two elements from ('natural numbers' union 'checkmark').
bool lte_checkmark(const std::optional<automaton::parity_label_t>& lhs, const std::optional<automaton::parity_label_t>& rhs);
bool lt_checkmark(const std::optional<automaton::parity_label_t>& lhs, const std::optional<automaton::parity_label_t>& rhs);


// Normal "delayed simulation" gamma function.
std::optional<automaton::parity_label_t> Gamma_de(automaton::parity_label_t i, automaton::parity_label_t j, const std::optional<automaton::parity_label_t>& k_or_not);



}