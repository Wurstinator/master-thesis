
#pragma once

#include "swa.hh"
#include <map>

// Given an automaton, returns the same automaton but with all edges/transitions inverted.
template <typename TagT>
nbautils::SWA<TagT> BackwardsAutomaton(nbautils::SWA<TagT> automaton) {
    std::map<std::pair<nbautils::state_t, nbautils::sym_t>, std::vector<nbautils::state_t>> predecessors;
    for (nbautils::state_t q : automaton.states()) {
        for (nbautils::sym_t sym = 0; sym < automaton.num_syms(); ++sym) {
            for (nbautils::state_t p : automaton.succ(q, sym)) {
                predecessors[std::make_pair(p, sym)].push_back(q);
            }
        }
    }

    for (nbautils::state_t q : automaton.states()) {
        for (nbautils::sym_t sym = 0; sym < automaton.num_syms(); ++sym) {
            automaton.set_succs(q, sym, predecessors[std::make_pair(q, sym)]);
        }
    }
    return automaton;
}