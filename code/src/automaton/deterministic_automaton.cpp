
#include "deterministic_automaton.h"


DeterministicAutomaton::DeterministicAutomaton(unsigned char atomic_propositions) :
        atomic_propositions(atomic_propositions) {}

const std::vector<FiniteAutomaton::state_t>& DeterministicAutomaton::Successors(FiniteAutomaton::state_t q) const {
    return this->transitions[q];
}

void DeterministicAutomaton::AddState(FiniteAutomaton::state_t q) {
    FiniteAutomaton::AddState(q);
    this->transitions[q] = std::vector(SymbolsNum(), q);
}

void DeterministicAutomaton::RedirectTransitions(FiniteAutomaton::state_t previous_target,
                                                 FiniteAutomaton::state_t new_target) {
    for (auto& kv_pair : this->transitions) {
        for (state_t& q : kv_pair.second) {
            if (q == previous_target)
                q = new_target;
        }
    }
}


void DeterministicAutomaton::RemoveState(FiniteAutomaton::state_t q) {
    FiniteAutomaton::RemoveState(q);
    const auto iter = this->transitions.find(q);
    if (iter != this->transitions.end())
        this->transitions.erase(iter);
}

void DeterministicAutomaton::NormalizeStateIDs() {
    FiniteAutomaton::NormalizeStateIDs();
    //TODO
}

DeterministicAutomaton::symbol_t DeterministicAutomaton::SymbolsNum() const {
    return static_cast<symbol_t>(1u) << this->atomic_propositions;
}
