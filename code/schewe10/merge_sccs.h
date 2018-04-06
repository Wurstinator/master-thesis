
#pragma once

#include "swa.hh"
#include <set>
#include "common/scc.hh"
#include <algorithm>
#include "merge_states.h"


// Given an automaton, creates a new automaton by merging all SCCs.
// The new tags are sets of old tags.
// If the second parameter is not null, the underlying map object is set so that each SCC is mapped to its
// representative.
// TODO there is probably some error in this function
template <typename TagT>
nbautils::SWA<std::set<TagT>> MergeSCCs(nbautils::SWA<TagT> automaton,
                                        std::map<std::set<nbautils::state_t>, nbautils::state_t>* representatives = nullptr) {
    // Find SCCs.
    nbautils::SCCDat<nbautils::state_t>::uptr sccs = nbautils::get_sccs(
          automaton.states(),
          [&](nbautils::state_t q) {return automaton.succ(q);});

    // Merge SCCs.
    std::map<std::set<nbautils::state_t>, nbautils::state_t> representatives_;
    for (const std::vector<nbautils::state_t>& scc : sccs->sccs) {
        std::set<nbautils::state_t> scc_set(scc.begin(), scc.end());
        representatives_[scc_set] = MergeStates(&automaton, scc_set);
    }

    // Create new automaton with renamed states.
    nbautils::SWA<std::set<TagT>> result(automaton.get_acceptance(), automaton.get_name(), automaton.get_aps());

    // Add the states with appropriate tags.
    for (const std::pair<const std::set<nbautils::state_t>, nbautils::state_t>& scc_rep : representatives_) {
        std::set<TagT> tag_set;
        for (nbautils::state_t q : scc_rep.first)
            if (automaton.tag->hasi(q))
                tag_set.insert(automaton.tag->geti(q));
        result.tag->put(std::move(tag_set), result.num_states());
        result.add_state(result.num_states());
    }

    // Add transitions between the states.
    nbautils::state_t merged_state = 0;
    for (const std::pair<const std::set<nbautils::state_t>, nbautils::state_t>& scc_rep : representatives_) {
        for (nbautils::sym_t sym = 0; sym < result.num_syms(); ++sym) {
            std::vector<nbautils::state_t> succs = automaton.succ(scc_rep.second, sym);
            for (nbautils::state_t& q : succs)
                q = sccs->scc_of[q];
            result.set_succs(merged_state, sym, succs);
        }
    }

    if (representatives != nullptr)
        *representatives = std::move(representatives_);

    return result;
}
