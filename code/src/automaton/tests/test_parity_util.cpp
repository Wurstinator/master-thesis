#include "catch.hpp"

#include "helper_functions.h"
#include "../parity_util.h"

using namespace tollk;
using namespace automaton;

NondeterministicAutomaton TestAutomaton1();

DeterministicAutomaton TestAutomaton2();

DeterministicAutomaton TestAutomaton3();

DeterministicAutomaton TestAutomaton4();

DPA TestAutomaton5();

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


TEST_CASE("Test LanguageEquivalentStates.") {
    DPA dpa(1);
    dpa.AddState(0);
    LanguageEquivalentStates(dpa);
    //TODO
}