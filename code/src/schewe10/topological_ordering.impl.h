

#include <unordered_set>


namespace tollk {


template <typename RT1, typename RT2>
void _TopologicalSorting_Visit(const tollk::automaton::TransitionAutomaton<RT1, RT2>& automaton, std::vector<tollk::automaton::state_t>* sorting, std::unordered_set<tollk::automaton::state_t>* to_visit, const tollk::automaton::state_t& node) {
    if (to_visit->find(node) == to_visit->end())
        return;
    for (const tollk::automaton::state_t& succ : automaton.Successors(node))
        if (succ != node)
            _TopologicalSorting_Visit(automaton, sorting, to_visit, succ);
    to_visit->erase(node);
    sorting->push_back(node);
}


template <typename RT1, typename RT2>
std::vector<tollk::automaton::state_t> TopologicalSorting(const tollk::automaton::TransitionAutomaton<RT1, RT2>& automaton) {
    std::vector<tollk::automaton::state_t> sorting;
    std::unordered_set<tollk::automaton::state_t> to_visit(automaton.States().begin(), automaton.States().end());
    while (!to_visit.empty())
        _TopologicalSorting_Visit(automaton, &sorting, &to_visit, *to_visit.begin());
    std::reverse(sorting.begin(), sorting.end());
    return sorting;
}



}