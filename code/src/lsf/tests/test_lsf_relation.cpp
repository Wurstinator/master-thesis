
#include <catch.hpp>
#include "../lsf_relation.h"
#include "../../automaton/util.h"
#include "../../threshold_moore/moore_ltk.h"

using namespace tollk;
using namespace automaton;

DPA example1();

TEST_CASE("Test LSFBaseRelation.") {
    const DPA automaton = example1();
    Moore_LTK(automaton, 0);
    const EquivalenceRelation<state_t> lsf_relation = LSFBaseRelation(automaton, 0, LanguageEquivalentStates(automaton));
    CHECK(true);
}


TEST_CASE("Test LSFMergeClasses") {
    const DPA automaton = example1();
    const std::unordered_map<state_t, std::unordered_set<state_t>> lsf_relation = LSFMergeClasses(automaton, 0, LanguageEquivalentStates(automaton));
    CHECK(true);
}