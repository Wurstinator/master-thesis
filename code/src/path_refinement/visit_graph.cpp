
#include "visit_graph.h"
#include "../automaton/parity_util.h"

namespace tollk {

using namespace automaton;

DeterministicAutomaton PriorityVisitGraph(const DPA& automaton, const EquivalenceRelation<state_t>::EquivClass& lambda,
                                          boost::bimap<state_t, std::pair<state_t, parity_label_t>>* state_indices_) {
    // Set up states.
    automaton::DeterministicAutomaton result(automaton.atomicPropositions);
    boost::bimap<automaton::state_t, std::pair<automaton::state_t, automaton::parity_label_t>> state_indices;
    for (state_t q : automaton.States()) {
        for (parity_label_t k : automaton.AllLabels()) {
            result.AddState(result.States().size());
            state_indices.right.insert(std::make_pair(std::make_pair(q, k), result.States().size() - 1));
        }
    }

    // Set up transitions.
    for (state_t p : result.States()) {
        for (symbol_t s : result.Symbols()) {
            const state_t p_state = state_indices.left.at(p).first;
            const parity_label_t p_prio = state_indices.left.at(p).second;
            const state_t q_state = automaton.Succ(p_state, s);
            const bool p_in_lambda = (lambda.find(p_state) != lambda.end());
            const parity_label_t q_prio = (p_in_lambda ? automaton.GetLabel(p_state) : std::min(
                    automaton.GetLabel(p_state), p_prio));
            const state_t q = state_indices.right.at(std::make_pair(q_state, q_prio));
            result.SetSucc(p, s, q);
        }
    }

    if (state_indices_ != nullptr)
        *state_indices_ = std::move(state_indices);
    return result;
}


EquivalenceRelation<state_t>
PathRefinementEquivalence_VIS(const DPA& dpa, const EquivalenceRelation<state_t>::EquivClass& lambda) {
    // Construct the priority visit graph.
    boost::bimap<state_t, std::pair<state_t, parity_label_t>> state_indices;
    const DeterministicAutomaton priority_visit_graph = PriorityVisitGraph(dpa, lambda, &state_indices);
    const std::unordered_set<parity_label_t> dpa_labels = dpa.AllLabels();

    // Set up the relation "V".
    EquivalenceRelation<state_t> V;

    // States that are not in \lambda.
    for (state_t p : dpa.States()) {
        if (lambda.find(p) == lambda.end()) {
            for (state_t q : dpa.States()) {
                if (lambda.find(q) == lambda.end()) {
                    for (parity_label_t l : dpa_labels) {
                        for (parity_label_t k: dpa_labels) {
                            const state_t pl = state_indices.right.at(std::make_pair(p, l));
                            const state_t qk = state_indices.right.at(std::make_pair(q, k));
                            V.AddConnection(pl, qk);
                        }
                    }
                }
            }
        }
    }

    // States that are in \lambda.
    for (state_t p : lambda) {
        for (state_t q : lambda) {
            for (parity_label_t k: dpa_labels) {
                const state_t pk = state_indices.right.at(std::make_pair(p, k));
                const state_t qk = state_indices.right.at(std::make_pair(q, k));
                V.AddConnection(pk, qk);
            }
        }
    }

    // Compute V_M.
    RefineToCongruence(&V, priority_visit_graph);

    // Now build the path refinement relation.
    EquivalenceRelation<state_t> pr;
    const parity_label_t max_prio = *std::max_element(dpa_labels.begin(), dpa_labels.end());

    for (state_t p : lambda) {
        for (state_t q : lambda) {
            const state_t pk = state_indices.right.at(std::make_pair(p, max_prio));
            const state_t qk = state_indices.right.at(std::make_pair(q, max_prio));
            if (V.IsEquiv(pk, qk))
                pr.AddConnection(p, q);
        }
    }

    return pr;
}


}