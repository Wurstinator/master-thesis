
#pragma once

// General utility functions for transition automata.

#include "deterministic_automaton.h"
#include "nondeterministic_automaton.h"
#include <boost/bimap.hpp>
#include "../util/equivalence_relation.h"


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

// Checks whether a given SCC is trivial in an automaton.
template<typename RT1, typename RT2, typename SCCRangeT>
bool SCCIsTrivial(const TransitionAutomaton<RT1, RT2>& automaton, SCCRangeT&& scc);

// Checks whether from some state, one can reach a given collection of states. "allow_trivial" considers the fact that
// "from" is an element of "goal". If "allow_trivial" is true, CanReach returns true. Otherwise there has to be an
// explicit path from "from" to itself. O(n) operation.
template<typename RT1, typename RT2, typename Rng>
bool CanReach(const TransitionAutomaton<RT1, RT2>& automaton, state_t from, Rng&& goal, bool allow_trivial = true);

// Computes reachable states of a given one. O(n) operation.
template<typename RT1, typename RT2>
std::unordered_set<state_t> ReachableStates(const TransitionAutomaton<RT1, RT2>& automaton, state_t q);

// Computes states that can reach any state in a given collection.
template<typename RT1, typename RT2, typename Rng>
std::unordered_set<state_t> ReachingStates(const TransitionAutomaton<RT1, RT2>& automaton, Rng&& rng);

// Computes states that cannot reach any state in a given collection.
template<typename RT1, typename RT2, typename Rng>
std::unordered_set<state_t> NotReachingStates(const TransitionAutomaton<RT1, RT2>& automaton, Rng&& rng);

// Merges a set of states into one. Outgoing transitions are unified and incoming transitions are redirected to the
// merged state. If the initial state is in the set, the merged state will be the initial state again.
// The ID of the new merged state will be *std::begin(merge_states). O(|A|) operation.
template<typename RT1, typename RT2, typename SetT>
NondeterministicAutomaton MergeStates(const TransitionAutomaton<RT1, RT2>& automaton, const SetT& merge_states);

// Merges all states in each SCC into one state, resulting in a DAG. This is performed in-place on a nondeterministic
// automaton. For general transition automata, use NondeterministicAutomaton::FromTransitionAutomaton first.
// If the second argument is not NULL, then it is set to the computed SCCCollection, as this has to be done internally
// anyway. O(|A|^2) operation.
void MergeSCCs(NondeterministicAutomaton* automaton, SCCCollection* sccs = nullptr);

// Constructs a product automaton, i.e. an automaton that uses pairs as states and performs transitions pair-wise.
// If the "pair_indices" parameter points to a bimap, it is filled with the according index in the resulting automaton
// that corresponds to a given pair of states.
template<typename RT1, typename RT2, typename RT3, typename RT4>
NondeterministicAutomaton
ProductAutomaton(const TransitionAutomaton<RT1, RT2>& automaton1, const TransitionAutomaton<RT3, RT4>& automaton2,
                 boost::bimap<state_t, std::pair<state_t, state_t>>* pair_indices = nullptr);

// Similar to the other ProductAutomaton call but a specialized case for deterministic automata.
DeterministicAutomaton
DeterministicProductAutomaton(const DeterministicAutomaton& automaton1, const DeterministicAutomaton& automaton2,
                 boost::bimap<state_t, std::pair<state_t, state_t>>* pair_indices = nullptr);

// Merges all states that are equivalent in a given relation into one. "MergeStates" is used, so the new representative
// is the "first" element in each equivalence class.
void QuotientAutomaton(NondeterministicAutomaton* automaton, const EquivalenceRelation<state_t>& relation);


// Merges all states that are equivalent in a given relation into one. This assumes that the given relation is a
// congruence relation. The label of the new states are an arbitrary label of one of their merged states.
void QuotientAutomatonUnsafe(DeterministicAutomaton* automaton, const EquivalenceRelation<state_t>& relation);

// Given a directed graph, computes a topological sorting on the states.
// The algorithm assumes that the graph is acyclic. Behaviour for cyclic graphs is undefined.
// based on DFS algorithm from [Tarjan1976].
// The first node is one from which no other node is reachable.
template<typename RT1, typename RT2>
std::vector<state_t>
TopologicalSorting(const TransitionAutomaton<RT1, RT2>& automaton);

// Given a Nondeterministic automaton, remove all states that are not in a given set (and their transitions).
void Subautomaton(NondeterministicAutomaton* automaton, const std::unordered_set<state_t>& newStateSet);


}  // namespace automaton
}  // namespace tollk

#include "transition_util.impl.h"