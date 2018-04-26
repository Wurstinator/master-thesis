
#pragma once

#include <unordered_map>
#include "finite_automaton.h"

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
    const Label& GetLabel(state_t q) const;

    // Assigns the label of a state.
    void SetLabel(state_t q, Label label);

 protected:
    std::unordered_map<state_t, Label> labels;
};

template<typename LabelT>
const Label& LabelledAutomaton<LabelT>::GetLabel(state_t q) const {
    return this->labels.find(q)->second;
}

template<typename LabelT>
void LabelledAutomaton<LabelT>::SetLabel(state_t q, Label label) {
    this->labels[q] = label;
}

}  // namespace automaton
}  // namespace tollk