
#pragma once

#include <cassert>
#include <algorithm>
#include <numeric>
#include <vector>
#include <set>

/*
 * A base class for automata. Only provides basic mechanisms for states.
 */
class FiniteAutomaton {
public:
    // States are represented by this type.
    using state_t = unsigned int;

    // Returns a list of the present states. O(1) operation.
    const std::vector<state_t>& States() const;

    // Returns a list of the initial states. O(1) operation.
    const std::set<state_t>& InitialStates() const;

    // Checks whether a given state is initial. O(log n) operation.
    bool IsInitial(state_t q) const;

    // Adds a state to the automaton. The caller has to make sure that it does not exist already. O(1) operation.
    virtual void AddState(state_t q);

    // Removes a state from the automaton, if it exists. O(n) operation.
    virtual void RemoveState(state_t q);

    // Checks whether a state exists. O(n) operation.
    bool HasState(state_t q) const;

    // Create an isomorphic automaton with state IDs continuously from 0 to n.
    virtual void NormalizeStateIDs();

protected:
    std::vector<state_t> states {};
    std::set<state_t> initial_states {};
};


// Implementation


inline const std::vector<FiniteAutomaton::state_t>& FiniteAutomaton::States() const {
    return this->states;
}

inline const std::set<FiniteAutomaton::state_t>& FiniteAutomaton::InitialStates() const {
    return this->initial_states;
}

inline bool FiniteAutomaton::IsInitial(FiniteAutomaton::state_t q) const {
    return this->initial_states.find(q) != this->initial_states.end();
}

inline void FiniteAutomaton::AddState(FiniteAutomaton::state_t q) {
#ifdef NDEBUG
    assert(!HasState(q));
#endif
    this->states.push_back(q);
}

inline void FiniteAutomaton::RemoveState(FiniteAutomaton::state_t q) {
    auto iter = std::find(this->states.begin(), this->states.end(), q);
    if (iter != this->states.end())
        this->states.erase(iter);
}

inline bool FiniteAutomaton::HasState(FiniteAutomaton::state_t q) const {
    return std::find(this->states.begin(), this->states.end(), q) != this->states.end();
}

void FiniteAutomaton::NormalizeStateIDs() {
    std::iota(this->states.begin(), this->states.end(), 0);
}
