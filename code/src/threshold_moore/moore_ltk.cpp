
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

}