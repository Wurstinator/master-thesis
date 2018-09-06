
#include <range/v3/algorithm/any_of.hpp>
#include "is_specialization_base_of.h"
#include <range/v3/algorithm/find.hpp>

namespace tollk {
namespace automaton {


template<typename RT1, typename RT2>
void RefineToCongruence(EquivalenceRelation <state_t>* relation, const TransitionAutomaton <RT1, RT2>& automaton) {
    std::set<EquivalenceRelation<state_t>::EquivClass> W(relation->Classes().begin() + 1, relation->Classes().end());
    while (!W.empty()) {
        EquivalenceRelation<state_t>::EquivClass A(std::move(*W.begin()));
        W.erase(W.begin());

        for (symbol_t s : automaton.Symbols()) {
            // Collect in "X" those states which move to "A" via symbol "s".
            std::set<state_t> X;
            for (state_t q : automaton.States()) {
                if (ranges::v3::any_of(automaton.Successors(q, s), [&A](state_t p) {
                    return A.find(p) != A.end();
                }))
                    X.insert(q);
            }

            const std::vector<EquivalenceRelation<state_t>::EquivClass> classes = relation->Classes();
            for (unsigned int i = 0; i < classes.size(); ++i) {
                const EquivalenceRelation<state_t>::EquivClass& Y = classes[i];
                EquivalenceRelation<state_t>::EquivClass XY_diff;
                std::set_difference(Y.begin(), Y.end(), X.begin(), X.end(), std::inserter(XY_diff, XY_diff.begin()));
                if (XY_diff.size() == Y.size() || XY_diff.empty())
                    continue;
                EquivalenceRelation<state_t>::EquivClass XY_intersect;
                std::set_intersection(Y.begin(), Y.end(), X.begin(), X.end(),
                                      std::inserter(XY_intersect, XY_intersect.begin()));

                relation->SplitClass(i, X);
                if (W.find(Y) != W.end()) {
                    W.erase(W.find(Y));
                    W.insert(XY_diff);
                    W.insert(XY_intersect);
                } else {
                    if (XY_diff.size() < XY_intersect.size())
                        W.insert(XY_diff);
                    else
                        W.insert(XY_intersect);
                }
            }
        }
    }
}


template<typename AutomatonT, typename LabelT>
void QuotientAutomaton(AutomatonT* automaton, const EquivalenceRelation <state_t>& relation,
                       const std::function<LabelT(
                               const EquivalenceRelation<state_t>::EquivClass&)>& merge_labels) {
    static_assert(std::is_base_of<LabelledAutomaton<LabelT>, AutomatonT>::value);
    static_assert(std::is_base_of<NondeterministicAutomaton, AutomatonT>::value);

    for (const EquivalenceRelation<state_t>::EquivClass& c : relation.Classes()) {
        automaton->MergeStates(c);
        automaton->SetLabel(*c.begin(), merge_labels(c));
    }
}


template<typename AutomatonT>
std::unordered_set<state_t> BuchiEmptyStates(const AutomatonT& automaton) {
    static_assert(std::is_base_of<ParityAutomaton, AutomatonT>::value);
    assert(automaton.IsBuchi());
    static_assert(is_specialization_base_of<TransitionAutomaton, AutomatonT>::value);

    // Find "goal" states, which are accepting states that can reach themselves.
    std::unordered_set<state_t> goal_states;
    for (state_t q : automaton.States()) {
        if (automaton.GetLabel(q) == 0 && CanReach(automaton, q, ranges::v3::view::single(q), false))
            goal_states.insert(q);
    }

    // Find all states that can reach no goal state.
    return NotReachingStates(automaton, goal_states);
}


}  // namespace automaton
}  // namespace tollk