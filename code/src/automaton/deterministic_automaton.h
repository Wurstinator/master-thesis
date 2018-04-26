
#pragma once

#include <unordered_map>
#include "transition_automaton.h"
#include "range/v3/core.hpp"
#include "range/v3/view/single.hpp"

namespace tollk {
namespace automaton {

/*
 * Extension for the base automaton class which provides a unique successor state for each symbol.
 */
class DeterministicAutomaton :
        public TransitionAutomaton<
                ranges::v3::single_view<state_t>,
                const std::vector<state_t>&
        > {
 public:
    explicit DeterministicAutomaton(unsigned char atomic_propositions) : TransitionAutomaton(atomic_propositions) {}

    // Return the successor of a given state and symbol. O(log n) operation.
    state_t Succ(state_t q, symbol_t s) const;

    // Set the successor of a given state and symbol. O(1) operation.
    void SetSucc(state_t q, symbol_t s, state_t succ);

    // Return iterators describing a range of successors of a given state and symbol. O(1) operation.
    SuccRangeStateSym Successors(state_t q, symbol_t s) const override;

    // Returns a list of all successors of a given state. O(1) operation.
    SuccRangeState Successors(state_t q) const override;

    // Adds a state to the automaton. The caller has to make sure that it does not exist already.
    // A new state has all outgoing transitions point to itself. O(1) operation.
    void AddState(state_t q) override;

    // Redirects all transitions from one state to another. O(n * m) operation.
    void RedirectTransitions(state_t previous_target, state_t new_target) override;

    // Removes a state from the automaton, if it exists. The caller has to make sure that the state has no incoming
    // transitions. O(1) operation.
    void RemoveState(state_t q) override;

 protected:
    std::unordered_map<state_t, std::vector<state_t>> transitions{};
};


inline state_t DeterministicAutomaton::Succ(state_t q, symbol_t s) const {
    return this->transitions.find(q)->second[s];
}

inline void
DeterministicAutomaton::SetSucc(state_t q, symbol_t s, state_t succ) {
    this->transitions.find(q)->second[s] = succ;
}

}  // namespace automaton
}  // namespace tollk