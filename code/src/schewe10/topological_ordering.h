
#pragma once

#include <vector>
#include "../automaton/transition_automaton.h"

// based on DFS algorithm from [Tarjan1976].

namespace tollk {

// Given a directed graph, computes a topological sorting.
// The algorithm assumes that the graph is acyclic. Behaviour for cyclic graphs is undefined.
template<typename RT1, typename RT2>
std::vector<tollk::automaton::state_t>
TopologicalSorting(const tollk::automaton::TransitionAutomaton<RT1, RT2>& automaton);

}

#include "topological_ordering.impl.h"