
#include "catch.hpp"

#include "helper_functions.h"

#include "../finite_automaton.h"
#include "../transition_automaton.h"
#include "../labelled_automaton.h"
#include "../deterministic_automaton.h"
#include "../nondeterministic_automaton.h"
#include "../parity.h"

using namespace tollk;
using namespace automaton;

NondeterministicAutomaton TestAutomaton1();
DeterministicAutomaton TestAutomaton2();
DeterministicAutomaton TestAutomaton3();


TEST_CASE("Test functionality of FiniteAutomaton class.") {
    FiniteAutomaton automaton;
    CHECK(automaton.States().empty());
    CHECK(!automaton.HasState(0));
    CHECK(!automaton.HasState(1));
    automaton.AddState(0);
    CHECK(automaton.States().size() == 1);
    CHECK(automaton.HasState(0));
    CHECK(!automaton.HasState(1));
    automaton.SetInitialState(0);
    CHECK(automaton.InitialState() == 0);
    automaton.AddState(1);
    CHECK(automaton.States().size() == 2);
    CHECK(automaton.HasState(0));
    CHECK(automaton.InitialState() == 0);
    CHECK(automaton.HasState(1));
    automaton.RemoveState(0);
    CHECK(automaton.States().size() == 1);
    CHECK(!automaton.HasState(0));
    CHECK(automaton.HasState(1));
    automaton.RemoveState(0);
    CHECK(automaton.States().size() == 1);
    CHECK(!automaton.HasState(0));
    CHECK(automaton.HasState(1));
    automaton.RemoveState(1);
    CHECK(automaton.States().empty());
    CHECK(!automaton.HasState(0));
    CHECK(!automaton.HasState(1));
}

TEST_CASE("Test functionality of LabelledAutomaton class.") {
    LabelledAutomaton<std::string> automaton;
    CHECK(automaton.States().empty());
    CHECK(!automaton.HasState(0));
    CHECK(!automaton.HasState(1));
    automaton.AddState(0);
    CHECK(automaton.States().size() == 1);
    CHECK(automaton.HasState(0));
    CHECK(!automaton.HasState(1));
    CHECK(automaton.GetLabel(0) == "");
    automaton.SetLabel(0, "foo");
    CHECK(automaton.GetLabel(0) == "foo");
}

TEST_CASE("Test LabelledAutomaton::LabelEquivalence.") {
    LabelledAutomaton<std::string> automaton;
    const EquivalenceRelation<state_t> er1 = automaton.LabelEquivalence();
    CHECK(er1.Classes().empty());

    automaton.AddState(0);
    automaton.AddState(1);
    automaton.AddState(2);
    automaton.SetLabel(1, "foo");
    const EquivalenceRelation<state_t> er2 = automaton.LabelEquivalence();
    CHECK(er2.Classes().size() == 2);
    CHECK(er2.IsEquiv(0, 0));
    CHECK(!er2.IsEquiv(0, 1));
    CHECK(er2.IsEquiv(0, 2));
    CHECK(!er2.IsEquiv(1, 0));
    CHECK(er2.IsEquiv(1, 1));
    CHECK(!er2.IsEquiv(1, 2));
    CHECK(er2.IsEquiv(2, 0));
    CHECK(!er2.IsEquiv(2, 1));
    CHECK(er2.IsEquiv(2, 2));
}

TEST_CASE("Test LabelledAutomaton::AllLabels.") {
    LabelledAutomaton<std::string> automaton;
    automaton.AddState(0);
    automaton.AddState(1);
    automaton.AddState(2);
    automaton.SetLabel(0, "foo");
    automaton.SetLabel(1, "bar");
    automaton.SetLabel(2, "baz");
    automaton.RemoveState(2);

    const std::unordered_set<std::string> all_labels = automaton.AllLabels();
    CHECK(all_labels.size() == 2);
    CHECK(all_labels.find("foo") != all_labels.end());
    CHECK(all_labels.find("bar") != all_labels.end());
    CHECK(all_labels.find("baz") == all_labels.end());
}

