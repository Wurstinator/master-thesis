

#include "../automaton/is_specialization_base_of.h"
#include "../automaton/parity.h"
#include "../automaton/util.h"
#include <range/v3/core.hpp>
#include <range/v3/algorithm/find.hpp>


namespace tollk {


template <typename AutomatonT>
tollk::EquivalenceRelation<tollk::automaton::state_t> PriorityAlmostEquivalence(const AutomatonT& automaton) {
    using namespace ::tollk::automaton;

    static_assert(std::is_base_of<ParityAutomaton, AutomatonT>::value);
    static_assert(is_specialization_base_of<TransitionAutomaton, AutomatonT>::value);

    // Build the product automaton of the automaton with itself. It is a Büchi automaton / DFA in which the
    // accepting states are those in which the priority of the components differs.
    boost::bimap<state_t, std::pair<state_t, state_t>> product_pairs;
    NPA product(NondeterministicAutomaton::FromTransitionAutomaton(ProductAutomaton(automaton, automaton, &product_pairs)));
    for (state_t q : product.States()) {
        const std::pair<state_t, state_t> state_pair = product_pairs.left.at(q);
        const parity_label_t label1 = automaton.GetLabel(state_pair.first);
        const parity_label_t label2 = automaton.GetLabel(state_pair.second);
        product.SetLabel(q, label1 == label2 ? 1 : 0);
    }

    // Merge all SCCs in the product automaton.
    NPA product_merged_sccs(product);
    SCCCollection product_sccs;
    MergeSCCs(&product_merged_sccs, &product_sccs);

    // Build a list of SCCs in "product" that are goals, i.e. they are non-trivial and contain an accepting state.
    std::vector<state_t> product_goal_sccs_representatives;
    for (const SCCCollection::SCC& scc : product_sccs.sccs) {
        // Check if the SCC is trivial.
        if (scc.size() == 1) {
            //const typename NPA::SuccRangeState successors = product.Successors(*scc.begin());
            ranges::v3::any_view<state_t> successors = product.Successors(*scc.begin());
            if (ranges::v3::find(successors.begin(), successors.end(), *scc.begin()) == successors.end())
                continue;
        }

        // Check if the SCC contains a final state.
        bool contains_final = false;
        for (state_t q : scc)
            contains_final = contains_final || product.GetLabel(q) == 0;
        if (!contains_final)
            continue;

        // Add SCC to the list.
        product_goal_sccs_representatives.push_back(*scc.begin());
    }

    // Check which SCCs can / cannot reach the goal SCCs.
    std::unordered_set<state_t> cannot_reach_goal_scc_in_product(product_merged_sccs.States().begin(), product_merged_sccs.States().end());
    for (state_t q : ReachingStates(product_merged_sccs, product_goal_sccs_representatives))
        cannot_reach_goal_scc_in_product.erase(q);

    // Collect all pairs of states which are equivalent, which is those pairs from whose pair-state no goal SCC is
    // reachable.
    tollk::EquivalenceRelation<state_t> result;
    for (state_t scc_representative : cannot_reach_goal_scc_in_product) {
        const SCCCollection::SCC& scc = product_sccs.sccs[product_sccs.scc_indices[scc_representative]];
        for (state_t pair_state : scc) {
            const std::pair<state_t, state_t>& state_pair = product_pairs.left.at(pair_state);
            result.AddConnection(state_pair.first, state_pair.second);
        }
    }
    return result;
}


}