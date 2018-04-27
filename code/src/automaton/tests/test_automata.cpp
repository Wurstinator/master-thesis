
#include "catch.hpp"

#include <range/v3/to_container.hpp>
#include <range/v3/action/sort.hpp>
#include <range/v3/action/unique.hpp>

#include "../finite_automaton.h"
#include "../transition_automaton.h"
#include "../labelled_automaton.h"
#include "../deterministic_automaton.h"
#include "../nondeterministic_automaton.h"

using namespace tollk;
using namespace automaton;

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
    CHECK(symvec1 == std::vector<symbol_t> {0, 1});
    CHECK(symvec2 == std::vector<symbol_t> {0, 1, 2, 3, 4, 5, 6, 7});
}

template <typename Rng1, typename Rng2>
bool CheckEquivalence(Rng1 range1, Rng2 range2) {
    const std::vector<state_t> vec1 = ranges::v3::to_vector(range1);
    const std::set<state_t> set1(vec1.begin(), vec1.end());
    const std::vector<state_t> vec2 = ranges::v3::to_vector(range2);
    const std::set<state_t> set2(vec2.begin(), vec2.end());
    return set1 == set2;
};

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

    CHECK(CheckEquivalence(automaton.Successors(0, 0), std::set<state_t> {1}));
    CHECK(CheckEquivalence(automaton.Successors(0, 1), std::set<state_t> {2}));
    CHECK(CheckEquivalence(automaton.Successors(1, 0), std::set<state_t> {0}));
    CHECK(CheckEquivalence(automaton.Successors(1, 1), std::set<state_t> {3}));
    CHECK(CheckEquivalence(automaton.Successors(2, 0), std::set<state_t> {2}));
    CHECK(CheckEquivalence(automaton.Successors(2, 1), std::set<state_t> {2}));
    CHECK(CheckEquivalence(automaton.Successors(3, 0), std::set<state_t> {3}));
    CHECK(CheckEquivalence(automaton.Successors(3, 1), std::set<state_t> {2}));

    CHECK(CheckEquivalence(automaton.Successors(0), std::set<state_t> {1, 2}));
    CHECK(CheckEquivalence(automaton.Successors(1), std::set<state_t> {0, 3}));
    CHECK(CheckEquivalence(automaton.Successors(2), std::set<state_t> {2}));
    CHECK(CheckEquivalence(automaton.Successors(3), std::set<state_t> {2, 3}));

    automaton.RedirectTransitions(2, 0);

    CHECK(automaton.Succ(0, 0) == 1);
    CHECK(automaton.Succ(0, 1) == 0);
    CHECK(automaton.Succ(1, 0) == 0);
    CHECK(automaton.Succ(1, 1) == 3);
    CHECK(automaton.Succ(2, 0) == 0);
    CHECK(automaton.Succ(2, 1) == 0);
    CHECK(automaton.Succ(3, 0) == 3);
    CHECK(automaton.Succ(3, 1) == 0);

    CHECK(CheckEquivalence(automaton.Successors(0, 0), std::set<state_t> {1}));
    CHECK(CheckEquivalence(automaton.Successors(0, 1), std::set<state_t> {0}));
    CHECK(CheckEquivalence(automaton.Successors(1, 0), std::set<state_t> {0}));
    CHECK(CheckEquivalence(automaton.Successors(1, 1), std::set<state_t> {3}));
    CHECK(CheckEquivalence(automaton.Successors(2, 0), std::set<state_t> {0}));
    CHECK(CheckEquivalence(automaton.Successors(2, 1), std::set<state_t> {0}));
    CHECK(CheckEquivalence(automaton.Successors(3, 0), std::set<state_t> {3}));
    CHECK(CheckEquivalence(automaton.Successors(3, 1), std::set<state_t> {0}));

    CHECK(CheckEquivalence(automaton.Successors(0), std::set<state_t> {0, 1}));
    CHECK(CheckEquivalence(automaton.Successors(1), std::set<state_t> {0, 3}));
    CHECK(CheckEquivalence(automaton.Successors(2), std::set<state_t> {0}));
    CHECK(CheckEquivalence(automaton.Successors(3), std::set<state_t> {0, 3}));
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

    CHECK(CheckEquivalence(automaton.Successors(0, 0), std::set<state_t> {0, 1}));
    CHECK(CheckEquivalence(automaton.Successors(0, 1), std::set<state_t> {}));
    CHECK(CheckEquivalence(automaton.Successors(1, 0), std::set<state_t> {2}));
    CHECK(CheckEquivalence(automaton.Successors(1, 1), std::set<state_t> {2}));
    CHECK(CheckEquivalence(automaton.Successors(2, 0), std::set<state_t> {1}));
    CHECK(CheckEquivalence(automaton.Successors(2, 1), std::set<state_t> {3}));
    CHECK(CheckEquivalence(automaton.Successors(3, 0), std::set<state_t> {1, 3}));
    CHECK(CheckEquivalence(automaton.Successors(3, 1), std::set<state_t> {}));

    CHECK(CheckEquivalence(automaton.Successors(0), std::set<state_t> {0, 1}));
    CHECK(CheckEquivalence(automaton.Successors(1), std::set<state_t> {2}));
    CHECK(CheckEquivalence(automaton.Successors(2), std::set<state_t> {1, 3}));
    CHECK(CheckEquivalence(automaton.Successors(3), std::set<state_t> {1, 3}));

    automaton.RedirectTransitions(0, 3);
    automaton.DeleteTransitionsTo(2);

    CHECK(CheckEquivalence(automaton.Successors(0, 0), std::set<state_t> {1, 3}));
    CHECK(CheckEquivalence(automaton.Successors(0, 1), std::set<state_t> {}));
    CHECK(CheckEquivalence(automaton.Successors(1, 0), std::set<state_t> {}));
    CHECK(CheckEquivalence(automaton.Successors(1, 1), std::set<state_t> {}));
    CHECK(CheckEquivalence(automaton.Successors(2, 0), std::set<state_t> {1}));
    CHECK(CheckEquivalence(automaton.Successors(2, 1), std::set<state_t> {3}));
    CHECK(CheckEquivalence(automaton.Successors(3, 0), std::set<state_t> {1, 3}));
    CHECK(CheckEquivalence(automaton.Successors(3, 1), std::set<state_t> {}));

    CHECK(CheckEquivalence(automaton.Successors(0), std::set<state_t> {1, 3}));
    CHECK(CheckEquivalence(automaton.Successors(1), std::set<state_t> {}));
    CHECK(CheckEquivalence(automaton.Successors(2), std::set<state_t> {1, 3}));
    CHECK(CheckEquivalence(automaton.Successors(3), std::set<state_t> {1, 3}));
}