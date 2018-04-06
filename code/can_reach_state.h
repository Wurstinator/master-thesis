
#pragma once

#include "swa.hh"

// Given an automaton and a state, returns a list of states which can reach that state.
template <typename TagT>
std::vector<nbautils::state_t> CanReachState(const nbautils::SWA<TagT>& automaton, nbautils::state_t target_state) {
    //TODO
}