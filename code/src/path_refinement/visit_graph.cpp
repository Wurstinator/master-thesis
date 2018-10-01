
#include "visit_graph.h"
#include "../automaton/parity_util.h"

namespace tollk {

using namespace automaton;

bool operator<(const VisitGraphState& lhs, const VisitGraphState& rhs) {
    if (lhs.state != rhs.state)
        return lhs.state < rhs.state;
    if (lhs.tracker != rhs.tracker)
        return lhs.tracker < rhs.tracker;
    return lhs.class_diff < rhs.class_diff;
}


DeterministicAutomaton PriorityVisitGraph(const DPA& automaton, const EquivalenceRelation<state_t>::EquivClass& lambda,
                                          boost::bimap<state_t, VisitGraphState>* state_indices_) {
    // Set up states.
    const auto in_lambda = [&lambda](state_t q) {return lambda.find(q) != lambda.end();};
    automaton::DeterministicAutomaton result(automaton.atomicPropositions);
    boost::bimap<automaton::state_t, VisitGraphState> state_indices;
    for (state_t q : automaton.States()) {
        for (parity_label_t k : automaton.AllLabels()) {
            if (in_lambda(q)) {
                for (parity_label_t l : automaton.AllLabels()) {
                    result.AddState(result.States().size());
                    state_indices.right.insert(std::make_pair(VisitGraphState {q, k, l}, result.States().size() - 1));
                }
            } else {
                result.AddState(result.States().size());
                state_indices.right.insert(std::make_pair(VisitGraphState {q, k, 0}, result.States().size() - 1));
            }
        }
    }

    // Set up transitions.
    for (state_t p : result.States()) {
        for (symbol_t s : result.Symbols()) {
            const VisitGraphState p_state = state_indices.left.at(p);
            VisitGraphState q_state;
            q_state.state = automaton.Succ(p_state.state, s);
            if (in_lambda(q_state.state)) {
                q_state.tracker = automaton.GetLabel(q_state.state);
                q_state.class_diff = std::min(automaton.GetLabel(q_state.state), p_state.tracker);
            } else {
                q_state.tracker = std::min(automaton.GetLabel(q_state.state), p_state.tracker);
                q_state.class_diff = 0;
            }
            const state_t q = state_indices.right.at(q_state);
            result.SetSucc(p, s, q);
        }
    }

    if (state_indices_ != nullptr)
        *state_indices_ = std::move(state_indices);
    return result;
}


EquivalenceRelation<automaton::state_t> VisitGraphVRelation(const DPA& dpa, const boost::bimap<automaton::state_t, VisitGraphState>& state_indices, const EquivalenceRelation<state_t>::EquivClass& lambda) {
    EquivalenceRelation<state_t> V;
    const std::unordered_set<parity_label_t> dpa_labels = dpa.AllLabels();

    // States that are not in \lambda.
    for (state_t p : dpa.States()) {
        if (lambda.find(p) == lambda.end()) {
            for (state_t q : dpa.States()) {
                if (lambda.find(q) == lambda.end()) {
                    for (parity_label_t l : dpa_labels) {
                        for (parity_label_t k: dpa_labels) {
                            const state_t pl = state_indices.right.at(VisitGraphState {p, l, 0});
                            const state_t qk = state_indices.right.at(VisitGraphState {q, k, 0});
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
            for (parity_label_t l : dpa_labels) {
                for (parity_label_t k : dpa_labels) {
                    for (parity_label_t i : dpa_labels) {
                        const state_t pli = state_indices.right.at(VisitGraphState{p, l, i});
                        const state_t qki = state_indices.right.at(VisitGraphState{q, k, i});
                        V.AddConnection(pli, qki);
                    }
                }
            }
        }
    }

    return V;
}


EquivalenceRelation<state_t>
PathRefinementEquivalence_VIS(const DPA& dpa, const EquivalenceRelation<state_t>::EquivClass& lambda) {
    // Compute A^\lambda_\text{visit}.
    boost::bimap<state_t, VisitGraphState> state_indices;
    const DeterministicAutomaton visit_graph = PriorityVisitGraph(dpa, lambda, &state_indices);

    // Compute V_M.
    EquivalenceRelation<state_t> V = VisitGraphVRelation(dpa, state_indices, lambda);
    RefineToCongruence(&V, visit_graph);

    // Build the path refinement relation.
    EquivalenceRelation<state_t> pr;
    const std::unordered_set<parity_label_t> dpa_labels = dpa.AllLabels();
    const parity_label_t max_prio = *std::max_element(dpa_labels.begin(), dpa_labels.end());

    for (state_t p : lambda) {
        for (state_t q : lambda) {
            const state_t pk = state_indices.right.at(VisitGraphState{p, dpa.GetLabel(p), max_prio});
            const state_t qk = state_indices.right.at(VisitGraphState{q, dpa.GetLabel(q), max_prio});
            if (V.IsEquiv(pk, qk))
                pr.AddConnection(p, q);
        }
    }

    return pr;
}



void PathRefinementMerge_VIS(automaton::DPA* dpa, const EquivalenceRelation<automaton::state_t>::EquivClass& lambda) {
    const EquivalenceRelation<automaton::state_t> pr = PathRefinementEquivalence_VIS(*dpa, lambda);
    std::function<automaton::parity_label_t(const EquivalenceRelation<automaton::state_t>::EquivClass&)> min_priority_selector =
            [dpa](const EquivalenceRelation<automaton::state_t>::EquivClass& kappa) {
                const auto compare_prio = [&dpa](automaton::state_t p, automaton::state_t q) {return dpa->GetLabel(p) < dpa->GetLabel(q);};
                const automaton::state_t min_state = *std::min_element(kappa.begin(), kappa.end(), compare_prio);
                return dpa->GetLabel(min_state);
            };
    QuotientAutomatonLabelled(dpa, pr, min_priority_selector);
}



}