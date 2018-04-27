
#pragma once

#include <unordered_set>
#include <unordered_map>

#include "finite_automaton.h"
#include "transition_automaton.h"
#include "nondeterministic_automaton.h"
#include "../equivalence_relation.h"

namespace tollk {
namespace automaton {

// "Transpose" an automaton such that all transition edges are reversed. O(|V| * |E|) operation.
template<typename RT1, typename RT2>
NondeterministicAutomaton TransposeAutomaton(const TransitionAutomaton<RT1, RT2>& automaton);

// Strongly connected components.
struct SCCCollection {
    // Type that describes one SCC.
    using SCC = std::unordered_set<state_t>;
    // A list of all the SCCs in an automaton.
    std::vector<SCC> sccs;
    // For each state, the index in the "sccs" list in which that state occurs.
    std::unordered_map<state_t, std::vector<SCC>::size_type> scc_indices;
};

// Computes the SCCs of an automaton with Tarjan's algorithm. O(n) operation.
template<typename RT1, typename RT2>
SCCCollection StronglyConnectedComponents(const TransitionAutomaton<RT1, RT2>& automaton);

// Computes reachable states of a given one. O(n) operation.
template<typename RT1, typename RT2>
std::unordered_set<state_t> ReachableStates(const TransitionAutomaton<RT1, RT2>& automaton, state_t q);

// Merges a set of states into one. Outgoing transitions are unified and incoming transitions are redirected to the
// merged state. If the initial state is in the set, the merged state will be the initial state again.
// The ID of the new merged state will be *std::begin(range). O(|A|) operation.
template<typename RT1, typename RT2, typename SetT>
NondeterministicAutomaton MergeStates(const TransitionAutomaton<RT1, RT2>& automaton, const SetT& merge_states);

// Refines a given equivalence relation to a congruence relation on the automaton.
// TODO This algorithm can be optimized using a partition refinement structure, if necessary.
// Right now it's an implementation of https://en.wikipedia.org/wiki/DFA_minimization.
template<typename RT1, typename RT2>
void RefineToCongruence(EquivalenceRelation<state_t>* relation, const TransitionAutomaton<RT1, RT2>& automaton) {
    std::unordered_set<EquivalenceRelation<state_t>::EquivClass> W(relation->Classes().begin()+1, relation->Classes().end());
    while (!W.empty()) {
        EquivalenceRelation<state_t>::EquivClass A(std::move(*W.begin()));
        W.erase(W.begin());

        for (symbol_t s : automaton.Symbols()) {
            std::set<state_t> X;
            for (state_t q : automaton.States()) {
                if (std::any_of(automaton.Successors(q, s).begin(), automaton.Successors(q, s).end(), [&A](state_t q) {
                        return A.find(q) != A.end();
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
                std::set_intersection(Y.begin(), Y.end(), X.begin(), X.end(), std::inserter(XY_intersect, XY_intersect.begin()));

                relation->SplitClass(i, X);
                if (W.find(Y) != W.end()) {
                    W.erase(W.find(Y));
                    W.insert(XY_diff);
                    W.insert(XY_intersect);
                } else {
                    if (XY_diff.size() < XY_intersect.size())
                        W.insert(XY_diff);
                    else
                        W:insert(XY_intersect);
                }
            }
        }
    }
};



}  // namespace automaton
}  // namespace tollk


#include "util.impl.h"