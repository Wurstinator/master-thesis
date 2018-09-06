

#include "../automaton/is_specialization_base_of.h"
#include "../automaton/parity.h"
#include "../automaton/util.h"


namespace tollk {


template <typename AutomatonT>
tollk::EquivalenceRelation<tollk::automaton::state_t> PriorityAlmostEquivalence(const AutomatonT& automaton) {
    using namespace ::tollk::automaton;

    static_assert(std::is_base_of<ParityAutomaton, AutomatonT>::value);
    static_assert(is_specialization_base_of<TransitionAutomaton, AutomatonT>::value);

    // Build the product automaton of the automaton with itself. It is a Büchi automaton / DFA in which the
    // accepting states are those in which the priority of the components differs.
    boost::bimap<state_t, std::pair<state_t, state_t>> product_pairs;
    NPA product(ProductAutomaton(automaton, automaton, &product_pairs));
    for (state_t q : product.States()) {
        const std::pair<state_t, state_t> state_pair = product_pairs.left.at(q);
        const parity_label_t label1 = automaton.GetLabel(state_pair.first);
        const parity_label_t label2 = automaton.GetLabel(state_pair.second);
        product.SetLabel(q, label1 == label2 ? 1 : 0);
    }

    // Find states from which only finitely many accepting states can be reached.
    const std::unordered_set<state_t> cannot_reach_goal_scc_in_product = BuchiEmptyStates(product);

    // Collect all pairs of states which are equivalent, which is those pairs from whose pair-state no goal SCC is
    // reachable.
    tollk::EquivalenceRelation<state_t> result;
    for (state_t pair_state : cannot_reach_goal_scc_in_product) {
        const std::pair<state_t, state_t>& state_pair = product_pairs.left.at(pair_state);
        result.AddConnection(state_pair.first, state_pair.second);
    }
    return result;
}



}