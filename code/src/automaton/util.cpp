
#include "util.h"




namespace tollk {
namespace automaton {



void MergeSCCs(NondeterministicAutomaton* automaton, SCCCollection* sccs) {
    SCCCollection sccs_ = StronglyConnectedComponents(*automaton);
    for (const SCCCollection::SCC& scc : sccs_.sccs) {
        automaton->MergeStates(scc);
    }
    if (sccs != nullptr)
        *sccs = std::move(sccs_);
}


void Hopcroft(DPA* automaton) {
    EquivalenceRelation<state_t> labels = automaton->LabelEquivalence();
    automaton::RefineToCongruence(&labels, *automaton);
    QuotientAutomatonUnsafe(automaton, labels);
}


DeterministicAutomaton ProductAutomaton(const DeterministicAutomaton& automaton1, const DeterministicAutomaton& automaton2, boost::bimap<state_t, std::pair<state_t, state_t>>* pair_indices) {
    assert(automaton1.atomicPropositions == automaton2.atomicPropositions);

    // Fill the map of state pairs.
    boost::bimap<state_t, std::pair<state_t, state_t>> pair_indices_;
    for (state_t p : automaton1.States())
        for (state_t q : automaton2.States())
            pair_indices_.left.insert(std::make_pair(pair_indices_.size(), std::make_pair(p, q)));

    // Create the product automaton.
    DeterministicAutomaton product(automaton1.atomicPropositions);
    for (const auto& kv_pair : pair_indices_.left)
        product.AddState(kv_pair.first);
    product.SetInitialState(pair_indices_.right.at(std::make_pair(automaton1.InitialState(), automaton2.InitialState())));

    // Set the transitions.
    for (state_t p : automaton1.States()) {
        for (state_t q : automaton2.States()) {
            for (symbol_t s : product.Symbols()) {
                const state_t succ1 = automaton1.Succ(p, s);
                const state_t succ2 = automaton2.Succ(q, s);
                product.SetSucc(pair_indices_.right.at(std::make_pair(p, q)), s, pair_indices_.right.at(std::make_pair(succ1, succ2)));
            }
        }
    }

    if (pair_indices != nullptr)
        *pair_indices = std::move(pair_indices_);
    return product;
}


void QuotientAutomaton(NondeterministicAutomaton* automaton, const EquivalenceRelation<state_t>& relation) {
    for (EquivalenceRelation<state_t>::EquivClass c : relation.Classes())
        automaton->MergeStates(c);
}


void QuotientAutomatonUnsafe(DeterministicAutomaton* automaton, const EquivalenceRelation<state_t>& relation) {
    for (EquivalenceRelation<state_t>::EquivClass c : relation.Classes())
        automaton->MergeStates(c);
}


void Subautomaton(NondeterministicAutomaton* automaton, const std::unordered_set<state_t>& newStateSet) {
    std::unordered_set<state_t> remove_states(automaton->States().begin(), automaton->States().end());
    for (state_t p : newStateSet)
        remove_states.erase(p);
    for (state_t p : remove_states)
        automaton->RemoveState(p);
    for (state_t p : automaton->States()) {
        for (symbol_t s : automaton->Symbols()) {
            for (state_t q : remove_states) {
                automaton->RemoveSucc(p, s, q);
            }
        }
    }
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
        const parity_label_t sub_m =_NormalizePriorities_M(sub_npa, new_priorities);

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
