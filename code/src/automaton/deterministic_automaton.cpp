
#include "deterministic_automaton.h"

namespace tollk {
namespace automaton {

DeterministicAutomaton::SuccRangeStateSym
DeterministicAutomaton::Successors(state_t q, symbol_t s) const {
    return ranges::v3::view::single(Succ(q, s));
}

DeterministicAutomaton::SuccRangeState DeterministicAutomaton::Successors(state_t q) const {
    return this->transitions.find(q)->second;
}


void DeterministicAutomaton::AddState(state_t q) {
    FiniteAutomaton::AddState(q);
    this->transitions[q] = std::vector<state_t>(SymbolsNum(), q);
}

void DeterministicAutomaton::RedirectTransitions(state_t previous_target, state_t new_target) {
    for (auto& kv_pair : this->transitions) {
        for (state_t& q : kv_pair.second) {
            if (q == previous_target)
                q = new_target;
        }
    }
}


void DeterministicAutomaton::RemoveState(state_t q) {
    FiniteAutomaton::RemoveState(q);
    const auto iter = this->transitions.find(q);
    if (iter != this->transitions.end())
        this->transitions.erase(iter);
}

}  // namespace automaton
}  // namespace tollk