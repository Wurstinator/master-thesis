
#include <range/v3/algorithm/any_of.hpp>

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
    if (visited->find(q) != visited->end())
        return;
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
    NondeterministicAutomaton result = NondeterministicAutomaton::FromTransitionAutomaton(automaton);
    result.MergeStates(merge_states);
    return result;
}



template<typename RT1, typename RT2>
void RefineToCongruence(EquivalenceRelation<state_t>* relation, const TransitionAutomaton<RT1, RT2>& automaton) {
    std::set<EquivalenceRelation<state_t>::EquivClass> W(relation->Classes().begin()+1, relation->Classes().end());
    while (!W.empty()) {
        EquivalenceRelation<state_t>::EquivClass A(std::move(*W.begin()));
        W.erase(W.begin());

        for (symbol_t s : automaton.Symbols()) {
            std::set<state_t> X;
            for (state_t q : automaton.States()) {
                if (ranges::v3::any_of(automaton.Successors(q, s), [&A](state_t q) {
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
                        W.insert(XY_intersect);
                }
            }
        }
    }
}


}  // namespace automaton
}  // namespace tollk