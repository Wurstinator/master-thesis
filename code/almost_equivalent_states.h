
#pragma once

#include "swa.hh"
#include "equivalence_relation.h"
#include "product_automaton.h"
#include "merge_sccs.h"
#include "find_goal_sccs.h"

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
    nbautils::SWA<std::set<std::pair<TagT, TagT>>> merged_sccs = MergeSCCs(product);

    // Find the accepting SCCs of the original product automaton and merge them in the reduced automaton.
    std::vector<std::set<nbautils::state_t>> goal_sccs = FindGoalSCCs(product);
    std::set<nbautils::state_t> goal_sccs_merged_ids;
    for (const std::set<nbautils::state_t>& goal_scc : goal_sccs) { //TODO does this work?
        const nbautils::state_t merged_id = FindMergeRepresentative(product, goal_scc.begin(), goal_scc.end());
    }
    MergeStates(&merged_sccs, std::set<std::set<nbautils::state_t>>(goal_sccs.begin(), goal_sccs.end()));
}