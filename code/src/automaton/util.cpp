
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



}
}
