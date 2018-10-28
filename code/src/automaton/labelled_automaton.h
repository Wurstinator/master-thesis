
#pragma once

#include <map>
#include <unordered_set>
#include "finite_automaton.h"
#include "deterministic_automaton.h"
#include "nondeterministic_automaton.h"
#include "../util/equivalence_relation.h"

namespace tollk {
namespace automaton {

/*
 * An extension of the base automaton class which adds labels to the states.
 */
template <typename LabelT>
class LabelledAutomaton : public virtual FiniteAutomaton {
    static_assert(std::is_default_constructible<LabelT>::value);

 public:
    using Label = LabelT;

    // Adds a state to the automaton. The caller has to make sure that it does not exist already. O(1) operation.
    void AddState(state_t q) override;

    // Removes a state from the automaton, if it exists. O(n) operation.
    void RemoveState(state_t q) override;

    // Returns the label of a state.
    const Label& GetLabel(state_t q) const;

    // Assigns the label of a state.
    void SetLabel(state_t q, Label label);

    // Returns an equivalence relation object that considers two states equivalent iff they have the same label.
    EquivalenceRelation<state_t> LabelEquivalence() const;

    // Returns a list of all labels that exist in the automaton.
    std::unordered_set<Label> AllLabels() const;

 protected:
    std::unordered_map<state_t, Label> labels;
};


template <typename LabelT> class NondeterministicLabelledAutomaton;

template <typename LabelT>
class DeterministicLabelledAutomaton : public LabelledAutomaton<LabelT>, public DeterministicAutomaton {
 public:
    using DeterministicAutomaton::DeterministicAutomaton;

    explicit DeterministicLabelledAutomaton(const DeterministicAutomaton& automaton);
    explicit DeterministicLabelledAutomaton(DeterministicAutomaton&& automaton);

    static DeterministicLabelledAutomaton FromNondeterministic(const NondeterministicLabelledAutomaton<LabelT>& automaton);

    void AddState(state_t q) override;
    void RemoveState(state_t q) override;
};

template <typename LabelT>
class NondeterministicLabelledAutomaton : public LabelledAutomaton<LabelT>, public NondeterministicAutomaton {
 public:
    using NondeterministicAutomaton::NondeterministicAutomaton;

    explicit NondeterministicLabelledAutomaton(const NondeterministicAutomaton& automaton);
    explicit NondeterministicLabelledAutomaton(NondeterministicAutomaton&& automaton);

    static NondeterministicLabelledAutomaton FromDeterministic(const DeterministicLabelledAutomaton<LabelT>& automaton);

    void AddState(state_t q) override;
    void RemoveState(state_t q) override;
};


}  // namespace automaton
}  // namespace tollk

#include "labelled_automaton.impl.h"
