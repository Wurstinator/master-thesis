
#include "nondeterministic_automaton.h"

namespace tollk {
namespace automaton {


NondeterministicAutomaton::NondeterministicAutomaton(const NondeterministicAutomaton& other) : TransitionAutomaton(other.atomicPropositions) {
    this->initial_state = other.initial_state;
    this->states = other.states;
    this->transitions = other.transitions;
}

NondeterministicAutomaton::NondeterministicAutomaton(NondeterministicAutomaton&& other)  : TransitionAutomaton(other.atomicPropositions)  {
    this->initial_state = other.initial_state;
    this->states = std::move(other.states);
    this->transitions = std::move(other.transitions);
}

NondeterministicAutomaton::SuccRangeStateSym NondeterministicAutomaton::Successors(state_t q, symbol_t s) const {
    return StateSymbOut(q, s);
}

NondeterministicAutomaton::SuccRangeState NondeterministicAutomaton::Successors(state_t q) const {
    return ranges::v3::view::for_each(StateOut(q), [this](const std::pair<const symbol_t, std::unordered_set<state_t>>& out_trans) {
        return ranges::v3::view::all(out_trans.second);
    });
}

void NondeterministicAutomaton::AddSucc(state_t q, symbol_t s, state_t succ) {
    StateSymbOut(q, s).insert(succ);
}

void NondeterministicAutomaton::RemoveSucc(state_t q, symbol_t s, state_t succ) {
    StateSymbOut(q, s).erase(succ);
}

void NondeterministicAutomaton::AddState(state_t q) {
    FiniteAutomaton::AddState(q);
    for (symbol_t s : Symbols())
        this->transitions[q][s] = std::unordered_set<state_t>();
}

void NondeterministicAutomaton::DeleteTransitionsTo(state_t to) {
    for (auto& kv_pair : this->transitions) {
        for (auto& kv_pair2 : kv_pair.second) {
            const auto iter = kv_pair2.second.find(to);
            if (iter != kv_pair2.second.end()) {
                kv_pair2.second.erase(iter);
            }
        }
    }
}

void NondeterministicAutomaton::RedirectTransitions(state_t previous_target, state_t new_target) {
    for (auto& kv_pair : this->transitions) {
        for (auto& kv_pair2 : kv_pair.second) {
            const auto iter = kv_pair2.second.find(previous_target);
            if (iter != kv_pair2.second.end()) {
                kv_pair2.second.erase(iter);
                kv_pair2.second.insert(new_target);
            }
        }
    }
}

void NondeterministicAutomaton::RemoveState(state_t q) {
    FiniteAutomaton::RemoveState(q);
    const auto iter = this->transitions.find(q);
    if (iter != this->transitions.end())
        this->transitions.erase(iter);
}

const std::unordered_map<symbol_t, std::unordered_set<state_t>>& NondeterministicAutomaton::StateOut(state_t q) const {
    return this->transitions.find(q)->second;
}

std::unordered_map<symbol_t, std::unordered_set<state_t>>& NondeterministicAutomaton::StateOut(state_t q) {
    return this->transitions.find(q)->second;
}

const std::unordered_set<state_t>& NondeterministicAutomaton::StateSymbOut(state_t q, symbol_t s) const {
    const std::unordered_map<symbol_t, std::unordered_set<state_t>>& state_out = StateOut(q);
    return state_out.find(s)->second;
}

std::unordered_set<state_t>& NondeterministicAutomaton::StateSymbOut(state_t q, symbol_t s) {
    std::unordered_map<symbol_t, std::unordered_set<state_t>>& state_out = StateOut(q);
    return state_out.find(s)->second;
}


}  // namespace automaton
}  // namespace tollk