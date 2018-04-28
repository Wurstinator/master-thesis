
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
void RefineToCongruence(EquivalenceRelation<state_t>* relation, const TransitionAutomaton<RT1, RT2>& automaton);

// Merges all states in each SCC into one state, resulting in a DAG. This is performed in-place on a nondeterministic
// automaton. For general transition automata, use NondeterministicAutomaton::FromTransitionAutomaton first.
// If the second argument is not NULL, then it is set to the computed SCCCollection, as this has to be done internally
// anyway. O(|A|^2) operation.
void MergeSCCs(NondeterministicAutomaton* automaton, SCCCollection* sccs = nullptr);


}  // namespace automaton
}  // namespace tollk


#include "util.impl.h"