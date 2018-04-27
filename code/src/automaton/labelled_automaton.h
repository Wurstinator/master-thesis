
#pragma once

#include <map>
#include "finite_automaton.h"
#include "../equivalence_relation.h"

namespace tollk {
namespace automaton {

/*
 * An extension of the base automaton class which adds labels to the states.
 */
template <typename LabelT>
class LabelledAutomaton : public FiniteAutomaton {
    static_assert(std::is_default_constructible<LabelT>::value);

 public:
    using Label = LabelT;

    // Returns the label of a state.
    const LabelT& GetLabel(state_t q) const;

    // Assigns the label of a state.
    void SetLabel(state_t q, Label label);

    // Returns an equivalence relation object that considers two states equivalent iff they have the same label.
    EquivalenceRelation<state_t> LabelEquivalence() const;

 protected:
    std::unordered_map<state_t, Label> labels;
};

template<typename LabelT>
const LabelT& LabelledAutomaton<LabelT>::GetLabel(state_t q) const {
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
            result.AddConnection(q, *iter);
        }
    }
    return result;
}

}  // namespace automaton
}  // namespace tollk