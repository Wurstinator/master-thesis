#include "catch.hpp"

#include "helper_functions.h"
#include "../deterministic_automaton.h"
#include "../util.h"

using namespace tollk;
using namespace automaton;

NondeterministicAutomaton TestAutomaton1();

DeterministicAutomaton TestAutomaton2();

DeterministicAutomaton TestAutomaton3();

DeterministicAutomaton TestAutomaton4();

DPA TestAutomaton5();

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
    CHECK(CheckStateRangeEquivalence(reachable0, std::set<state_t>{0, 1, 2, 3}));
    CHECK(CheckStateRangeEquivalence(reachable1, std::set<state_t>{1, 2, 3}));
    CHECK(CheckStateRangeEquivalence(reachable2, std::set<state_t>{1, 2, 3}));
    CHECK(CheckStateRangeEquivalence(reachable3, std::set<state_t>{3}));

    NondeterministicAutomaton test_automaton2(0);
    test_automaton2.AddState(0);
    test_automaton2.AddState(1);
    CHECK(CheckStateRangeEquivalence(ReachableStates(test_automaton2, 0), std::set<state_t>{0}));
    CHECK(CheckStateRangeEquivalence(ReachableStates(test_automaton2, 1), std::set<state_t>{1}));
}

TEST_CASE("Test ReachingStates.") {
    const std::unordered_set<state_t> reach0 = ReachingStates(TestAutomaton1(), std::set<state_t>{0});
    const std::unordered_set<state_t> reach1 = ReachingStates(TestAutomaton1(), std::set<state_t>{1});
    const std::unordered_set<state_t> reach2 = ReachingStates(TestAutomaton1(), std::set<state_t>{2});
    const std::unordered_set<state_t> reach3 = ReachingStates(TestAutomaton1(), std::set<state_t>{3});
    CHECK(CheckStateRangeEquivalence(reach0, std::set<state_t>{0}));
    CHECK(CheckStateRangeEquivalence(reach1, std::set<state_t>{0, 1, 2}));
    CHECK(CheckStateRangeEquivalence(reach2, std::set<state_t>{0, 1, 2}));
    CHECK(CheckStateRangeEquivalence(reach3, std::set<state_t>{0, 1, 2, 3}));

    NondeterministicAutomaton test_automaton2(0);
    test_automaton2.AddState(0);
    test_automaton2.AddState(1);
    CHECK(CheckStateRangeEquivalence(ReachingStates(test_automaton2, std::set<state_t>{0, 1}), std::set<state_t>{0, 1}));
}

TEST_CASE("Test NotReachingStates.") {
    const std::unordered_set<state_t> reach0 = NotReachingStates(TestAutomaton1(), std::set<state_t>{0});
    const std::unordered_set<state_t> reach1 = NotReachingStates(TestAutomaton1(), std::set<state_t>{1});
    const std::unordered_set<state_t> reach2 = NotReachingStates(TestAutomaton1(), std::set<state_t>{2});
    const std::unordered_set<state_t> reach3 = NotReachingStates(TestAutomaton1(), std::set<state_t>{3});
    CHECK(CheckStateRangeEquivalence(reach0, std::set<state_t>{1, 2, 3}));
    CHECK(CheckStateRangeEquivalence(reach1, std::set<state_t>{3}));
    CHECK(CheckStateRangeEquivalence(reach2, std::set<state_t>{3}));
    CHECK(CheckStateRangeEquivalence(reach3, std::set<state_t>{}));

    NondeterministicAutomaton test_automaton2(0);
    test_automaton2.AddState(0);
    test_automaton2.AddState(1);
    CHECK(CheckStateRangeEquivalence(NotReachingStates(test_automaton2, std::set<state_t>{0, 1}), std::set<state_t>{}));
}

