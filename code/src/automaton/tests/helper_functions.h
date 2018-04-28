
#pragma once

#include <set>
#include "../finite_automaton.h"
#include <range/v3/to_container.hpp>

template <typename Rng1, typename Rng2>
bool CheckStateRangeEquivalence(Rng1 range1, Rng2 range2) {
    const std::vector<tollk::automaton::state_t> vec1 = ranges::v3::to_vector(range1);
    const std::set<tollk::automaton::state_t> set1(vec1.begin(), vec1.end());
    const std::vector<tollk::automaton::state_t> vec2 = ranges::v3::to_vector(range2);
    const std::set<tollk::automaton::state_t> set2(vec2.begin(), vec2.end());
    return set1 == set2;
};
