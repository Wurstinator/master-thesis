
#pragma once

#include "swa.hh"
#include <set>
#include "common/scc.hh"
#include <algorithm>
#include "merge_states.h"


// Given an automaton, creates a new automaton by merging all SCCs.
// The new tags are sets of old tags.
template <typename TagT>
nbautils::SWA<std::set<TagT>> MergeSCCs(nbautils::SWA<TagT> automaton) {
    // Find SCCs.
    nbautils::SCCDat<nbautils::state_t>::uptr sccs = nbautils::get_sccs(
          automaton.states(),
          [&](nbautils::state_t q) {return automaton.succ(q);});

    // Merge SCCs.
    std::map<std::vector<nbautils::state_t>, nbautils::state_t> representatives;
    for (const std::vector<nbautils::state_t>& scc : sccs->sccs) {
        representatives[scc] = MergeStates(&automaton, std::set<nbautils::state_t>(scc.begin(), scc.end()));
    }

    // Create new automaton with renamed states.
    nbautils::SWA<std::set<TagT>> result(automaton.get_acceptance(), automaton.get_name(), automaton.get_aps());

    // Add the states with appropriate tags.
    for (const std::vector<nbautils::state_t>& scc : sccs->sccs) {
        const std::set<TagT> scc_set(scc.begin(), scc.end());
        result.tag->put(std::move(scc_set), result.num_states());
        result.add_state(result.num_states());
    }

    // Add transitions between the states.
    nbautils::state_t merged_state = 0;
    for (const std::vector<nbautils::state_t>& scc : sccs->sccs) {
        for (nbautils::sym_t sym = 0; sym < result.num_syms(); ++sym) {
            std::vector<nbautils::state_t> succs = automaton.succ(representatives[scc], sym);
            for (nbautils::state_t& q : succs)
                q = sccs->scc_of[q];
            result.set_succs(merged_state, sym, succs);
        }
    }

    return result;
}
