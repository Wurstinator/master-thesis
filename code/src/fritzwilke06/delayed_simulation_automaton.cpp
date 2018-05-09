
#include "delayed_simulation_automaton.h"
#include "gamma.h"

namespace tollk {

using namespace automaton;

// Comparison operator so the DelayedSimulationAutomatonState objects can be used in a map.
int operator<(const DelayedSimulationAutomatonState& lhs, const DelayedSimulationAutomatonState& rhs) {
    if (lhs.p < rhs.p) return true;
    if (lhs.p > rhs.p) return false;
    if (lhs.q < rhs.q) return true;
    if (lhs.q > rhs.q) return false;
    return lhs.obligation < rhs.obligation;
}

DPA DelayedSimulationAutomaton(const DPA& automaton, const Desim_Gamma& gamma, boost::bimap<state_t, DelayedSimulationAutomatonState>* state_indices) {
    // Collect all priorities that occur in "automaton".
    std::set<parity_label_t> automaton_priorities;
    for (state_t q : automaton.States())
        automaton_priorities.insert(automaton.GetLabel(q));

    // Fill the map of state pairs.
    boost::bimap<state_t, DelayedSimulationAutomatonState> state_indices_;
    for (state_t p : automaton.States()) {
        for (state_t q : automaton.States()) {
            DelayedSimulationAutomatonState state;
            state.p = p;
            state.q = q;
            state_indices_.left.insert(std::make_pair(state_indices_.size(), state));
            for (parity_label_t priority : automaton_priorities) {
                state.obligation = priority;
                state_indices_.left.insert(std::make_pair(state_indices_.size(), state));
            }
        }
    }

    // Create the product automaton. Initial state does not matter.
    DPA product(automaton.atomicPropositions);
    for (const auto& kv_pair : state_indices_.left)
        product.AddState(kv_pair.first);

    // Set the transitions.
    for (state_t p : automaton.States()) {
        for (state_t q : automaton.States()) {
            for (symbol_t s : product.Symbols()) {
                DelayedSimulationAutomatonState state, succ_state;
                state.p = p;
                state.q = q;
                succ_state.p = automaton.Succ(p, s);
                succ_state.q = automaton.Succ(q, s);
                succ_state.obligation = gamma(automaton.GetLabel(p), automaton.GetLabel(q), state.obligation);
                product.SetSucc(state_indices_.right.at(state), s, state_indices_.right.at(succ_state));
                for (parity_label_t priority : automaton_priorities) {
                    state.obligation = priority;
                    succ_state.obligation = gamma(automaton.GetLabel(p), automaton.GetLabel(q), state.obligation);
                    product.SetSucc(state_indices_.right.at(state), s, state_indices_.right.at(succ_state));
                }
            }
        }
    }

    // Set the state acceptance.
    for (state_t q : product.States())
        product.SetLabel(q, state_indices_.left.at(q).obligation ? 1 : 0);

    if (state_indices != nullptr)
        *state_indices = std::move(state_indices_);
    return product;
}


}