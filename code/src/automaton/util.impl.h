
#include <range/v3/algorithm/any_of.hpp>
#include "is_specialization_base_of.h"
#include <range/v3/algorithm/find.hpp>

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
                std::map<state_t, _SCCTarjan_Struct>* visit_indices,
                std::stack<state_t>* stack,
                SCCCollection* sccs) {
    if (sccs->scc_indices.find(node) != sccs->scc_indices.end())
        return;

    // Add the node to the "visited" set.
    (*visit_indices)[node] = _SCCTarjan_Struct {*depth, *depth, true};
    stack->push(node);
    (*depth) += 1;

    // DFS
    for (state_t neighbour : automaton.Successors(node)) {
        if (visit_indices->find(neighbour) == visit_indices->end()) {
            // The node still has to be visited.
            _SCCTarjan(automaton, neighbour, depth, visit_indices, stack, sccs);
            if ((visit_indices->find(neighbour) != visit_indices->end()) && (*visit_indices)[node].min_reach > (*visit_indices)[neighbour].min_reach)
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
            (*visit_indices)[popped_node].in_stack = false;
            sccs->sccs.back().insert(popped_node);
            sccs->scc_indices[popped_node] = sccs->sccs.size() - 1;
        } while (popped_node != node);
    }
}


template<typename RT1, typename RT2>
SCCCollection StronglyConnectedComponents(const TransitionAutomaton<RT1, RT2>& automaton) {
    SCCCollection result;
    for (state_t q : automaton.States()) {
        std::map<state_t, _SCCTarjan_Struct> visit_indices;
        std::stack<state_t> stack;
        unsigned int depth = 0;
        _SCCTarjan(automaton, q, &depth, &visit_indices, &stack, &result);
    }
    return result;
}

// Performs a simple DFS and collects the visited states. Returns true if a goal state was found.
template<typename RT1, typename RT2, typename Rng>
bool _CanReach_DSF(const TransitionAutomaton<RT1, RT2>& automaton, state_t q, Rng&& goal, std::unordered_set<state_t>* visited) {
    if (ranges::v3::find(goal, q) != goal.end())
        return true;
    if (visited->find(q) != visited->end())
        return false;
    visited->insert(q);
    for (state_t p : automaton.Successors(q))
        if (_CanReach_DSF(automaton, p, goal, visited))
            return true;
    return false;
};


template<typename RT1, typename RT2, typename Rng>
bool CanReach(const TransitionAutomaton<RT1, RT2>& automaton, state_t from, Rng&& goal, bool allow_trivial) {
    std::unordered_set<state_t> visited;
    if (allow_trivial) {
        return _CanReach_DSF(automaton, from, goal, &visited);
    } else {
        for (state_t neighbour : automaton.Successors(from)) {
            if (_CanReach_DSF(automaton, neighbour, goal, &visited))
                return true;
        }
        return false;
    }
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

template <typename RT1, typename RT2, typename Rng>
std::unordered_set<state_t> ReachingStates(const TransitionAutomaton<RT1, RT2>& automaton, Rng&& rng) {
    const NondeterministicAutomaton transposed_automaton = TransposeAutomaton(automaton);
    std::unordered_set<state_t> result;
    for (state_t q : rng) {
        for (state_t reach : ReachableStates(transposed_automaton, q)) {
            result.insert(reach);
        }
    }
    return result;
}

template <typename RT1, typename RT2, typename Rng>
std::unordered_set<state_t> NotReachingStates(const TransitionAutomaton<RT1, RT2>& automaton, Rng&& rng) {
    const NondeterministicAutomaton transposed_automaton = TransposeAutomaton(automaton);
    std::unordered_set<state_t> result(automaton.States().begin(), automaton.States().end());
    for (state_t q : rng) {
        for (state_t reach : ReachableStates(transposed_automaton, q)) {
            result.erase(reach);
        }
    }
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


template <typename RT1, typename RT2, typename RT3, typename RT4>
NondeterministicAutomaton ProductAutomaton(const TransitionAutomaton<RT1, RT2>& automaton1, const TransitionAutomaton<RT3, RT4>& automaton2, boost::bimap<state_t, std::pair<state_t, state_t>>* pair_indices) {
    assert(automaton1.atomicPropositions == automaton2.atomicPropositions);

    // Fill the map of state pairs.
    boost::bimap<state_t, std::pair<state_t, state_t>> pair_indices_;
    for (state_t p : automaton1.States())
        for (state_t q : automaton2.States())
            pair_indices_.left.insert(std::make_pair(pair_indices_.size(), std::make_pair(p, q)));

    // Create the product automaton.
    NondeterministicAutomaton product(automaton1.atomicPropositions);
    for (const auto& kv_pair : pair_indices_.left)
        product.AddState(kv_pair.first);
    product.SetInitialState(pair_indices_.right.at(std::make_pair(automaton1.InitialState(), automaton2.InitialState())));

    // Set the transitions.
    for (state_t p : automaton1.States()) {
        for (state_t q : automaton2.States()) {
            for (symbol_t s : product.Symbols()) {
                for (state_t succ1 : automaton1.Successors(p, s)) {
                    for (state_t succ2 : automaton2.Successors(q, s)) {
                        product.AddSucc(pair_indices_.right.at(std::make_pair(p, q)), s, pair_indices_.right.at(std::make_pair(succ1, succ2)));
                    }
                }
            }
        }
    }

    if (pair_indices != nullptr)
        *pair_indices = std::move(pair_indices_);
    return product;
}


template <typename AutomatonT>
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



template <typename AutomatonT>
void QuotientAutomaton(AutomatonT* automaton, const EquivalenceRelation<state_t>& relation, const std::function<parity_label_t(const EquivalenceRelation<state_t>::EquivClass&)>& merge_labels) {
    static_assert(is_specialization_base_of<LabelledAutomaton, AutomatonT>::value);
    static_assert(std::is_base_of<NondeterministicAutomaton, AutomatonT>::value);

    for (const EquivalenceRelation<state_t>::EquivClass& c : relation.Classes()) {
        automaton->MergeStates(c);
        automaton->SetLabel(*c.begin(), merge_labels(c));
    }
}




}  // namespace automaton
}  // namespace tollk