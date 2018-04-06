
#pragma once

#include "swa.hh"
#include "equivalence_relation.h"
#include "product_automaton.h"
#include "merge_sccs.h"
#include "find_goal_sccs.h"
#include "can_reach_state.h"

// Given a parity automaton, returns the equivalence relation of priority almost-equivalent states.
template <typename TagT>
EquivalenceRelation<nbautils::state_t> PriorityAlmostEquivalence(const nbautils::SWA<TagT>& automaton) {
    // Build the product automaton of the automaton with itself. It is a Büchi automaton / DFA in which the
    // accepting states are those in which the priority of the components differs.
    nbautils::SWA<std::pair<TagT, TagT>> product = ProductAutomaton(automaton, automaton);
    for (nbautils::state_t q : product.states()) {
        const auto component_ids = ProdID2CompID(q, automaton.num_states());
        const bool accepting = automaton.get_accs(component_ids.first) != automaton.get_accs(component_ids.second);
        if (accepting)
            product.set_accs(q, std::vector<nbautils::acc_t> {0});
    }

    // Merge all SCCs in the product automaton so that there are no more cycles.
    std::map<std::set<nbautils::state_t>, nbautils::state_t> scc_representatives;
    nbautils::SWA<std::set<std::pair<TagT, TagT>>> merged_sccs = MergeSCCs(product, &scc_representatives);

    // From the SCC-Repr. map, also compute a Repr.-SCC map for later.
    std::map<nbautils::state_t, std::set<nbautils::state_t>> representative_sccs;
    for (const std::pair<const std::set<nbautils::state_t>, nbautils::state_t>& kv_pair : scc_representatives)
        representative_sccs.insert(std::make_pair(kv_pair.second, kv_pair.first));

    // Find the accepting SCCs of the original product automaton and merge them in the reduced automaton.
    std::vector<std::set<nbautils::state_t>> goal_sccs = FindGoalSCCs(product);
    std::set<nbautils::state_t> goal_sccs_merged_ids;
    for (const std::set<nbautils::state_t>& goal_scc : goal_sccs) {
        const nbautils::state_t merged_id = scc_representatives[goal_scc];
        goal_sccs_merged_ids.insert(merged_id);
    }
    nbautils::state_t final_scc = MergeStates(&merged_sccs, goal_sccs_merged_ids);

    // All state-pairs in SCCs from which "final_scc" is reachable are pairs of almost-equivalent states.
    // Convert the IDs of the merged SCCs back to pairs of original state IDs.
    const std::vector<nbautils::state_t> equivalent_pairs_sccs = CanReachState(merged_sccs, final_scc);
    std::vector<std::pair<nbautils::state_t, nbautils::state_t>> equivalent_pairs;
    for (nbautils::state_t scc_q : equivalent_pairs_sccs) {
        for (nbautils::state_t q : representative_sccs[scc_q]) {
            equivalent_pairs.push_back(ProdID2CompID(q, automaton.num_states()));
        }
    }

    // Now construct the equivalence relation object.
    EquivalenceRelation<nbautils::state_t> relation;
    for (nbautils::state_t equiv_pair : equivalent_pairs) {
        relation.AddElement(equiv_pair.first, equiv_pair.second);
    }
}