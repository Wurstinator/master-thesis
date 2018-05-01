
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

    // Copy constructors and assignment operators.
    NondeterministicAutomaton(const NondeterministicAutomaton& other);
    NondeterministicAutomaton(NondeterministicAutomaton&& other);

    // Given an arbitrary TransitionAutomaton, constructs an equivalent NondeterministicAutomaton from it.
    template <typename RT1, typename RT2>
    static NondeterministicAutomaton FromTransitionAutomaton(const TransitionAutomaton<RT1, RT2>& automaton);

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

    // Deletes all transitions to a certain state. O(|Δ|+n) operation.
    void DeleteTransitionsTo(state_t to);

    // Redirects all transitions from one state to another. O(|Δ|+n) operation.
    void RedirectTransitions(state_t previous_target, state_t new_target);

    // Removes a state from the automaton, if it exists. The caller has to make sure that the state has no incoming
    // transitions. O(1) operation.
    void RemoveState(state_t q) override;

    // Merges a set of states into one. Outgoing transitions are unified and incoming transitions are redirected to the
    // merged state. If the initial state is in the set, the merged state will be the initial state again.
    // The ID of the new merged state will be *std::begin(range). O(|A|) operation.
    template <typename SetT>
    void MergeStates(const SetT& merge_states);

 protected:
    std::unordered_map<state_t, std::unordered_map<symbol_t, std::unordered_set<state_t>>> transitions{};

 private:
    // Returns the outgoing transitions of a given state / state-symbol combination.
    const std::unordered_map<symbol_t, std::unordered_set<state_t>>& StateOut(state_t q) const;
    std::unordered_map<symbol_t, std::unordered_set<state_t>>& StateOut(state_t q);
    const std::unordered_set<state_t>& StateSymbOut(state_t q, symbol_t s) const;
    std::unordered_set<state_t>& StateSymbOut(state_t q, symbol_t s);
};



// Implementation


template <typename RT1, typename RT2>
NondeterministicAutomaton NondeterministicAutomaton::FromTransitionAutomaton(const TransitionAutomaton<RT1, RT2>& automaton) {
    NondeterministicAutomaton result(automaton.atomicPropositions);

    // Copy the states to the new non-deterministic automaton.
    for (state_t q : automaton.States())
        result.AddState(q);

    // Copy the transitions.
    for (state_t p : automaton.States())
        for (symbol_t s : automaton.Symbols())
            for (state_t q : automaton.Successors(p, s))
                result.AddSucc(p, s, q);

    // Set the initial state.
    result.SetInitialState(automaton.InitialState());

    return result;
}


template <typename SetT>
void NondeterministicAutomaton::MergeStates(const SetT& merge_states) {
    const state_t representative = *merge_states.begin();

    // Move the incoming transitions to the representative.
    for (state_t q : States()) {
        for (symbol_t s : Symbols()) {
            std::unordered_set<state_t>& outgoing = StateSymbOut(q, s);
            for (state_t merge : merge_states) {
                if (merge != representative) {
                    const auto iter = outgoing.find(merge);
                    if (iter != outgoing.end()) {
                        outgoing.erase(iter);
                        outgoing.insert(representative);
                    }
                }
            }
        }
    }

    // Move the outgoing transitions to the representative.
    for (state_t merge : merge_states) {
        for (symbol_t s : Symbols()) {
            std::unordered_set<state_t>& representative_out = StateSymbOut(representative, s);
            for (state_t out : StateSymbOut(merge, s)) {
                representative_out.insert(out);
            }
        }
    }

    // Erase the merged states.
    for (state_t merge : merge_states) {
        if (merge != representative) {
            RemoveState(merge);
        }
    }

    // Change the initial state, if necessary.
    if (merge_states.find(InitialState()) != merge_states.end())
        SetInitialState(representative);
}



}  // namespace automaton
}  // namespace tollk