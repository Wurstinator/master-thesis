
#include "path_refinement_automaton.h"
#include "../automaton/util.h"

namespace tollk {

using namespace automaton;

// Comparison operator so the DelayedSimulationAutomatonState objects can be used in a map.
int operator<(const PathRefinementAutomatonState& lhs, const PathRefinementAutomatonState& rhs) {
    if (lhs.p < rhs.p) return true;
    if (lhs.p > rhs.p) return false;
    if (lhs.q < rhs.q) return true;
    if (lhs.q > rhs.q) return false;
    if (lhs.k < rhs.k) return true;
    if (lhs.k > rhs.k) return false;
    return lhs.obligation < rhs.obligation;
}

PathRefinementAutomatonState::LEG
_EtaX(parity_label_t kp, parity_label_t kq, std::optional<parity_label_t> k, PathRefinementAutomatonState::LEG x) {
    if (kp < kq && (!k || kp < k))
        return PathRefinementAutomatonState::Less;
    if (kp > kq && (!k || kq < k))
        return PathRefinementAutomatonState::Greater;
    if (k && k < kq && x == PathRefinementAutomatonState::Less)
        return PathRefinementAutomatonState::Less;
    if (k && k < kp && x == PathRefinementAutomatonState::Greater)
        return PathRefinementAutomatonState::Greater;
    return PathRefinementAutomatonState::Equal;
}

// Computes the successor state. nullopt corresponds to the sink state.
std::optional<PathRefinementAutomatonState>
_Delta(const DPA& automaton, const PathRefinementAutomatonState& state, symbol_t a,
       const std::set<state_t>& lambda) {
    const bool goes_to_lambda = (lambda.find(automaton.Succ(state.p, a)) != lambda.end());
    const bool obligations_satisfied = (state.obligation == PathRefinementAutomatonState::Equal);

    if (goes_to_lambda && !obligations_satisfied)
        return std::nullopt;

    const state_t p_prime = automaton.Succ(state.p, a);
    const state_t q_prime = automaton.Succ(state.q, a);
    const parity_label_t kp = automaton.GetLabel(p_prime);
    const parity_label_t kq = automaton.GetLabel(q_prime);
    const parity_label_t k_prime = std::min(std::min(kp, kq), (goes_to_lambda ? kp : state.k));
    const std::optional<parity_label_t> k = goes_to_lambda ? std::nullopt : std::optional<parity_label_t>(state.k);
    const PathRefinementAutomatonState::LEG x_prime = _EtaX(kp, kq, k,
                                                            goes_to_lambda ? PathRefinementAutomatonState::Equal
                                                                           : state.obligation);
    return PathRefinementAutomatonState{p_prime, q_prime, k_prime, x_prime};
}

DeterministicAutomaton PathRefinementAutomaton(const DPA& automaton, const std::set<state_t>& lambda,
                                               boost::bimap<state_t, PathRefinementAutomatonState>* state_indices) {
    // Collect all priorities that occur in "automaton".
    std::set<parity_label_t> automaton_priorities;
    for (state_t q : automaton.States())
        automaton_priorities.insert(automaton.GetLabel(q));

    // Fill the map of state pairs.
    boost::bimap<state_t, PathRefinementAutomatonState> state_indices_;
    for (state_t p : automaton.States()) {
        for (state_t q : automaton.States()) {
            for (parity_label_t k : automaton_priorities) {
                PathRefinementAutomatonState state{p, q, k, PathRefinementAutomatonState::Less};
                state.obligation = PathRefinementAutomatonState::Less;
                state_indices_.left.insert(std::make_pair(1 + state_indices_.size(), state));
                state.obligation = PathRefinementAutomatonState::Equal;
                state_indices_.left.insert(std::make_pair(1 + state_indices_.size(), state));
                state.obligation = PathRefinementAutomatonState::Greater;
                state_indices_.left.insert(std::make_pair(1 + state_indices_.size(), state));
            }
        }
    }

    // Create the product automaton. Initial state is 0 = \perp.
    DeterministicAutomaton product(automaton.atomicPropositions);
    product.AddState(0);
    product.SetInitialState(0);
    for (const auto& kv_pair : state_indices_.left)
        product.AddState(kv_pair.first);

    // Set the transitions.
    for (state_t prod_p : product.States()) {
        for (symbol_t s : product.Symbols()) {
            if (prod_p == 0)
                continue;
            const PathRefinementAutomatonState p_decoded = state_indices_.left.at(prod_p);
            const std::optional<PathRefinementAutomatonState> successor = _Delta(automaton, p_decoded, s, lambda);
            if (successor)
                product.SetSucc(prod_p, s, state_indices_.right.at(*successor));
            else
                product.SetSucc(prod_p, s, 0);
        }
    }

    for (symbol_t s : product.Symbols())
        product.SetSucc(0, s, 0);

    if (state_indices != nullptr)
        *state_indices = std::move(state_indices_);
    return product;
}


PathRefinementAutomatonState
PathRefinementInitialState(const DPA& automaton, automaton::state_t p, automaton::state_t q) {
    const parity_label_t kp = automaton.GetLabel(p);
    const parity_label_t kq = automaton.GetLabel(q);
    const auto x = _EtaX(kp, kq, std::nullopt, PathRefinementAutomatonState::Equal);
    return PathRefinementAutomatonState{p, q, std::min(kp, kq), x};
}


}