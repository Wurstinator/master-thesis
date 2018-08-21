
#pragma once

#include "labelled_automaton.h"
#include "deterministic_automaton.h"
#include "nondeterministic_automaton.h"

namespace tollk {
namespace automaton {

using parity_label_t = unsigned int;

/*
 * Base class for parity automata which are basically labelled automata with non-negative integers as labels.
 * We consider a "min even" acceptance, i.e. the automaton accepts if the lowest parity seen infinitely often is even.
 */
class ParityAutomaton : public LabelledAutomaton<parity_label_t> {
 public:
    // Checks whether this automaton is a Büchi automaton, meaning that all it's priorities are 0 or 1.
    bool IsBuchi() const;
};

class NPA;
class DPA;

class NPA : public ParityAutomaton, public NondeterministicAutomaton {
 public:
    using NondeterministicAutomaton::NondeterministicAutomaton;

    static NPA FromDPA(const DPA& dpa);

    void AddState(state_t q) override {
        NondeterministicAutomaton::AddState(q);
        this->labels[q] = Label();
    }

    void RemoveState(state_t q) override {
        NondeterministicAutomaton::RemoveState(q);
        this->labels.erase(this->labels.find(q));
    }

    NPA(NondeterministicAutomaton&& automaton) : NondeterministicAutomaton(std::move(automaton)) {}
};


class DPA : public ParityAutomaton, public DeterministicAutomaton {
 public:
    using DeterministicAutomaton::DeterministicAutomaton;

    static DPA FromNPA(const NPA& npa);

    void AddState(state_t q) override {
        DeterministicAutomaton::AddState(q);
        this->labels[q] = Label();
    }

    void RemoveState(state_t q) override {
        DeterministicAutomaton::RemoveState(q);
        this->labels.erase(this->labels.find(q));
    }

    DPA(DeterministicAutomaton&& automaton) : DeterministicAutomaton(std::move(automaton)) {}
};





}
}