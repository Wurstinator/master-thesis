

#include "catch.hpp"
#include "../nondeterministic_automaton.h"
#include "../nbautils_bridge.h"
#include <swa.hh>

using namespace tollk;
using namespace automaton;

NPA TestAutomaton1();

TEST_CASE("Test FromNbautils.") {
    nbautils::SWA<void*> swa;
    swa.set_aps(std::vector<std::string>{"a"});
    swa.add_state(0);
    swa.add_state(1);
    swa.add_state(2);
    swa.add_state(3);
    swa.set_init(std::vector<state_t>{0});
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

TEST_CASE("Test ToNbautils on a more complex input.") {
    //TODO
}