TEST_CASE("Test functionality of TransitionAutomaton class.") {
    const DeterministicAutomaton automaton1(1);
    const DeterministicAutomaton automaton2(3);
    CHECK(automaton1.atomicPropositions == 1);
    CHECK(automaton1.SymbolsNum() == 2);
    CHECK(automaton2.atomicPropositions == 3);
    CHECK(automaton2.SymbolsNum() == 8);
    const auto symbols1 = automaton1.Symbols();
    const std::vector<symbol_t> symvec1(std::begin(symbols1), std::end(symbols1));
    const auto symbols2 = automaton2.Symbols();
    const std::vector<symbol_t> symvec2(std::begin(symbols2), std::end(symbols2));
    CHECK(symvec1 == std::vector<symbol_t>{0, 1});
    CHECK(symvec2 == std::vector<symbol_t>{0, 1, 2, 3, 4, 5, 6, 7});
}

TEST_CASE("Test TransitionAutomaton::IsDeterministic.") {
    CHECK(!TestAutomaton1().IsDeterministic());
    CHECK(TestAutomaton2().IsDeterministic());
    CHECK(TestAutomaton3().IsDeterministic());
}


TEST_CASE("Test TransitionAutomaton::operator==.") {
    DeterministicAutomaton aut1(1), aut2(1), aut3(1), aut4(1), aut5(2);
    aut1.AddState(0);
    aut1.AddState(1);
    aut2.AddState(0);
    aut2.AddState(1);
    aut3.AddState(0);
    aut4.AddState(1);
    aut4.AddState(2);
    aut5.AddState(0);
    aut2.AddState(1);
    aut2.SetInitialState(1);
    aut1.SetInitialState(0);
    CHECK(aut1 != aut2);
    CHECK(aut1 != aut3);
    CHECK(aut1 != aut4);
    CHECK(aut1 != aut5);

    DeterministicAutomaton aut6(0), aut7(0), aut8(0), aut9(1);
    aut6.AddState(0);
    aut6.AddState(1);
    aut7.AddState(0);
    aut7.AddState(1);
    aut8.AddState(1);
    aut8.AddState(0);
    aut9.AddState(0);
    aut9.AddState(1);
    aut6.SetSucc(0, 0, 1);
    aut6.SetSucc(1, 0, 1);
    aut7.SetSucc(0, 0, 0);
    aut7.SetSucc(1, 0, 1);
    aut8.SetSucc(0, 0, 1);
    aut8.SetSucc(1, 0, 1);
    aut9.SetSucc(0, 0, 1);
    aut9.SetSucc(1, 0, 1);
    CHECK(aut6 != aut7);
    CHECK(aut6 == aut8);
    CHECK(aut6 != aut9);

    NondeterministicAutomaton aut10(0), aut11(0), aut12(0), aut13(0);
    aut10.AddState(0);
    aut10.AddState(1);
    aut11.AddState(0);
    aut11.AddState(1);
    aut12.AddState(0);
    aut12.AddState(1);
    aut13.AddState(0);
    aut13.AddState(1);
    aut10.AddSucc(0, 0, 1);
    aut12.AddSucc(0, 0, 0);
    aut13.AddSucc(0, 0, 1);
    CHECK(aut10 != aut11);
    CHECK(aut10 != aut12);
    CHECK(aut10 == aut13);

    aut10.AddSucc(1, 0, 1);
    CHECK(aut6 == aut10);
    CHECK(aut6 != aut11);
}

