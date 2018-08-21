
#include "iterated_moore.h"

namespace tollk {

using namespace automaton;

EquivalenceRelation<automaton::state_t> IteratedMooreRelation(const DPA& automaton) {
    // Sort the SCCs.
    NondeterministicAutomaton merged_sccs = NondeterministicAutomaton::FromTransitionAutomaton(automaton);
    SCCCollection sccs;
    MergeSCCs(&merged_sccs, &sccs);
    const std::vector<unsigned int> scc_sorting = TopologicalSorting(merged_sccs);

    // Iteratively add the SCCs.
    DPA result_dpa(automaton.atomicPropositions);
    for (unsigned int scc_index : scc_sorting) {
        // Add the SCC.
        result_dpa.SetInitialState(*sccs.sccs[scc_index].begin());
        for (state_t q : sccs.sccs[scc_index]) {
            result_dpa.AddState(q);
            for (symbol_t sym : automaton.Symbols())
                result_dpa.SetSucc(q, sym, automaton.Succ(q, sym));
            result_dpa.SetLabel(q, automaton.GetLabel(q));
        }

        // Check if the new SCC is trivial. Otherwise, nothing is left to do.
        if (!SCCIsTrivial(result_dpa, sccs.sccs[scc_index]))
            continue;
        const state_t trivial_state = *sccs.sccs[scc_index].begin();

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
    }


    // Return the Moore equivalence of the new automaton.
    EquivalenceRelation<state_t> result_relation = result_dpa.LabelEquivalence();
    RefineToCongruence(&result_relation, result_dpa);
    return result_relation;
}

}