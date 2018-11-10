
#pragma once

#include "labelled_automaton.h"
#include "deterministic_automaton.h"
#include "nondeterministic_automaton.h"

namespace tollk {
namespace automaton {

// Classes for parity automata which are basically labelled automata with non-negative integers as labels.
// We consider a "min even" acceptance, i.e. the automaton accepts if the lowest parity seen infinitely often is even.
using parity_label_t = unsigned int;
using ParityAutomaton = LabelledAutomaton<parity_label_t>;
using DPA = DeterministicLabelledAutomaton<parity_label_t>;
using NPA = NondeterministicLabelledAutomaton<parity_label_t>;

bool ParityIsBuchi(const ParityAutomaton& automaton);

template <typename AutomatonT>
constexpr bool IsParityAutomaton() {
    return std::is_base_of<ParityAutomaton, AutomatonT>::value;
}


}
}