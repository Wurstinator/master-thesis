
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


// Erase all elements which satisfy "Predicate" from an unordered_set.
template <class Set, class Predicate>
void discard_if(Set* c, Predicate pred) {
    for (auto it{c->begin()}, end{c->end()}; it != end; ) {
        if (pred(*it))
            it = c->erase(it);
        else
            ++it;
    }
}


EquivalenceRelation<state_t> LanguageEquivalentStates(const DPA& dpa) {
    // Build the product automaton with pairs of priorities as labels.
    boost::bimap<state_t, std::pair<state_t, state_t>> pair_indices;
    NondeterministicAutomaton product = ProductAutomaton(dpa, dpa, &pair_indices);

    // For each pair of priorities, (k, l), remove all states from the product automaton which have first priority
    // < k or second priority < l.
    const std::unordered_set<parity_label_t> all_labels = dpa.AllLabels();
    const parity_label_t max_priority = *std::max_element(all_labels.begin(), all_labels.end());
    std::map<std::pair<parity_label_t, parity_label_t>, NondeterministicAutomaton> sub_automata;
    std::unordered_set<state_t> first_prio_ge_k(product.States().begin(), product.States().end());

    for (parity_label_t k = 0; k <= max_priority; ++k) {
        const auto prio_lt_k = [&dpa, &pair_indices, k](state_t q) {return dpa.GetLabel(pair_indices.left.at(q).first) < k;};
        discard_if(&first_prio_ge_k, prio_lt_k);
        Subautomaton(&product, first_prio_ge_k);
        NondeterministicAutomaton product_copy = product;
        std::unordered_set<state_t> second_prio_ge_l(product.States().begin(), product.States().end());

        for (parity_label_t l = 0; l <= max_priority; ++l) {
            const auto prio_lt_l = [&dpa, &pair_indices, k](state_t q) {return dpa.GetLabel(pair_indices.left.at(q).second) < k;};
            discard_if(&second_prio_ge_l, prio_lt_l);
            Subautomaton(&product_copy, second_prio_ge_l);
            sub_automata.insert(std::make_pair(std::make_pair(k, l), product_copy));
        }
    }

    // Two states are NOT language equivalent, if there is a non-trivial SCC in one of the sub-automata such that
    // the parity of the lowest first priority and the lowest second priority differs.

    // First, collect all non-trivial SCCs that occur in any of the subautomata.
    std::set<std::set<state_t>> all_sccs;
    for (const std::pair<const std::pair<parity_label_t, parity_label_t>, NondeterministicAutomaton>& kv_pair : sub_automata) {
        const SCCCollection sccs = StronglyConnectedComponents(kv_pair.second);
        for (const SCCCollection::SCC& scc : sccs.sccs)
            if (!SCCIsTrivial(kv_pair.second, scc))
                all_sccs.insert(std::set<state_t>(scc.begin(), scc.end()));
    }

    // Now remove all SCCs from the set in which the lowest first and second priorities are of the same parity.
    const auto first_second_prio_is_same = [&dpa, &pair_indices](const std::set<state_t>& scc) {
        std::unordered_set<parity_label_t> first_prios, second_prios;
        for (state_t q : scc) {
            first_prios.insert(dpa.GetLabel(pair_indices.left.at(q).first));
            second_prios.insert(dpa.GetLabel(pair_indices.left.at(q).second));
        }
        const parity_label_t min_first_prio = *std::min_element(first_prios.begin(), first_prios.end());
        const parity_label_t min_second_prio = *std::min_element(second_prios.begin(), second_prios.end());
        return min_first_prio == min_second_prio;
    };
    discard_if(&all_sccs, first_second_prio_is_same);

    // Finally, collect all states that are not in any of the leftover SCCs.
    std::vector<state_t> non_equivalent_states;
    for (const std::set<state_t>& scc : all_sccs)
        std::copy(scc.begin(), scc.end(), std::back_inserter(non_equivalent_states));

    std::set<std::pair<state_t, state_t>> equivalent_states;
    for (state_t p : dpa.States())
        for (state_t q : dpa.States())
            equivalent_states.insert(std::make_pair(p, q));
    for (state_t q : non_equivalent_states)
        equivalent_states.erase(pair_indices.left.at(q));

    // Collect the equivalent states in a relation.
    EquivalenceRelation<state_t> result;
    for (const std::pair<state_t, state_t>& pair : equivalent_states)
        result.AddConnection(pair.first, pair.second);
    return result;
}


}
}
