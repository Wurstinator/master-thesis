
#pragma once

#include <map>
#include "finite_automaton.h"


/*
 * Extension for the base automaton class which provides a unique successor state for each symbol.
 */
class DeterministicAutomaton : public FiniteAutomaton {
public:
    // Read symbols are represented by this type.
    using symbol_t = unsigned char;

    // Constructs a new det. automaton with a given number of atomic propositions 'm'.
    // The number of symbols will be 2^m. Note that it must hold that m < sizeof(symbol_t).
    explicit DeterministicAutomaton(unsigned char atomic_propositions);

    // Return the successor of a given state and symbol. O(log n) operation.
    state_t Succ(state_t q, symbol_t s) const;

    // Returns a set of all successors of a given state. O(log n) operation.
    const std::vector<state_t>& Successors(state_t q) const;

    // Set the successor of a given state and symbol. O(1) operation.
    void SetSucc(state_t q, symbol_t s, state_t succ);

    // Adds a state to the automaton. The caller has to make sure that it does not exist already.
    // A new state has all outgoing transitions point to itself. O(1) operation.
    void AddState(state_t q) override;

    // Redirects all transitions from one state to another. O(n * 2^m) operation.
    void RedirectTransitions(state_t previous_target, state_t new_target);

    // Removes a state from the automaton, if it exists. The caller has to make sure that the state has no incoming
    // transitions. O(n + m) operation.
    void RemoveState(state_t q) override;

    // Create an isomorphic automaton with state IDs continuously from 0 to n. O(n) operation.
    void NormalizeStateIDs() override;

    // Return 2^m, which is the number of different symbols used by this automaton. O(1) operation.
    symbol_t SymbolsNum() const;

protected:
    const unsigned char atomic_propositions;
    std::map<state_t, std::vector<state_t>> transitions;
};


inline FiniteAutomaton::state_t DeterministicAutomaton::Succ(FiniteAutomaton::state_t q, DeterministicAutomaton::symbol_t s) const {
    return this->transitions[q][s];
}

inline void DeterministicAutomaton::SetSucc(FiniteAutomaton::state_t q, DeterministicAutomaton::symbol_t s, state_t succ) {
    this->transitions[q][s] = succ;
}