TEST_CASE("Test StronglyConnectedComponents.") {
    return;
    const SCCCollection sccs = StronglyConnectedComponents(TestAutomaton1());
    CHECK(sccs.sccs.size() == 3);
    CHECK(sccs.scc_indices.size() == 4);
    CHECK(sccs.scc_indices.at(0) != sccs.scc_indices.at(1));
    CHECK(sccs.scc_indices.at(0) != sccs.scc_indices.at(2));
    CHECK(sccs.scc_indices.at(0) != sccs.scc_indices.at(3));
    CHECK(sccs.scc_indices.at(1) == sccs.scc_indices.at(2));
    CHECK(sccs.scc_indices.at(1) != sccs.scc_indices.at(3));
    CHECK(sccs.scc_indices.at(2) != sccs.scc_indices.at(3));
    REQUIRE(sccs.scc_indices.size() == 4);
    const auto& scc1 = sccs.sccs[sccs.scc_indices.at(0)];
    const auto& scc2 = sccs.sccs[sccs.scc_indices.at(1)];
    const auto& scc3 = sccs.sccs[sccs.scc_indices.at(3)];
    CHECK(scc1.size() == 1);
    CHECK(scc1.find(0) != scc1.end());
    CHECK(scc2.size() == 2);
    CHECK(scc2.find(1) != scc1.end());
    CHECK(scc2.find(2) != scc1.end());
    CHECK(scc3.size() == 1);
    CHECK(scc3.find(3) != scc1.end());
}

TEST_CASE("Test StronglyConnectedComponents #2.") {
    DeterministicAutomaton automaton(1);
    automaton.AddState(0);
    automaton.AddState(1);
    automaton.AddState(2);
    automaton.AddState(3);
    automaton.SetSucc(0, 0, 1);
    automaton.SetSucc(0, 1, 2);
    automaton.SetSucc(1, 0, 0);
    automaton.SetSucc(1, 1, 3);
    automaton.SetSucc(2, 0, 3);
    automaton.SetSucc(2, 1, 3);
    automaton.SetSucc(3, 0, 3);
    automaton.SetSucc(3, 1, 3);
    const SCCCollection sccs = StronglyConnectedComponents(automaton);

    REQUIRE(sccs.sccs.size() == 3);
    const auto& scc1 = sccs.sccs[sccs.scc_indices.at(0)];
    const auto& scc2 = sccs.sccs[sccs.scc_indices.at(2)];
    const auto& scc3 = sccs.sccs[sccs.scc_indices.at(3)];

    CHECK(scc1.size() == 2);
    CHECK(scc1.find(0) != scc1.end());
    CHECK(scc1.find(1) != scc1.end());
    CHECK(scc2.size() == 1);
    CHECK(scc2.find(2) != scc2.end());
    CHECK(scc3.size() == 1);
    CHECK(scc3.find(3) != scc3.end());
}

