#include "catch.hpp"

#include "helper_functions.h"
#include "../hoa/hoa_io.h"
#include "../parity_util.h"

using namespace tollk;
using namespace automaton;

DeterministicAutomaton TestAutomaton2();
DPA TestAutomaton5();
DPA TestAutomaton6();

TEST_CASE("Test RefineToCongruence.") {
    EquivalenceRelation<state_t> equivalence_relation;
    equivalence_relation.AddConnection(0, 1);
    equivalence_relation.AddConnection(0, 2);
    equivalence_relation.AddConnection(3, 3);
    RefineToCongruence(&equivalence_relation, TestAutomaton2());
    CHECK(equivalence_relation.Classes().size() == 3);
    CHECK(equivalence_relation.IsEquiv(0, 0));
    CHECK(!equivalence_relation.IsEquiv(0, 1));
    CHECK(equivalence_relation.IsEquiv(0, 2));
    CHECK(!equivalence_relation.IsEquiv(0, 3));
    CHECK(!equivalence_relation.IsEquiv(1, 0));
    CHECK(equivalence_relation.IsEquiv(1, 1));
    CHECK(!equivalence_relation.IsEquiv(1, 2));
    CHECK(!equivalence_relation.IsEquiv(1, 3));
    CHECK(equivalence_relation.IsEquiv(2, 0));
    CHECK(!equivalence_relation.IsEquiv(2, 1));
    CHECK(equivalence_relation.IsEquiv(2, 2));
    CHECK(!equivalence_relation.IsEquiv(2, 3));
    CHECK(!equivalence_relation.IsEquiv(3, 0));
    CHECK(!equivalence_relation.IsEquiv(3, 1));
    CHECK(!equivalence_relation.IsEquiv(3, 2));
    CHECK(equivalence_relation.IsEquiv(3, 3));
}


TEST_CASE("Test Hopcroft.") {
    DPA dpa(1);
    dpa.AddState(0);
    dpa.AddState(1);
    dpa.SetInitialState(0);
    dpa.SetSucc(0, 0, 1);
    dpa.SetSucc(0, 1, 1);
    dpa.SetSucc(1, 0, 1);
    dpa.SetSucc(1, 1, 1);
    Hopcroft(&dpa);
    CHECK(dpa.States().size() == 1);
    CHECK(dpa.HasState(0));
    CHECK(dpa.Succ(0, 0) == 0);
    CHECK(dpa.Succ(0, 1) == 0);
}


TEST_CASE("Test BuchiEmptyStates.") {
    const DPA automaton = TestAutomaton5();
    const std::unordered_set<state_t> empty_states = BuchiEmptyStates(automaton);
    CHECK(empty_states.size() == 3);
    CHECK(empty_states.find(0) == empty_states.end());
    CHECK(empty_states.find(1) != empty_states.end());
    CHECK(empty_states.find(2) == empty_states.end());
    CHECK(empty_states.find(3) == empty_states.end());
    CHECK(empty_states.find(4) != empty_states.end());
    CHECK(empty_states.find(5) != empty_states.end());
}

TEST_CASE("Test NormalizePriorities.") {
    DPA dpa(1);
    dpa.AddState(0);
    dpa.AddState(1);
    dpa.AddState(2);
    dpa.AddState(3);
    dpa.AddState(4);
    dpa.AddState(5);
    dpa.AddState(6);
    dpa.AddState(7);
    dpa.SetSucc(0, 0, 1);
    dpa.SetSucc(0, 1, 1);
    dpa.SetSucc(1, 0, 0);
    dpa.SetSucc(1, 1, 2);
    dpa.SetSucc(2, 0, 1);
    dpa.SetSucc(2, 1, 1);
    dpa.SetSucc(3, 0, 3);
    dpa.SetSucc(3, 1, 1);
    dpa.SetSucc(4, 0, 4);
    dpa.SetSucc(4, 1, 5);
    dpa.SetSucc(5, 0, 4);
    dpa.SetSucc(5, 1, 5);
    dpa.SetSucc(6, 0, 7);
    dpa.SetSucc(6, 1, 7);
    dpa.SetSucc(7, 0, 6);
    dpa.SetSucc(7, 1, 6);
    dpa.SetLabel(0, 0);
    dpa.SetLabel(1, 1);
    dpa.SetLabel(2, 2);
    dpa.SetLabel(3, 1);
    dpa.SetLabel(4, 2);
    dpa.SetLabel(5, 0);
    dpa.SetLabel(6, 3);
    dpa.SetLabel(7, 3);

    const std::map<state_t, parity_label_t> new_priorities = NormalizePriorities(dpa);
    CHECK(new_priorities.at(0) == 0);
    CHECK(new_priorities.at(1) == 1);
    CHECK(new_priorities.at(2) == 2);
    CHECK(new_priorities.at(3) == 1);
    CHECK(new_priorities.at(4) == 0);
    CHECK(new_priorities.at(5) == 0);
    CHECK(new_priorities.at(6) == 1);
    CHECK(new_priorities.at(7) == 1);
}

