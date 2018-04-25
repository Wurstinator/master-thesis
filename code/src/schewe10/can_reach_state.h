
#pragma once

#include "swa.hh"
#include "common/graph.hh"
#include "backwards_automaton.h"

// Given an automaton and a state, returns a list of states which can reach that state.
template <typename TagT>
std::vector<nbautils::state_t> CanReachState(const nbautils::SWA<TagT>& automaton, nbautils::state_t target_state) {
    const nbautils::SWA<TagT> back_aut = BackwardsAutomaton(automaton);
    const nbautils::succ_fun<nbautils::state_t> foo = [&back_aut](nbautils::state_t q) {return back_aut.succ(q);};
    return nbautils::reachable_states(target_state, foo);
}

// Given an automaton and a state, returns a list of states which cannot reach that state.
template <typename TagT>
std::vector<nbautils::state_t> CannotReachState(const nbautils::SWA<TagT>& automaton, nbautils::state_t target_state) {
    const nbautils::SWA<TagT> back_aut = BackwardsAutomaton(automaton);
    const nbautils::succ_fun<nbautils::state_t> foo = [&back_aut](nbautils::state_t q) {return back_aut.succ(q);};
    return nbautils::unreachable_states(automaton.states(), target_state, foo);
}