TEST_CASE("Test MergeStates.") {
    const NondeterministicAutomaton automaton = MergeStates(TestAutomaton1(), std::set<state_t>{2, 3});
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


TEST_CASE("Test MergeSCCs.") {
    NondeterministicAutomaton automaton = NondeterministicAutomaton::FromTransitionAutomaton(TestAutomaton3());

    const SCCCollection sccs = StronglyConnectedComponents(automaton);

    MergeSCCs(&automaton);
    REQUIRE(automaton.States().size() == 2);
    const state_t rep1 = *(sccs.sccs.at(sccs.scc_indices.at(0)).begin());
    const state_t rep2 = *(sccs.sccs.at(sccs.scc_indices.at(2)).begin());
    CHECK(CheckStateRangeEquivalence(automaton.States(), std::set<state_t>{rep1, rep2}));
    CHECK(automaton.InitialState() == rep1);
    CHECK(CheckStateRangeEquivalence(automaton.Successors(rep1, 0), std::set<state_t>{rep1}));
    CHECK(CheckStateRangeEquivalence(automaton.Successors(rep1, 1), std::set<state_t>{rep2}));
    CHECK(CheckStateRangeEquivalence(automaton.Successors(rep2, 0), std::set<state_t>{rep2}));
    CHECK(CheckStateRangeEquivalence(automaton.Successors(rep2, 1), std::set<state_t>{rep2}));

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

TEST_CASE("Test ProductAutomaton.") {
    boost::bimap<state_t, std::pair<state_t, state_t>> pair_indices;
    const NondeterministicAutomaton automaton = TestAutomaton1();
    const NondeterministicAutomaton product = ProductAutomaton(automaton, automaton, &pair_indices);

    REQUIRE(product.States().size() == 16);
    REQUIRE(product.atomicPropositions == 1);

    const state_t q00 = pair_indices.right.at(std::make_pair(0, 0));
    const state_t q01 = pair_indices.right.at(std::make_pair(0, 1));
    const state_t q02 = pair_indices.right.at(std::make_pair(0, 2));
    const state_t q03 = pair_indices.right.at(std::make_pair(0, 3));
    const state_t q10 = pair_indices.right.at(std::make_pair(1, 0));
    const state_t q11 = pair_indices.right.at(std::make_pair(1, 1));
    const state_t q12 = pair_indices.right.at(std::make_pair(1, 2));
    const state_t q13 = pair_indices.right.at(std::make_pair(1, 3));
    const state_t q20 = pair_indices.right.at(std::make_pair(2, 0));
    const state_t q21 = pair_indices.right.at(std::make_pair(2, 1));
    const state_t q22 = pair_indices.right.at(std::make_pair(2, 2));
    const state_t q23 = pair_indices.right.at(std::make_pair(2, 3));
    const state_t q30 = pair_indices.right.at(std::make_pair(3, 0));
    const state_t q31 = pair_indices.right.at(std::make_pair(3, 1));
    const state_t q32 = pair_indices.right.at(std::make_pair(3, 2));
    const state_t q33 = pair_indices.right.at(std::make_pair(3, 3));

    CHECK(product.InitialState() == q00);

    CHECK(CheckStateRangeEquivalence(product.Successors(q00, 0), std::vector<state_t>{q00, q01, q10, q11}));
    CHECK(CheckStateRangeEquivalence(product.Successors(q00, 1), std::vector<state_t>{}));
    CHECK(CheckStateRangeEquivalence(product.Successors(q01, 0), std::vector<state_t>{q02, q12}));
    CHECK(CheckStateRangeEquivalence(product.Successors(q01, 1), std::vector<state_t>{}));
    CHECK(CheckStateRangeEquivalence(product.Successors(q02, 0), std::vector<state_t>{q01, q11}));
    CHECK(CheckStateRangeEquivalence(product.Successors(q02, 1), std::vector<state_t>{}));
    CHECK(CheckStateRangeEquivalence(product.Successors(q03, 0), std::vector<state_t>{q03, q13}));
    CHECK(CheckStateRangeEquivalence(product.Successors(q03, 1), std::vector<state_t>{}));
    CHECK(CheckStateRangeEquivalence(product.Successors(q10, 0), std::vector<state_t>{q20, q21}));
    CHECK(CheckStateRangeEquivalence(product.Successors(q10, 1), std::vector<state_t>{}));
    CHECK(CheckStateRangeEquivalence(product.Successors(q11, 0), std::vector<state_t>{q22}));
    CHECK(CheckStateRangeEquivalence(product.Successors(q11, 1), std::vector<state_t>{q22}));
    CHECK(CheckStateRangeEquivalence(product.Successors(q12, 0), std::vector<state_t>{q21}));
    CHECK(CheckStateRangeEquivalence(product.Successors(q12, 1), std::vector<state_t>{q23}));
    CHECK(CheckStateRangeEquivalence(product.Successors(q13, 0), std::vector<state_t>{q23}));
    CHECK(CheckStateRangeEquivalence(product.Successors(q13, 1), std::vector<state_t>{}));
    CHECK(CheckStateRangeEquivalence(product.Successors(q20, 0), std::vector<state_t>{q10, q11}));
    CHECK(CheckStateRangeEquivalence(product.Successors(q20, 1), std::vector<state_t>{}));
    CHECK(CheckStateRangeEquivalence(product.Successors(q21, 0), std::vector<state_t>{q12}));
    CHECK(CheckStateRangeEquivalence(product.Successors(q21, 1), std::vector<state_t>{q32}));
    CHECK(CheckStateRangeEquivalence(product.Successors(q22, 0), std::vector<state_t>{q11}));
    CHECK(CheckStateRangeEquivalence(product.Successors(q22, 1), std::vector<state_t>{q33}));
    CHECK(CheckStateRangeEquivalence(product.Successors(q23, 0), std::vector<state_t>{q13}));
    CHECK(CheckStateRangeEquivalence(product.Successors(q23, 1), std::vector<state_t>{}));
    CHECK(CheckStateRangeEquivalence(product.Successors(q30, 0), std::vector<state_t>{q30, q31}));
    CHECK(CheckStateRangeEquivalence(product.Successors(q30, 1), std::vector<state_t>{}));
    CHECK(CheckStateRangeEquivalence(product.Successors(q31, 0), std::vector<state_t>{q32}));
    CHECK(CheckStateRangeEquivalence(product.Successors(q31, 1), std::vector<state_t>{}));
    CHECK(CheckStateRangeEquivalence(product.Successors(q32, 0), std::vector<state_t>{q31}));
    CHECK(CheckStateRangeEquivalence(product.Successors(q32, 1), std::vector<state_t>{}));
    CHECK(CheckStateRangeEquivalence(product.Successors(q33, 0), std::vector<state_t>{q33}));
    CHECK(CheckStateRangeEquivalence(product.Successors(q33, 1), std::vector<state_t>{}));

    CHECK(product == ProductAutomaton(automaton, automaton));
}

TEST_CASE("Test ProductAutomaton. (deterministic)") {
    boost::bimap<state_t, std::pair<state_t, state_t>> pair_indices;
    const DeterministicAutomaton automaton1 = TestAutomaton4();
    const DeterministicAutomaton automaton2 = TestAutomaton3();
    const DeterministicAutomaton product = ProductAutomaton(automaton1, automaton2, &pair_indices);

    REQUIRE(product.States().size() == 10);
    REQUIRE(product.atomicPropositions == 1);

    const state_t q10 = pair_indices.right.at(std::make_pair(1, 0));
    const state_t q11 = pair_indices.right.at(std::make_pair(1, 1));
    const state_t q12 = pair_indices.right.at(std::make_pair(1, 2));
    const state_t q13 = pair_indices.right.at(std::make_pair(1, 3));
    const state_t q14 = pair_indices.right.at(std::make_pair(1, 4));
    const state_t q20 = pair_indices.right.at(std::make_pair(2, 0));
    const state_t q21 = pair_indices.right.at(std::make_pair(2, 1));
    const state_t q22 = pair_indices.right.at(std::make_pair(2, 2));
    const state_t q23 = pair_indices.right.at(std::make_pair(2, 3));
    const state_t q24 = pair_indices.right.at(std::make_pair(2, 4));
    CHECK(product.InitialState() == q10);

    CHECK(product.Succ(q10, 0) == q21);
    CHECK(product.Succ(q10, 1) == q12);
    CHECK(product.Succ(q11, 0) == q20);
    CHECK(product.Succ(q11, 1) == q14);
    CHECK(product.Succ(q12, 0) == q23);
    CHECK(product.Succ(q12, 1) == q13);
    CHECK(product.Succ(q13, 0) == q22);
    CHECK(product.Succ(q13, 1) == q14);
    CHECK(product.Succ(q14, 0) == q23);
    CHECK(product.Succ(q14, 1) == q13);
    CHECK(product.Succ(q20, 0) == q21);
    CHECK(product.Succ(q20, 1) == q12);
    CHECK(product.Succ(q21, 0) == q20);
    CHECK(product.Succ(q21, 1) == q14);
    CHECK(product.Succ(q22, 0) == q23);
    CHECK(product.Succ(q22, 1) == q13);
    CHECK(product.Succ(q23, 0) == q22);
    CHECK(product.Succ(q23, 1) == q14);
    CHECK(product.Succ(q24, 0) == q23);
    CHECK(product.Succ(q24, 1) == q13);

    CHECK(product == ProductAutomaton(automaton1, automaton2));
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

TEST_CASE("Test QuotientAutomaton.") {
    NondeterministicAutomaton automaton = TestAutomaton1();
    automaton.AddSucc(3, 1, 0);
    EquivalenceRelation<state_t> quotient;
    quotient.AddConnection(0, 3);
    quotient.AddConnection(1, 1);
    quotient.AddConnection(2, 2);
    QuotientAutomaton(&automaton, quotient);

    CHECK(automaton.InitialState() == 0);
    CHECK(automaton.HasState(0));
    CHECK(automaton.HasState(1));
    CHECK(automaton.HasState(2));
    CHECK(!automaton.HasState(3));
    CHECK(CheckStateRangeEquivalence(automaton.Successors(0, 0), std::vector<state_t>{0, 1}));
    CHECK(CheckStateRangeEquivalence(automaton.Successors(0, 1), std::vector<state_t>{0}));
    CHECK(CheckStateRangeEquivalence(automaton.Successors(1, 0), std::vector<state_t>{2}));
    CHECK(CheckStateRangeEquivalence(automaton.Successors(1, 1), std::vector<state_t>{2}));
    CHECK(CheckStateRangeEquivalence(automaton.Successors(2, 0), std::vector<state_t>{1}));
    CHECK(CheckStateRangeEquivalence(automaton.Successors(2, 1), std::vector<state_t>{0}));
}