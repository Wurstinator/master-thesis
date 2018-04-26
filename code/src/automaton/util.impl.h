

namespace tollk {
namespace automaton {


template<typename RT1, typename RT2>
NondeterministicAutomaton TransposeAutomaton(const TransitionAutomaton<RT1, RT2>& automaton) {
    NondeterministicAutomaton transposed(automaton.atomicPropositions);
    for (state_t q : automaton.States())
        transposed.AddState(q);

    for (state_t p : automaton.States()) {
        for (symbol_t s : automaton.Symbols()) {
            for (state_t q : automaton.Successors(p, s)) {
                transposed.AddSucc(q, s, p);
            }
        }
    }
    return transposed;
};



// Recursive helper function for the SCC algorithm. Takes as input the automaton to operate on, current DFS node,
// current call depth, a map containing for each state the index at which it was visited, the smallest vertex that
// can be visited and whether it is present in the stack, the visitation order stack, and the resulting structure.
struct _SCCTarjan_Struct {
    unsigned int visited_at;
    unsigned int min_reach;
    bool in_stack;
};

template<typename RT1, typename RT2>
void _SCCTarjan(const TransitionAutomaton<RT1, RT2>& automaton,
                state_t node,
                unsigned int* depth,
                std::unordered_map<state_t, _SCCTarjan_Struct>* visit_indices,
                std::stack<state_t>* stack,
                SCCCollection* sccs) {
    // Add the node to the "visited" set.
    (*visit_indices)[node] = _SCCTarjan_Struct {*depth, *depth, true};
    stack->push(node);
    (*depth) += 1;

    // DFS
    for (state_t neighbour : automaton.Successors(node)) {
        if (visit_indices->find(neighbour) == visit_indices->end()) {
            // The node still has to be visited.
            _SCCTarjan(automaton, neighbour, depth, visit_indices, stack, sccs);
            if ((*visit_indices)[node].min_reach > (*visit_indices)[neighbour].min_reach)
                (*visit_indices)[node].min_reach = (*visit_indices)[neighbour].min_reach;
        } else if ((*visit_indices)[neighbour].in_stack) {
            // The node has been visited but has not been assigned to an SCC yet.
            if ((*visit_indices)[node].min_reach > (*visit_indices)[neighbour].visited_at)
                (*visit_indices)[node].min_reach = (*visit_indices)[neighbour].visited_at;
        }
    }

    // Collect the SCCs.
    if ((*visit_indices)[node].visited_at == (*visit_indices)[node].min_reach) {
        sccs->sccs.emplace_back();
        state_t popped_node;
        do {
            popped_node = stack->top();
            stack->pop();
            sccs->sccs.back().insert(popped_node);
            sccs->scc_indices[popped_node] = sccs->sccs.size() - 1;
        } while (popped_node != node);
    }
};


template<typename RT1, typename RT2>
SCCCollection StronglyConnectedComponents(const TransitionAutomaton<RT1, RT2>& automaton) {
    std::unordered_map<state_t, _SCCTarjan_Struct> visit_indices;
    std::stack<state_t> stack;
    SCCCollection result;
    unsigned int depth = 0;
    _SCCTarjan(automaton, automaton.InitialState(), &depth, &visit_indices, &stack, &result);
    return result;
}


// Performs a simple DFS and collects the visited states.
template<typename RT1, typename RT2>
void _ReachableStates_DSF(const TransitionAutomaton<RT1, RT2>& automaton, state_t q, std::unordered_set<state_t>* visited) {
    visited->insert(q);
    for (state_t p : automaton.Successors(q))
        _ReachableStates_DSF(automaton, p, visited);
};


template<typename RT1, typename RT2>
std::unordered_set<state_t> ReachableStates(const TransitionAutomaton<RT1, RT2>& automaton, state_t q) {
    std::unordered_set<state_t> result;
    _ReachableStates_DSF(automaton, q, &result);
    return result;
}


template<typename RT1, typename RT2, typename SetT>
NondeterministicAutomaton MergeStates(const TransitionAutomaton<RT1, RT2>& automaton, const SetT& merge_states) {
    NondeterministicAutomaton result(automaton.atomicPropositions);
    const state_t representative = *merge_states.begin();

    // Copy the states to the new non-deterministic automaton.
    for (state_t p : automaton.States())
        if (merge_states.find(p) != merge_states.end())
            result.AddState(p);
    result.AddState(representative);

    // Copy the transitions.
    for (state_t p : automaton.States()) {
        for (symbol_t s : automaton.Symbols()) {
            for (state_t q : automaton.Successors(p, s)) {
                const state_t merge_p = (merge_states.find(p) != merge_states.end() ? representative : p);
                const state_t merge_q = (merge_states.find(q) != merge_states.end() ? representative : q);
                result.AddSucc(merge_p, s, merge_q);
            }
        }
    }

    // Set the initial state.
    if (merge_states.find(automaton.InitialState()) != merge_states.end()) {
        result.SetInitialState(representative);
    } else {
        result.SetInitialState(automaton.InitialState());
    }

    return result;
}


}  // namespace automaton
}  // namespace tollk