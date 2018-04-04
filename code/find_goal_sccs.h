
#pragma once


#include <swa.hh>

// Given an SCC, determines whether it is trivial.
template <typename TagT>
bool SCCIsTrivial(const std::set<nbautils::state_t>& scc, const nbautils::SWA<TagT>& automaton) {
    if (scc.size() > 1)
        return false;
    const nbautils::state_t q = *scc.begin();
    const std::vector<nbautils::state_t> q_successors = automaton.succ(q);
    return std::find(q_successors.begin(), q_successors.end(), q) != q_successors.end();
}

// Given an SCC, determines whether it contains an accepting state.
// TODO replace vector by range?
bool SCCContainsAccepting(const std::set<nbautils::state_t>& scc, const std::set<nbautils::state_t>& accepting_states);

// Given a Büchi automaton, returns a list of SCCs that non-trivial and contain an accepting state.
template <typename TagT>
std::vector<std::set<nbautils::state_t>> MergeSCCs(const nbautils::SWA<TagT>& automaton) {
    // Find SCCs.
    nbautils::SCCDat<nbautils::state_t>::uptr sccs_raw = nbautils::get_sccs(
            automaton.states(),
            [&](nbautils::state_t q) {return automaton.succ(q);});

    // Create a vector of all SCCs.
    std::vector<std::set<nbautils::state_t>> sccs;
    std::transform(
            sccs_raw->sccs.begin(),
            sccs_raw->sccs.end(),
            std::back_inserter(sccs),
            [](const std::vector<nbautils::state_t>& scc) {return std::set<nbautils::state_t>(scc.begin(), scc.end());}
            );

    // Filter trivial SCCs.
    sccs.erase(std::remove_if(sccs.begin(), sccs.end(), std::bind2nd(&SCCIsTrivial, automaton)), sccs.end());

    // Filter SCCs without an accepting state.
    const std::set<nbautils::state_t> accepting_states; //TODO
    const auto not_accepting = [](const std::set<nbautils::state_t>& scc) {return !SCCContainsAccepting(scc, accepting_states);};
    sccs.erase(std::remove_if(sccs.begin(), sccs.end(), not_accepting), sccs.end());

    return sccs;
}