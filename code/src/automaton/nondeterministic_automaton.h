
#pragma once

#include <unordered_set>
#include <unordered_map>
#include "transition_automaton.h"
#include "range/v3/core.hpp"
#include "range/v3/view.hpp"
#include "range/v3/iterator_range.hpp"

namespace tollk {
namespace automaton {

/*
 * Extension for the base automaton class which provides any number of successor states for each symbol.
 */
class NondeterministicAutomaton
        : public TransitionAutomaton<
                const std::unordered_set<state_t>&,
                ranges::v3::any_view<state_t>
        > {
 public:
    explicit NondeterministicAutomaton(unsigned char atomic_propositions) : TransitionAutomaton(atomic_propositions) {}

    // Return iterators describing a range of successors of a given state and symbol. O(|Δ|) operations.
    SuccRangeStateSym Successors(state_t q, symbol_t s) const override;

    // Return iterators describing a range of successors of a given state. O(1) operation.
    SuccRangeState Successors(state_t q) const override;

    // Add the successor of a given state and symbol to the transition list. O(1) operation.
    void AddSucc(state_t q, symbol_t s, state_t succ);

    // Removes the successor of a given state and symbol from the transition list. O(1) operation.
    void RemoveSucc(state_t q, symbol_t s, state_t succ);

    // Adds a state to the automaton. The caller has to make sure that it does not exist already. O(m) operation.
    void AddState(state_t q) override;

    // Deletes all transitions to a certain state.
    void DeleteTransitions(state_t to);

    // Redirects all transitions from one state to another. O(|Δ|) operation.
    void RedirectTransitions(state_t previous_target, state_t new_target);

    // Removes a state from the automaton, if it exists. The caller has to make sure that the state has no incoming
    // transitions. O(1) operation.
    void RemoveState(state_t q) override;

 protected:
    std::unordered_map<state_t, std::unordered_map<symbol_t, std::unordered_set<state_t>>> transitions{};


 private:
    // Returns the outgoing transitions of a given state / state-symbol combination.
    const std::unordered_map<symbol_t, std::unordered_set<state_t>>& StateOut(state_t q) const;
    std::unordered_map<symbol_t, std::unordered_set<state_t>>& StateOut(state_t q);
    const std::unordered_set<state_t>& StateSymbOut(state_t q, symbol_t s) const;
    std::unordered_set<state_t>& StateSymbOut(state_t q, symbol_t s);
};

}  // namespace automaton
}  // namespace tollk