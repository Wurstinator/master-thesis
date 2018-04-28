#include "catch.hpp"

#include "helper_functions.h"
#include "../deterministic_automaton.h"
#include "../util.h"

using namespace tollk;
using namespace automaton;

NondeterministicAutomaton TestAutomaton1();
DeterministicAutomaton TestAutomaton2();
DeterministicAutomaton TestAutomaton3();

TEST_CASE("Test TransposeAutomaton.") {
    const NondeterministicAutomaton automaton = TransposeAutomaton(TestAutomaton1());
    CHECK(CheckStateRangeEquivalence(automaton.Successors(0, 0), std::set<state_t>{0}));
    CHECK(CheckStateRangeEquivalence(automaton.Successors(0, 1), std::set<state_t>{}));
    CHECK(CheckStateRangeEquivalence(automaton.Successors(1, 0), std::set<state_t>{0, 2}));
    CHECK(CheckStateRangeEquivalence(automaton.Successors(1, 1), std::set<state_t>{}));
    CHECK(CheckStateRangeEquivalence(automaton.Successors(2, 0), std::set<state_t>{1}));
    CHECK(CheckStateRangeEquivalence(automaton.Successors(2, 1), std::set<state_t>{1}));
    CHECK(CheckStateRangeEquivalence(automaton.Successors(3, 0), std::set<state_t>{3}));
    CHECK(CheckStateRangeEquivalence(automaton.Successors(3, 1), std::set<state_t>{2}));
}

TEST_CASE("Test ReachableStates.") {
    const std::unordered_set<state_t> reachable0 = ReachableStates(TestAutomaton1(), 0);
    const std::unordered_set<state_t> reachable1 = ReachableStates(TestAutomaton1(), 1);
    const std::unordered_set<state_t> reachable2 = ReachableStates(TestAutomaton1(), 2);
    const std::unordered_set<state_t> reachable3 = ReachableStates(TestAutomaton1(), 3);
    CHECK(CheckStateRangeEquivalence(reachable0, std::set<state_t>{0,1,2,3}));
    CHECK(CheckStateRangeEquivalence(reachable1, std::set<state_t>{1,2,3}));
    CHECK(CheckStateRangeEquivalence(reachable2, std::set<state_t>{1,2,3}));
    CHECK(CheckStateRangeEquivalence(reachable3, std::set<state_t>{3}));

    NondeterministicAutomaton test_automaton2(0);
    test_automaton2.AddState(0);
    test_automaton2.AddState(1);
    CHECK(CheckStateRangeEquivalence(ReachableStates(test_automaton2, 0), std::set<state_t>{0}));
    CHECK(CheckStateRangeEquivalence(ReachableStates(test_automaton2, 1), std::set<state_t>{1}));
}

TEST_CASE("Test StronglyConnectedComponents.") {
    SCCCollection sccs = StronglyConnectedComponents(TestAutomaton1());
    CHECK(sccs.sccs.size() == 3);
    CHECK(sccs.scc_indices.size() == 4);
    CHECK(sccs.scc_indices[0] != sccs.scc_indices[1]);
    CHECK(sccs.scc_indices[0] != sccs.scc_indices[2]);
    CHECK(sccs.scc_indices[0] != sccs.scc_indices[3]);
    CHECK(sccs.scc_indices[1] == sccs.scc_indices[2]);
    CHECK(sccs.scc_indices[1] != sccs.scc_indices[3]);
    CHECK(sccs.scc_indices[2] != sccs.scc_indices[3]);
    REQUIRE(sccs.scc_indices.size() == 4);
    const auto& scc1 = sccs.sccs[sccs.scc_indices[0]];
    const auto& scc2 = sccs.sccs[sccs.scc_indices[1]];
    const auto& scc3 = sccs.sccs[sccs.scc_indices[3]];
    CHECK(scc1.size() == 1);
    CHECK(scc1.find(0) != scc1.end());
    CHECK(scc2.size() == 2);
    CHECK(scc2.find(1) != scc1.end());
    CHECK(scc2.find(2) != scc1.end());
    CHECK(scc3.size() == 1);
    CHECK(scc3.find(3) != scc1.end());
}

TEST_CASE("Test MergeStates.") {
    const NondeterministicAutomaton automaton = MergeStates(TestAutomaton1(), std::set<state_t> {2,3});
    CHECK(automaton.States().size() == 3);
    CHECK(automaton.HasState(0));
    CHECK(automaton.HasState(1));
    CHECK(automaton.HasState(2));
    CHECK(!automaton.HasState(3));
    CHECK(CheckStateRangeEquivalence(automaton.Successors(0, 0), std::set<state_t>{0, 1}));
    CHECK(CheckStateRangeEquivalence(automaton.Successors(0, 1), std::set<state_t>{}));
    CHECK(CheckStateRangeEquivalence(automaton.Successors(1, 0), std::set<state_t>{2}));
    CHECK(CheckStateRangeEquivalence(automaton.Successors(1, 1), std::set<state_t>{2}));
    CHECK(CheckStateRangeEquivalence(automaton.Successors(2, 0), std::set<state_t>{1, 2}));
    CHECK(CheckStateRangeEquivalence(automaton.Successors(2, 1), std::set<state_t>{2}));
}

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


TEST_CASE("Test MergeSCCs.") {
    NondeterministicAutomaton automaton = NondeterministicAutomaton::FromTransitionAutomaton(TestAutomaton3());

    const SCCCollection sccs = StronglyConnectedComponents(automaton);

    MergeSCCs(&automaton);
    REQUIRE(automaton.States().size() == 2);
    const state_t rep1 = *(sccs.sccs.at(sccs.scc_indices.at(0)).begin());
    const state_t rep2 = *(sccs.sccs.at(sccs.scc_indices.at(2)).begin());
    CHECK(CheckStateRangeEquivalence(automaton.States(), std::set<state_t>{rep1, rep2}));
    CHECK(automaton.InitialState() == rep1);
    CHECK(CheckStateRangeEquivalence(automaton.Successors(rep1, 0), std::set<state_t> {rep1}));
    CHECK(CheckStateRangeEquivalence(automaton.Successors(rep1, 1), std::set<state_t> {rep2}));
    CHECK(CheckStateRangeEquivalence(automaton.Successors(rep2, 0), std::set<state_t> {rep2}));
    CHECK(CheckStateRangeEquivalence(automaton.Successors(rep2, 1), std::set<state_t> {rep2}));

    SCCCollection sccs2;
    NondeterministicAutomaton automaton2 = NondeterministicAutomaton::FromTransitionAutomaton(TestAutomaton3());
    MergeSCCs(&automaton2, &sccs2);
    CHECK(automaton.States() == automaton2.States());
    CHECK(automaton.InitialState() == automaton2.InitialState());
    CHECK(CheckStateRangeEquivalence(automaton.Successors(rep1, 0), automaton2.Successors(rep1, 0)));
    CHECK(CheckStateRangeEquivalence(automaton.Successors(rep1, 1), automaton2.Successors(rep1, 1)));
    CHECK(CheckStateRangeEquivalence(automaton.Successors(rep2, 0), automaton2.Successors(rep2, 0)));
    CHECK(CheckStateRangeEquivalence(automaton.Successors(rep2, 1), automaton2.Successors(rep2, 1)));
    CHECK(sccs.sccs == sccs2.sccs);
    CHECK(sccs.scc_indices == sccs2.scc_indices);
}
