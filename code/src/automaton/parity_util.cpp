
#include "util.h"


namespace tollk {
namespace automaton {


void Hopcroft(DPA* automaton) {
    EquivalenceRelation<state_t> labels = automaton->LabelEquivalence();
    automaton::RefineToCongruence(&labels, *automaton);
    QuotientAutomatonUnsafe(automaton, labels);
}


// Helper function for "NormalizePriorities". See TeX for more information about the algorithm.
// Returns (0 or 1), the new lowest priority after the normalization process. Only used internally.
parity_label_t _NormalizePriorities_M(NPA npa, std::map<state_t, parity_label_t>* new_priorities) {
    if (npa.States().empty())
        return 0;

    parity_label_t min = 0;
    SCCCollection sccs = StronglyConnectedComponents(npa);
    for (SCCCollection::SCC scc : sccs.sccs) {
        // Find the smallest priority in "scc".
        parity_label_t m = npa.GetLabel(*scc.begin());
        for (state_t q : scc)
            m = std::min(m, npa.GetLabel(q));

        // Find the states with the lowest priority.
        std::unordered_set<state_t> lowest_prio_states;
        for (state_t q : scc) {
            if (npa.GetLabel(q) == m) {
                (*new_priorities)[q] = m % 2;
                lowest_prio_states.insert(q);
            }
        }
        m %= 2;

        // Call this function recursively without the lowest priority.
        NPA sub_npa(npa);
        for (state_t q : lowest_prio_states)
            scc.erase(q);
        Subautomaton(&sub_npa, scc);
        const parity_label_t sub_m = _NormalizePriorities_M(sub_npa, new_priorities);

        if (m == 1 && sub_m == 0)
            for (state_t q : sub_npa.States())
                (*new_priorities)[q] += 2;

        min = std::min(min, m);
    }

    return min;
}


std::map<state_t, parity_label_t> NormalizePriorities(DPA dpa) {
    std::map<state_t, parity_label_t> new_priorities;
    for (state_t q : dpa.States())
        new_priorities[q] = dpa.GetLabel(q);
    _NormalizePriorities_M(NPA::FromDeterministic(dpa), &new_priorities);
    return new_priorities;
}


}
}
