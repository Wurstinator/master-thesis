
#pragma once

#include "labelled_automaton.h"
#include "deterministic_automaton.h"
#include "nondeterministic_automaton.h"

namespace tollk {
namespace automaton {


/*
 * Base class for parity automata which are basically labelled automata with non-negative integers as labels.
 * We consider a "min even" acceptance, i.e. the automaton accepts if the lowest parity seen infinitely often is even.
 */
class ParityAutomaton : public LabelledAutomaton<unsigned int> {
 public:
    // Checks whether this automaton is a Büchi automaton, meaning that all it's priorities are 0 or 1.
    bool IsBuchi() const;
};


class DPA : public ParityAutomaton, DeterministicAutomaton {
 public:
    using DeterministicAutomaton::DeterministicAutomaton;
};


class NPA : public ParityAutomaton, NondeterministicAutomaton {
 public:
    using NondeterministicAutomaton::NondeterministicAutomaton;
};





}
}