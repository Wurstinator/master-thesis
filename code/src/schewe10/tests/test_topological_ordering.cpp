
#include "catch.hpp"
#include "swa.hh"
#include "../../automaton/util.h"
#include "../../automaton/nondeterministic_automaton.h"


TEST_CASE("Test TopologicalSorting.") {
    tollk::automaton::NondeterministicAutomaton automaton(0);
    automaton.AddState(0);
    automaton.AddState(1);
    automaton.AddState(2);
    automaton.AddState(3);
    automaton.AddState(4);
    automaton.AddSucc(0, 0, 1);
    automaton.AddSucc(0, 0, 2);
    automaton.AddSucc(2, 0, 3);
    automaton.AddSucc(4, 0, 3);

    const std::vector<tollk::automaton::state_t> sorting = tollk::automaton::TopologicalSorting(automaton);
    REQUIRE(sorting.size() == 5);

    const unsigned int index0 = std::distance(sorting.begin(), std::find(sorting.begin(), sorting.end(), 0));
    const unsigned int index1 = std::distance(sorting.begin(), std::find(sorting.begin(), sorting.end(), 1));
    const unsigned int index2 = std::distance(sorting.begin(), std::find(sorting.begin(), sorting.end(), 2));
    const unsigned int index3 = std::distance(sorting.begin(), std::find(sorting.begin(), sorting.end(), 3));
    const unsigned int index4 = std::distance(sorting.begin(), std::find(sorting.begin(), sorting.end(), 4));

    CHECK(index0 < index1);
    CHECK(index0 < index2);
    CHECK(index0 < index3);
    CHECK(index2 < index3);
    CHECK(index4 < index3);
}