/*
TEST_CASE("Test LanguageEquivalentStates. 1") {
    DPA dpa(1);
    dpa.AddState(0);
    dpa.AddState(1);
    dpa.SetInitialState(0);
    dpa.SetSucc(0, 0, 0);
    dpa.SetSucc(0, 1, 1);
    dpa.SetSucc(1, 0, 0);
    dpa.SetSucc(1, 1, 1);
    dpa.SetLabel(0, 0);
    dpa.SetLabel(0, 1);

    const EquivalenceRelation<state_t> relation = LanguageEquivalentStates(dpa);
    CHECK(relation.Classes().size() == 1);
    CHECK(relation.Exists(0));
    CHECK(relation.Exists(1));
    CHECK(relation.IsEquiv(0, 1));
    CHECK(relation.GetClass(0).size() == 2);
}


TEST_CASE("Test LanguageEquivalentStates. 2") {
    DPA dpa(1);
    dpa.AddState(0);
    dpa.AddState(1);
    dpa.AddState(2);
    dpa.AddState(3);
    dpa.SetInitialState(0);
    dpa.SetSucc(0, 0, 2);
    dpa.SetSucc(0, 1, 1);
    dpa.SetSucc(1, 0, 3);
    dpa.SetSucc(1, 1, 1);
    dpa.SetSucc(2, 0, 2);
    dpa.SetSucc(2, 1, 2);
    dpa.SetSucc(3, 0, 3);
    dpa.SetSucc(3, 1, 0);
    dpa.SetLabel(0, 0);
    dpa.SetLabel(1, 0);
    dpa.SetLabel(2, 1);
    dpa.SetLabel(3, 1);

    const EquivalenceRelation<state_t> relation = LanguageEquivalentStates(dpa);
    CHECK(relation.Classes().size() == 4);
    CHECK(relation.Exists(0));
    CHECK(relation.Exists(1));
    CHECK(relation.Exists(2));
    CHECK(relation.Exists(3));
    CHECK(!relation.IsEquiv(0, 1));
    CHECK(!relation.IsEquiv(0, 2));
    CHECK(!relation.IsEquiv(0, 3));
    CHECK(!relation.IsEquiv(1, 2));
    CHECK(!relation.IsEquiv(1, 3));
    CHECK(!relation.IsEquiv(2, 3));
}
*/


TEST_CASE("Test LanguageEquivalentStates. 3") {
    const DPA dpa = TestAutomaton6();
    const EquivalenceRelation<state_t> relation = LanguageEquivalentStates(dpa);
    const EquivalenceRelation<state_t>::EquivClass c1 {10, 29, 52, 66, 87}; // 10 29 52 / 66 87
    const EquivalenceRelation<state_t>::EquivClass c2 {16, 31, 67, 73, 82};
    const EquivalenceRelation<state_t>::EquivClass c3 {25, 36};
    const EquivalenceRelation<state_t>::EquivClass c4 {26, 45};
    const EquivalenceRelation<state_t>::EquivClass c5 {27, 35, 50};
    const EquivalenceRelation<state_t>::EquivClass c6 {30, 63};
    const EquivalenceRelation<state_t>::EquivClass c7 {33, 38};
    const EquivalenceRelation<state_t>::EquivClass c8 {34, 51};
    const EquivalenceRelation<state_t>::EquivClass c9 {39, 46};
    const EquivalenceRelation<state_t>::EquivClass c10 {40, 58};
    const EquivalenceRelation<state_t>::EquivClass c11 {41, 47, 54, 75};
    const EquivalenceRelation<state_t>::EquivClass c12 {42, 57, 68, 74, 86};
    const EquivalenceRelation<state_t>::EquivClass c13 {43, 61};
    const EquivalenceRelation<state_t>::EquivClass c14 {48, 56, 64, 81};
    const EquivalenceRelation<state_t>::EquivClass c15 {49, 53, 60, 69, 70, 76, 77, 78, 79, 83, 85, 89, 90, 91, 92, 93, 94};
    const EquivalenceRelation<state_t>::EquivClass c16 {59, 65, 71, 72, 80, 84, 88, 95};
    //CHECK(std::find(relation.Classes().begin(), relation.Classes().end(), c1) != relation.Classes().end());
    //CHECK(std::find(relation.Classes().begin(), relation.Classes().end(), c2) != relation.Classes().end());
    CHECK(std::find(relation.Classes().begin(), relation.Classes().end(), c3) != relation.Classes().end());
    CHECK(std::find(relation.Classes().begin(), relation.Classes().end(), c4) != relation.Classes().end());
    CHECK(std::find(relation.Classes().begin(), relation.Classes().end(), c5) != relation.Classes().end());
    CHECK(std::find(relation.Classes().begin(), relation.Classes().end(), c6) != relation.Classes().end());
    CHECK(std::find(relation.Classes().begin(), relation.Classes().end(), c7) != relation.Classes().end());
    CHECK(std::find(relation.Classes().begin(), relation.Classes().end(), c8) != relation.Classes().end());
    CHECK(std::find(relation.Classes().begin(), relation.Classes().end(), c9) != relation.Classes().end());
    CHECK(std::find(relation.Classes().begin(), relation.Classes().end(), c10) != relation.Classes().end());
    CHECK(std::find(relation.Classes().begin(), relation.Classes().end(), c11) != relation.Classes().end());
    CHECK(std::find(relation.Classes().begin(), relation.Classes().end(), c12) != relation.Classes().end());
    CHECK(std::find(relation.Classes().begin(), relation.Classes().end(), c13) != relation.Classes().end());
    CHECK(std::find(relation.Classes().begin(), relation.Classes().end(), c14) != relation.Classes().end());
    CHECK(std::find(relation.Classes().begin(), relation.Classes().end(), c15) != relation.Classes().end());
    CHECK(std::find(relation.Classes().begin(), relation.Classes().end(), c16) != relation.Classes().end());
}