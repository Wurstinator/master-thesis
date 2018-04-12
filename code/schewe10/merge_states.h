
#pragma once

#include "swa.hh"

// Computes the representative of a set. If one of the states is an initial state, it is picked. Otherwise the
// representative is an arbitrary element.
template <typename TagT, typename IterT>
nbautils::state_t FindMergeRepresentative(const nbautils::SWA<TagT>& automaton, IterT first_state, IterT last_state) {
    auto representative_iter = std::find_if(first_state, last_state, [&automaton](nbautils::state_t q) { return automaton.is_init(q); });
    if (representative_iter != last_state)
        return *representative_iter;
    else
        return *first_state;
}


// Merges a set of states in an automaton, also accounting for their acceptance sets.
// The new representative is returned.
template <typename TagT>
nbautils::state_t MergeStates(nbautils::SWA<TagT>* automaton, std::set<nbautils::state_t> merge_states) {
    assert(!merge_states.empty());

    // Move all acceptance statuses into the representative.
    std::vector<nbautils::acc_t> acceptance;
    for (nbautils::state_t merge_state : merge_states)
        for (nbautils::acc_t acc : automaton->get_accs(merge_state))
            acceptance.push_back(acc);

    // Remove the representative from the set.
    nbautils::state_t representative = FindMergeRepresentative(*automaton, merge_states.begin(), merge_states.end());
    merge_states.erase(merge_states.find(representative));

    automaton->set_accs(representative, acceptance);

    // Merge.
    automaton->merge_states(std::vector<nbautils::state_t>(merge_states.begin(), merge_states.end()), representative);
    return representative;
}