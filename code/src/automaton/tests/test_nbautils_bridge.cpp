

#include "catch.hpp"
#include "../nondeterministic_automaton.h"
#include "../nbautils_bridge.h"
#include <swa.hh>
#include "helper_functions.h"
#include <range/v3/empty.hpp>
#include <range/v3/view/all.hpp>

using namespace tollk;
using namespace automaton;

//TODO these tests should probably be more exhaustive

NondeterministicAutomaton TestAutomaton1();

TEST_CASE("Test FromNbautils.") {
    nbautils::SWA<void*> swa(nbautils::Acceptance::PARITY, "", std::vector<std::string>{"a"}, std::vector<nbautils::state_t> {0});
    swa.add_state(0);
    swa.add_state(1);
    swa.add_state(2);
    swa.add_state(3);
    swa.set_succs(0, 0, std::vector<state_t>{0, 1});
    swa.set_succs(1, 0, std::vector<state_t>{2});
    swa.set_succs(1, 1, std::vector<state_t>{2});
    swa.set_succs(2, 0, std::vector<state_t>{1});
    swa.set_succs(2, 1, std::vector<state_t>{3});
    swa.set_succs(3, 0, std::vector<state_t>{3});
    CHECK(FromNbautils(swa) == TestAutomaton1());
}


TEST_CASE("Test ToNbautils.") {
    const nbautils::SWA<void*> swa = ToNbautils(TestAutomaton1());
    CHECK(swa.num_states() == 4);
    CHECK(swa.states() == std::vector<state_t>{0, 1, 2, 3});
    CHECK(swa.get_init() == std::vector<state_t>{0});
    CHECK(swa.get_aps().size() == 1);
    CHECK(( (swa.succ(0, 0) == std::vector<state_t>{0, 1}) || (swa.succ(0, 0) == std::vector<state_t>{1, 0}) ));
    CHECK(swa.succ(0, 1) == std::vector<state_t>{});
    CHECK(swa.succ(1, 0) == std::vector<state_t>{2});
    CHECK(swa.succ(1, 1) == std::vector<state_t>{2});
    CHECK(swa.succ(2, 0) == std::vector<state_t>{1});
    CHECK(swa.succ(2, 1) == std::vector<state_t>{3});
    CHECK(swa.succ(3, 0) == std::vector<state_t>{3});
    CHECK(swa.succ(3, 1) == std::vector<state_t>{});
}

TEST_CASE("Test FromNbautils on a more complex input.") {
    nbautils::SWA<void*> swa(nbautils::Acceptance::PARITY, "", std::vector<std::string>{"a"}, std::vector<nbautils::state_t> {0, 1});
    swa.add_state(0);
    swa.add_state(1);
    const NPA automaton = FromNbautils(swa);
    CHECK(automaton.States().size() == 3);
    CHECK(CheckStateRangeEquivalence(automaton.Successors(0), std::vector<state_t>{}));
    CHECK(CheckStateRangeEquivalence(automaton.Successors(1), std::vector<state_t>{}));
    CHECK(CheckStateRangeEquivalence(automaton.Successors(automaton.InitialState(), 0), std::vector<state_t>{0,1}));
    CHECK(CheckStateRangeEquivalence(automaton.Successors(automaton.InitialState(), 1), std::vector<state_t>{0,1}));
}