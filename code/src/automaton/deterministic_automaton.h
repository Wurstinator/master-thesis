
#pragma once

#include <unordered_map>
#include "transition_automaton.h"
#include "range/v3/core.hpp"
#include "range/v3/view/single.hpp"
#include "range/v3/distance.hpp"
#include "range/v3/action/sort.hpp"

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

    // Copy constructors.
    DeterministicAutomaton(const DeterministicAutomaton& other);
    DeterministicAutomaton(DeterministicAutomaton&& other);

    // Given an arbitrary TransitionAutomaton, tries to construct an equivalent DeterministicAutomaton from it.
    // If any state-symbol combination does not have a unique successor, an exception is thrown.
    template <typename RT1, typename RT2>
    static DeterministicAutomaton FromTransitionAutomaton(const TransitionAutomaton<RT1, RT2>& automaton);

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

    // Merges a set of states into one. Incoming transitions are redirected to the merged state.
    // If the initial state is in the set, the merged state will be the initial state again.
    // Note that the automaton remains deterministic, so only the outgoing transitions of *std::begin(merge_states)
    // are kept. The ID of the new merged state will be *std::begin(merge_states). O(|A|) operation.
    template <typename SetT>
    void MergeStates(SetT&& merge_states);

    // Merge operation but with an explicit representative state. The initial state of the automaton is updated,
    // if necessary.
    template <typename SetT>
    void MergeStates(SetT&& merge_states, state_t representative);

    // Returns true.
    bool IsDeterministic() const override;

 protected:
    std::unordered_map<state_t, std::vector<state_t>> transitions{};
};

template <typename AutomatonT>
constexpr bool IsDeterministicAutomaton() {
    return std::is_base_of<DeterministicAutomaton, AutomatonT>::value;
}


// Implementation


template<typename RT1, typename RT2>
DeterministicAutomaton DeterministicAutomaton::FromTransitionAutomaton(const TransitionAutomaton<RT1, RT2>& automaton) {
    if (!automaton.IsDeterministic())
        throw std::logic_error("DeterministicAutomaton::FromTransitionAutomaton : Given automaton is not deterministic.");

    DeterministicAutomaton result(automaton.atomicPropositions);

    // Copy the states to the new non-deterministic automaton.
    for (state_t q : automaton.States())
        result.AddState(q);

    // Copy the transitions.
    for (state_t q : automaton.States())
        for (symbol_t s : automaton.Symbols())
            result.SetSucc(q, s, *(automaton.Successors(q, s).begin()));

    // Set the initial state.
    result.SetInitialState(automaton.InitialState());

    return result;
}

inline state_t DeterministicAutomaton::Succ(state_t q, symbol_t s) const {
    return this->transitions.find(q)->second[s];
}

inline void DeterministicAutomaton::SetSucc(state_t q, symbol_t s, state_t succ) {
    this->transitions.find(q)->second[s] = succ;
}

template <typename SetT>
void DeterministicAutomaton::MergeStates(SetT&& merge_states) {
    MergeStates(std::forward<SetT>(merge_states), *merge_states.begin());
}


template <typename SetT>
void DeterministicAutomaton::MergeStates(SetT&& merge_states, state_t representative) {
    // Move the incoming transitions to the representative.
    for (state_t q : States()) {
        for (symbol_t s : Symbols()) {
            state_t succ = Succ(q, s);
            for (state_t merge : merge_states)
                if (merge != representative && succ == merge)
                    SetSucc(q, s, representative);
        }
    }

    // Erase the merged states.
    for (state_t merge : merge_states)
        if (merge != representative)
            RemoveState(merge);

    // Change the initial state, if necessary.
    if (merge_states.find(InitialState()) != merge_states.end())
        SetInitialState(representative);
}

inline bool DeterministicAutomaton::IsDeterministic() const {
    return true;
}



}  // namespace automaton
}  // namespace tollk