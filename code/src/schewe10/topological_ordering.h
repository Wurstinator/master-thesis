
#pragma once

#include <vector>
#include <unordered_set>

// based on DFS algorithm from [Tarjan1976].
// TODO write test

// Given a directed graph, computes a topological sorting. The vertices are given by two iterators (first_node
// and last_node). The edges are given by a functor get_successors which, given a node, returns an iterable of nodes.
// The algorithm assumes that the graph is acyclic. Behaviour for cyclic graphs is undefined.
template <typename NodeIter, typename SuccFunc>
std::vector<typename std::iterator_traits<NodeIter>::value_type> TopologicalSorting(NodeIter first_node, NodeIter last_node, const SuccFunc& get_successors) {
    std::vector<typename std::iterator_traits<NodeIter>::value_type> sorting;
    std::unordered_set<typename std::iterator_traits<NodeIter>::value_type> to_visit(first_node, last_node);
    while (!to_visit.empty()) {
        _TopologicalSorting_Visit(&sorting, &to_visit, *to_visit.begin(), get_successors);
    }
    std::reverse(sorting.begin(), sorting.end());
    return sorting;
};


template <typename Node, typename SuccFunc>
void _TopologicalSorting_Visit(std::vector<Node>* sorting, std::unordered_set<Node>* to_visit, const Node& node, const SuccFunc& get_successors) {
    if (to_visit->find(node) == to_visit->end())
        return;
    for (const Node& succ : get_successors(node))
        if (succ != node)
            _TopologicalSorting_Visit(sorting, to_visit, succ, get_successors);
    to_visit->erase(node);
    sorting->push_back(node);
};