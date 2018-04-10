
#pragma once

#include "almost_equivalent_states.h"
#include "topological_ordering.h"
#include "swa.hh"
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <string>


// Computes a Schewe automaton of a given deterministic (!) parity automaton. The function performs minimization neither
// before nor after the construction.
template<typename TagT>
void ScheweAutomaton(nbautils::SWA<TagT>* automaton) {
    // Compute priority almost-equivalent states.
    const EquivalenceRelation<nbautils::state_t> almost_equivalent_states = PriorityAlmostEquivalence(*automaton);

    // Compute SCCs.
    nbautils::SCCDat<nbautils::state_t>::uptr sccs = nbautils::get_sccs(
            automaton->states(),
            [&](nbautils::state_t q) {return automaton->succ(q);});

    // Merge the SCCs.
    std::map<std::set<nbautils::state_t>, nbautils::state_t> scc_representatives;
    nbautils::SWA<TagT> merged_sccs = MergeSCCs(*automaton, &scc_representatives);

    // From the SCC-Repr. map, also compute a Repr.-SCC map for later.
    std::map<nbautils::state_t, std::set<nbautils::state_t>> representative_sccs;
    for (const std::pair<const std::set<nbautils::state_t>, nbautils::state_t>& kv_pair : scc_representatives)
        representative_sccs.insert(std::make_pair(kv_pair.second, kv_pair.first));

    // Compute a topological sorting on the SCCs.
    const std::vector<nbautils::state_t> scc_states = merged_sccs.states();
    const std::vector<nbautils::state_t> topological_ordering =
            TopologicalSorting(scc_states.begin(),
                               scc_states.end(),
                               [&merged_sccs](nbautils::state_t q) {
                                   return merged_sccs.succ(q);
                               });

    // For each class, find an element that is maximal w.r.t to the ordering.
    std::map<EquivalenceRelation<nbautils::state_t>::EquivClass, nbautils::state_t> representatives;
    for (nbautils::state_t scc : topological_ordering)
        for (nbautils::state_t q : representative_sccs[scc])
            representatives[almost_equivalent_states.GetClass(q)] = q;

    // Build the "inverted" vector of the ordering.
    std::map<nbautils::state_t, std::vector<nbautils::state_t>::size_type> topological_ordering_pos;
    for (std::vector<nbautils::state_t>::size_type i = 0; i < topological_ordering.size(); ++i)
        topological_ordering_pos[topological_ordering[i]] = i;

    // Given a state in "automaton", returns the position of the SCC of that state in the topological ordering.
    const auto ordering_pos = [&sccs,&scc_representatives,&topological_ordering_pos](nbautils::state_t q) {
       const std::vector<nbautils::state_t>& scc_vec = sccs->sccs[sccs->scc_of[q]];
       const std::set<nbautils::state_t> scc(scc_vec.begin(), scc_vec.end());
       return topological_ordering_pos[scc_representatives[scc]];
    };

    // Redirect transitions.
    for (nbautils::state_t q : automaton->states()) {
        for (nbautils::sym_t sym = 0; sym < automaton->num_syms(); ++sym) {
            const nbautils::state_t successor = automaton->succ(q, sym)[0]; // we assume a deterministic automaton
            const nbautils::state_t succ_representative = representatives[almost_equivalent_states.GetClass(successor)];
            if (ordering_pos(succ_representative) > ordering_pos(q)) {
                automaton->set_succs(q, sym, std::vector<nbautils::state_t> {succ_representative});
            }
        }
    }
}