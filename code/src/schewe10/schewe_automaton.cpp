
#include "schewe_automaton.h"

#include "almost_equivalent_states.h"
#include "../automaton/util.h"
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <string>

namespace tollk {

// Computes a Schewe automaton of a given deterministic (!) parity automaton. The function performs minimization neither
// before nor after the construction.
void ScheweAutomaton(automaton::DPA* automaton) {
    using namespace tollk::automaton;

    // Compute priority almost-equivalent states.
    const EquivalenceRelation<state_t> almost_equivalent_states = PriorityAlmostEquivalence(*automaton);

    // Merge the SCCs and compute a topological sorting on them.
    NondeterministicAutomaton merged_sccs = NondeterministicAutomaton::FromTransitionAutomaton(*automaton);
    SCCCollection automaton_sccs;
    MergeSCCs(&merged_sccs, &automaton_sccs);
    const std::vector<state_t> scc_sorting = TopologicalSorting(merged_sccs);

    // For each state, compute its position in the sorting (i.e. the position of its SCC).
    std::map<state_t, std::vector<state_t>::size_type> state_sorting;
    for (std::vector<state_t>::size_type i = 0; i < scc_sorting.size(); ++i) {
        for (state_t q : automaton_sccs.sccs[automaton_sccs.scc_indices[scc_sorting[i]]]) {
            state_sorting[q] = i;
        }
    }

    // For each equivalence class (priority almost-equivalence), find an element that is maximal w.r.t. the ordering.
    std::map<EquivalenceRelation<automaton::state_t>::EquivClass, state_t> representatives;
    for (const EquivalenceRelation<automaton::state_t>::EquivClass& clas : almost_equivalent_states.Classes()) {
        const auto compare_topological = [&state_sorting](state_t lhs, state_t rhs) {
            return state_sorting[lhs] < state_sorting[rhs];
        };
        representatives[clas] = *std::max_element(clas.begin(), clas.end(), compare_topological);
    }

    // Redirect transitions.
    for (state_t p : automaton->States()) {
        for (symbol_t s : automaton->Symbols()) {
            const state_t q = automaton->Succ(p, s);
            const state_t representative = representatives[almost_equivalent_states.GetClass(q)];
            if (state_sorting[q] < state_sorting[representative])
                automaton->SetSucc(p, s, representative);
        }
    }
}




}