TEST_CASE("Test functionality of DeterministicAutomaton class.") {
    DeterministicAutomaton automaton(1);
    automaton.AddState(0);
    automaton.AddState(1);
    automaton.AddState(2);
    automaton.AddState(3);
    automaton.SetInitialState(0);
    automaton.SetSucc(0, 0, 1);
    automaton.SetSucc(0, 1, 2);
    automaton.SetSucc(1, 0, 0);
    automaton.SetSucc(1, 1, 3);
    automaton.SetSucc(2, 0, 2);
    automaton.SetSucc(2, 1, 2);
    automaton.SetSucc(3, 0, 3);
    automaton.SetSucc(3, 1, 2);

    CHECK(automaton.Succ(0, 0) == 1);
    CHECK(automaton.Succ(0, 1) == 2);
    CHECK(automaton.Succ(1, 0) == 0);
    CHECK(automaton.Succ(1, 1) == 3);
    CHECK(automaton.Succ(2, 0) == 2);
    CHECK(automaton.Succ(2, 1) == 2);
    CHECK(automaton.Succ(3, 0) == 3);
    CHECK(automaton.Succ(3, 1) == 2);

    CHECK(CheckStateRangeEquivalence(automaton.Successors(0, 0), std::set<state_t>{1}));
    CHECK(CheckStateRangeEquivalence(automaton.Successors(0, 1), std::set<state_t>{2}));
    CHECK(CheckStateRangeEquivalence(automaton.Successors(1, 0), std::set<state_t>{0}));
    CHECK(CheckStateRangeEquivalence(automaton.Successors(1, 1), std::set<state_t>{3}));
    CHECK(CheckStateRangeEquivalence(automaton.Successors(2, 0), std::set<state_t>{2}));
    CHECK(CheckStateRangeEquivalence(automaton.Successors(2, 1), std::set<state_t>{2}));
    CHECK(CheckStateRangeEquivalence(automaton.Successors(3, 0), std::set<state_t>{3}));
    CHECK(CheckStateRangeEquivalence(automaton.Successors(3, 1), std::set<state_t>{2}));

    CHECK(CheckStateRangeEquivalence(automaton.Successors(0), std::set<state_t>{1, 2}));
    CHECK(CheckStateRangeEquivalence(automaton.Successors(1), std::set<state_t>{0, 3}));
    CHECK(CheckStateRangeEquivalence(automaton.Successors(2), std::set<state_t>{2}));
    CHECK(CheckStateRangeEquivalence(automaton.Successors(3), std::set<state_t>{2, 3}));

    automaton.RedirectTransitions(2, 0);

    CHECK(automaton.Succ(0, 0) == 1);
    CHECK(automaton.Succ(0, 1) == 0);
    CHECK(automaton.Succ(1, 0) == 0);
    CHECK(automaton.Succ(1, 1) == 3);
    CHECK(automaton.Succ(2, 0) == 0);
    CHECK(automaton.Succ(2, 1) == 0);
    CHECK(automaton.Succ(3, 0) == 3);
    CHECK(automaton.Succ(3, 1) == 0);

    CHECK(CheckStateRangeEquivalence(automaton.Successors(0, 0), std::set<state_t>{1}));
    CHECK(CheckStateRangeEquivalence(automaton.Successors(0, 1), std::set<state_t>{0}));
    CHECK(CheckStateRangeEquivalence(automaton.Successors(1, 0), std::set<state_t>{0}));
    CHECK(CheckStateRangeEquivalence(automaton.Successors(1, 1), std::set<state_t>{3}));
    CHECK(CheckStateRangeEquivalence(automaton.Successors(2, 0), std::set<state_t>{0}));
    CHECK(CheckStateRangeEquivalence(automaton.Successors(2, 1), std::set<state_t>{0}));
    CHECK(CheckStateRangeEquivalence(automaton.Successors(3, 0), std::set<state_t>{3}));
    CHECK(CheckStateRangeEquivalence(automaton.Successors(3, 1), std::set<state_t>{0}));

    CHECK(CheckStateRangeEquivalence(automaton.Successors(0), std::set<state_t>{0, 1}));
    CHECK(CheckStateRangeEquivalence(automaton.Successors(1), std::set<state_t>{0, 3}));
    CHECK(CheckStateRangeEquivalence(automaton.Successors(2), std::set<state_t>{0}));
    CHECK(CheckStateRangeEquivalence(automaton.Successors(3), std::set<state_t>{0, 3}));
}


TEST_CASE("Test DeterministicAutomaton::FromTransitionAutomaton.") {
    CHECK(DeterministicAutomaton::FromTransitionAutomaton(TestAutomaton2()) == TestAutomaton2());
    CHECK_THROWS(DeterministicAutomaton::FromTransitionAutomaton(TestAutomaton1()));
}


TEST_CASE("Test DeterministicAutomaton::MergeStates.") {
    // TODO
    /*NondeterministicAutomaton automaton = TestAutomaton1();
    automaton.SetInitialState(3);
    automaton.MergeStates(std::set<state_t>{2, 3});
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
    CHECK(automaton.InitialState() == 2); */
}

