
#pragma once

#include <map>
#include <unordered_set>
#include "finite_automaton.h"
#include "../equivalence_relation.h"

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

template<typename LabelT>
void LabelledAutomaton<LabelT>::AddState(state_t q) {
    FiniteAutomaton::AddState(q);
    this->labels[q] = Label();
}

template<typename LabelT>
void LabelledAutomaton<LabelT>::RemoveState(state_t q) {
    FiniteAutomaton::RemoveState(q);
    this->labels.erase(this->labels.find(q));
}

template<typename LabelT>
const typename LabelledAutomaton<LabelT>::Label& LabelledAutomaton<LabelT>::GetLabel(state_t q) const {
    return this->labels.find(q)->second;
}

template<typename LabelT>
void LabelledAutomaton<LabelT>::SetLabel(state_t q, Label label) {
    this->labels[q] = label;
}

template<typename LabelT>
EquivalenceRelation<state_t> LabelledAutomaton<LabelT>::LabelEquivalence() const {
    EquivalenceRelation<state_t> result;
    // For each label, memorize some representative state.
    std::map<LabelT, state_t> representatives;

    for (state_t q : States()) {
        // Check if the label has an assigned representative already.
        const auto iter = representatives.find(GetLabel(q));
        if (iter == representatives.end()) {
            // If a label without a representative was found, create a new equivalence class...
            representatives[GetLabel(q)] = q;
            result.AddConnection(q, q);
        } else {
            // ..otherwise simply add q to the existing equivalence class.
            result.AddConnection(q, iter->second);
        }
    }
    return result;
}


template<typename LabelT>
std::unordered_set<typename LabelledAutomaton<LabelT>::Label> LabelledAutomaton<LabelT>::AllLabels() const {
    std::unordered_set<typename LabelledAutomaton<LabelT>::Label> result;
    for (const std::pair<const state_t, Label>& kv_pair : this->labels)
        result.insert(kv_pair.second);
    return result;
}

}  // namespace automaton
}  // namespace tollk