
#pragma once

#include <cassert>
#include <algorithm>
#include <numeric>
#include <vector>

namespace tollk {
namespace automaton {


// States are represented by this type.
using state_t = unsigned int;

/*
 * A base class for automata. Only provides basic mechanisms for states.
 */
class FiniteAutomaton {
public:

    virtual ~FiniteAutomaton() = default;

    // Returns a list of the present states. O(1) operation.
    const std::vector<state_t>& States() const;

    // Adds a state to the automaton. The caller has to make sure that it does not exist already. O(1) operation.
    virtual void AddState(state_t q);

    // Removes a state from the automaton, if it exists. O(n) operation.
    virtual void RemoveState(state_t q);

    // Checks whether a state exists. O(n) operation.
    bool HasState(state_t q) const;

    // Returns a the initial state. O(1) operation.
    state_t InitialState() const;

    // Sets the initial state.
    void SetInitialState(state_t q);

protected:
    std::vector<state_t> states {};
    state_t initial_state {};
};


// Implementation


inline const std::vector<state_t>& FiniteAutomaton::States() const {
    return this->states;
}


inline void FiniteAutomaton::AddState(state_t q) {
#ifdef NDEBUG
    assert(!HasState(q));
#endif
    this->states.push_back(q);
}


inline void FiniteAutomaton::RemoveState(state_t q) {
    auto iter = std::find(this->states.begin(), this->states.end(), q);
    if (iter != this->states.end())
        this->states.erase(iter);
}

inline bool FiniteAutomaton::HasState(state_t q) const {
    return std::find(this->states.begin(), this->states.end(), q) != this->states.end();
}

inline state_t FiniteAutomaton::InitialState() const {
    return this->initial_state;
}

inline void FiniteAutomaton::SetInitialState(state_t q) {
    this->initial_state = q;
}

}  // namespace automaton
}  // namespace tollk