TEST_CASE("Test functionality of NondeterministicAutomaton class.") {
    NondeterministicAutomaton automaton(1);
    automaton.AddState(0);
    automaton.AddState(1);
    automaton.AddState(2);
    automaton.AddState(3);
    automaton.SetInitialState(0);
    automaton.AddSucc(0, 0, 0);
    automaton.AddSucc(0, 0, 1);
    automaton.AddSucc(1, 0, 2);
    automaton.AddSucc(1, 1, 2);
    automaton.AddSucc(2, 0, 1);
    automaton.AddSucc(2, 1, 3);
    automaton.AddSucc(3, 0, 1);
    automaton.AddSucc(3, 0, 3);

    CHECK(CheckStateRangeEquivalence(automaton.Successors(0, 0), std::set<state_t>{0, 1}));
    CHECK(CheckStateRangeEquivalence(automaton.Successors(0, 1), std::set<state_t>{}));
    CHECK(CheckStateRangeEquivalence(automaton.Successors(1, 0), std::set<state_t>{2}));
    CHECK(CheckStateRangeEquivalence(automaton.Successors(1, 1), std::set<state_t>{2}));
    CHECK(CheckStateRangeEquivalence(automaton.Successors(2, 0), std::set<state_t>{1}));
    CHECK(CheckStateRangeEquivalence(automaton.Successors(2, 1), std::set<state_t>{3}));
    CHECK(CheckStateRangeEquivalence(automaton.Successors(3, 0), std::set<state_t>{1, 3}));
    CHECK(CheckStateRangeEquivalence(automaton.Successors(3, 1), std::set<state_t>{}));

    CHECK(CheckStateRangeEquivalence(automaton.Successors(0), std::set<state_t>{0, 1}));
    CHECK(CheckStateRangeEquivalence(automaton.Successors(1), std::set<state_t>{2}));
    CHECK(CheckStateRangeEquivalence(automaton.Successors(2), std::set<state_t>{1, 3}));
    CHECK(CheckStateRangeEquivalence(automaton.Successors(3), std::set<state_t>{1, 3}));

    automaton.RedirectTransitions(0, 3);
    automaton.DeleteTransitionsTo(2);

    CHECK(CheckStateRangeEquivalence(automaton.Successors(0, 0), std::set<state_t>{1, 3}));
    CHECK(CheckStateRangeEquivalence(automaton.Successors(0, 1), std::set<state_t>{}));
    CHECK(CheckStateRangeEquivalence(automaton.Successors(1, 0), std::set<state_t>{}));
    CHECK(CheckStateRangeEquivalence(automaton.Successors(1, 1), std::set<state_t>{}));
    CHECK(CheckStateRangeEquivalence(automaton.Successors(2, 0), std::set<state_t>{1}));
    CHECK(CheckStateRangeEquivalence(automaton.Successors(2, 1), std::set<state_t>{3}));
    CHECK(CheckStateRangeEquivalence(automaton.Successors(3, 0), std::set<state_t>{1, 3}));
    CHECK(CheckStateRangeEquivalence(automaton.Successors(3, 1), std::set<state_t>{}));

    CHECK(CheckStateRangeEquivalence(automaton.Successors(0), std::set<state_t>{1, 3}));
    CHECK(CheckStateRangeEquivalence(automaton.Successors(1), std::set<state_t>{}));
    CHECK(CheckStateRangeEquivalence(automaton.Successors(2), std::set<state_t>{1, 3}));
    CHECK(CheckStateRangeEquivalence(automaton.Successors(3), std::set<state_t>{1, 3}));
}

TEST_CASE("Test NondeterministicAutomaton::FromTransitionAutomaton.") {
    CHECK(NondeterministicAutomaton::FromTransitionAutomaton(TestAutomaton2()) == TestAutomaton2());
}

TEST_CASE("Test NondeterministicAutomaton::MergeStates.") {
    NondeterministicAutomaton automaton = TestAutomaton1();
    automaton.SetInitialState(3);
    automaton.MergeStates(std::set<state_t>{2, 3});
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
    CHECK(automaton.InitialState() == 2);
}


TEST_CASE("Test functionality of ParityAutomaton class.") {
    ParityAutomaton aut1, aut2;
    aut1.AddState(0);
    aut1.AddState(1);
    aut2.AddState(0);
    aut2.AddState(1);
    aut1.SetLabel(0, 0);
    aut1.SetLabel(1, 1);
    aut2.SetLabel(0, 0);
    aut2.SetLabel(1, 2);
    CHECK(aut1.IsBuchi());
    CHECK(!aut2.IsBuchi());
}