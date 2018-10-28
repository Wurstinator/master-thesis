
#include "moore_ltk.h"
#include "../automaton/parity_util.h"

namespace tollk {

using namespace automaton;

EquivalenceRelation<state_t> Moore_LTK(const DPA& automaton, parity_label_t k) {
    EquivalenceRelation<state_t> labels = automaton.LabelEquivalence();

    // Find the equivalence classes for priority > k.
    std::vector<state_t> new_equivalences;
    for (const EquivalenceRelation<state_t>::EquivClass& clas : labels.Classes()) {
        const parity_label_t clas_priority = automaton.GetLabel(*clas.begin());
        if (clas_priority > k)
            new_equivalences.push_back(*clas.begin());
    }

    // Merge and refine.
    for (state_t p : new_equivalences)
        for (state_t q : new_equivalences)
            labels.AddConnection(p, q);
    automaton::RefineToCongruence(&labels, automaton);

    return labels;
}


EquivalenceRelation<state_t> AllMoore_LTK(const DPA& automaton) {
    std::unordered_map<parity_label_t, std::vector<state_t>> label_states;
    for (parity_label_t k : automaton.AllLabels())
        label_states.insert(std::make_pair(k, std::vector<state_t>()));
    for (state_t q : automaton.States())
        label_states[automaton.GetLabel(q)].push_back(q);

    std::vector<EquivalenceRelation<state_t>::EquivClass> equivalence_classes;
    for (const std::pair<const parity_label_t, std::vector<state_t>>& kv_pair : label_states) {
        const EquivalenceRelation<state_t> ltk = Moore_LTK(automaton, kv_pair.first);
        std::unordered_set<EquivalenceRelation<state_t>::ClassIndex> classes_with_prio_k;
        for (state_t q : kv_pair.second)
            classes_with_prio_k.insert(ltk.GetClassIndex(q));
        for (EquivalenceRelation<state_t>::ClassIndex i : classes_with_prio_k)
            equivalence_classes.push_back(ltk.Classes()[i]);
    }

    return EquivalenceRelation<state_t>(equivalence_classes);
}

}