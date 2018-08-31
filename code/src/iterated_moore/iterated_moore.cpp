
#include "iterated_moore.h"

namespace tollk {

using namespace automaton;

std::unordered_map<automaton::state_t, automaton::parity_label_t> IteratedMooreLabels(const automaton::DPA& automaton) {
    std::unordered_map<automaton::state_t, automaton::parity_label_t> new_labels;

    // Sort the SCCs.
    NondeterministicAutomaton merged_sccs = NondeterministicAutomaton::FromTransitionAutomaton(automaton);
    SCCCollection sccs;
    MergeSCCs(&merged_sccs, &sccs);
    const std::vector<state_t> scc_sorting = TopologicalSorting(merged_sccs);

    // Iteratively add the SCCs.
    DPA result_dpa(automaton.atomicPropositions);
    for (state_t scc_rep : scc_sorting) {
        const SCCCollection::SCC& scc = sccs.sccs[sccs.scc_indices[scc_rep]];
        // Add the SCC.
        result_dpa.SetInitialState(scc_rep);
        for (state_t q : scc) {
            result_dpa.AddState(q);
            for (symbol_t sym : automaton.Symbols())
                result_dpa.SetSucc(q, sym, automaton.Succ(q, sym));
            result_dpa.SetLabel(q, automaton.GetLabel(q));
        }

        // Check if the new SCC is trivial. Otherwise, nothing is left to do.
        if (!SCCIsTrivial(result_dpa, scc))
            continue;
        const state_t trivial_state = scc_rep;

        // Compute the Moore equivalence for the current automaton.
        EquivalenceRelation<state_t> moore_relation = result_dpa.LabelEquivalence();
        RefineToCongruence(&moore_relation, result_dpa);

        if (moore_relation.GetClass(trivial_state).size() > 1)
            continue;

        // No state that is equivalent to the newly added state was found. Check if the priority can be changed.
        // TODO improve this
        for (parity_label_t parity : result_dpa.AllLabels()) {
            result_dpa.SetLabel(trivial_state, parity);
            moore_relation = result_dpa.LabelEquivalence();
            RefineToCongruence(&moore_relation, result_dpa);
            if (moore_relation.GetClass(trivial_state).size() > 1)
                break;
        }
        if (moore_relation.GetClass(trivial_state).size() == 1)
            result_dpa.SetLabel(trivial_state, automaton.GetLabel(trivial_state));
        else
            new_labels[trivial_state] = result_dpa.GetLabel(trivial_state);
    }

    return new_labels;
}


void IteratedMooreQuotient(automaton::DPA* automaton) {
    const std::unordered_map<automaton::state_t, automaton::parity_label_t> new_labels = IteratedMooreLabels(*automaton);

    for (const std::pair<const automaton::state_t, automaton::parity_label_t>& kv_pair : new_labels)
        automaton->SetLabel(kv_pair.first, kv_pair.second);

    EquivalenceRelation<automaton::state_t> moore = automaton->LabelEquivalence();
    RefineToCongruence(&moore, *automaton);
    automaton::QuotientAutomatonUnsafe(automaton, moore);
}


}