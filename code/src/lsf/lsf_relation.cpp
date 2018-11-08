
#include "lsf_relation.h"
#include "../automaton/util.h"
#include "../threshold_moore/moore_ltk.h"

namespace tollk {

using namespace automaton;

// Returns a set of those states which have priority greater than k.
std::unordered_set<state_t> _gtk_states(const ParityAutomaton& automaton, parity_label_t k) {
    std::unordered_set<state_t> result;
    for (state_t q : automaton.States())
        if (automaton.GetLabel(q) > k)
            result.insert(q);
    return result;
}

// Two states are equivalent if their priority is both greater than k.
EquivalenceRelation<state_t> _gtk_relation(const DPA& automaton, parity_label_t k) {
    EquivalenceRelation<state_t> result;
    for (state_t q : automaton.States())
        result.AddConnection(q, q);

    const std::unordered_set<state_t> gtk_states = _gtk_states(automaton, k);
    for (state_t q : gtk_states)
        result.AddConnection(*gtk_states.begin(), q);

    return result;
}

EquivalenceRelation<state_t> LSFBaseRelation(const DPA& automaton, parity_label_t k) {
    return EquivalenceRelation<state_t>::Intersection(
            EquivalenceRelation<state_t>::Intersection(
                    LanguageEquivalentStates(automaton),
                    Moore_LTK(automaton, k)
            ),
            _gtk_relation(automaton, k)
            );
}

std::unordered_map<state_t, std::unordered_set<state_t>> LSFMergeClasses(const DPA& automaton, parity_label_t k) {
    // Build the subautomaton limited to states with priority greater than k and find its SCCs.
    NPA gtk_subautomaton = NPA::FromDeterministic(automaton);
    Subautomaton(&gtk_subautomaton, _gtk_states(gtk_subautomaton, k));
    SCCCollection subautomaton_sccs;
    MergeSCCs(&gtk_subautomaton, &subautomaton_sccs);

    // Find a topological sorting of the SCCs.
    const std::vector<state_t> scc_sorting = TopologicalSorting(gtk_subautomaton);

    // Iterate through all equivalence classes from the base relation.
    std::unordered_map<state_t, std::unordered_set<state_t>> result;
    for (const EquivalenceRelation<state_t>::EquivClass& clas : LSFBaseRelation(automaton, k).Classes()) {
        // Skip if the class contains only one element.
        if (clas.size() == 1)
            continue;

        // Find the "deepest" SCC that contains an element from the class.
        const auto is_in_clas = [&clas](state_t q) {return clas.find(q) != clas.end();};
        size_t deepest_scc = 0;
        for (size_t i = 0; i < scc_sorting.size(); ++i) {
            const SCCCollection::SCC& scc = subautomaton_sccs.sccs[subautomaton_sccs.scc_indices[scc_sorting[i]]];
            if (std::any_of(scc.begin(), scc.end(), is_in_clas))
                deepest_scc = i;
        }

        // Copy all elements from the class that lie in a different SCC to the result set.
        std::unordered_set<state_t> lambda_prime;
        for (state_t q : clas)
            if (subautomaton_sccs.scc_indices[q] < deepest_scc)
                lambda_prime.insert(q);

        // Choose a representative.
        const SCCCollection::SCC& scc = subautomaton_sccs.sccs[subautomaton_sccs.scc_indices[scc_sorting[deepest_scc]]];
        const state_t representative = *std::find_if(scc.begin(), scc.end(), is_in_clas);
        lambda_prime.insert(representative);

        // Insert the set to the result map.
        if (lambda_prime.size() > 1)
            result[representative] = std::move(lambda_prime);
    }

    return result;
}


void LSFPerformMerge(DPA* automaton, parity_label_t k) {
    const std::unordered_map<state_t, std::unordered_set<state_t>> merge_classes = LSFMergeClasses(*automaton, k);
    for (const std::pair<const state_t, std::unordered_set<state_t>>& kv_pair : merge_classes) {
        const state_t representative = kv_pair.first;
        const std::unordered_set<state_t>& clas = kv_pair.second;
        automaton->MergeStates(clas, representative);
    }
}


}