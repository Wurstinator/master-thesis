
#include "find_goal_sccs.h"


// Given an SCC, determines whether it contains an accepting state.
bool SCCContainsAccepting(const std::set<nbautils::state_t>& scc, const std::set<nbautils::state_t>& accepting_states) {
    const auto is_accepting = [&accepting_states](nbautils::state_t q) {
        return accepting_states.find(q) != accepting_states.end();
    };
    return std::any_of(scc.begin(), scc.end(), is_accepting);
}