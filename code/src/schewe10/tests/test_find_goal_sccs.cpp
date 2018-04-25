
#include "catch.hpp"
#include "swa.hh"
#include "../find_goal_sccs.h"

nbautils::SWA<int> sven_example();

TEST_CASE("Trivial SCCS in an example automaton provided by Sven Schewe.") {
    const nbautils::SWA<int> swa = sven_example();
    CHECK(!SCCIsTrivial(std::set<nbautils::state_t> {0, 1}, swa));
    CHECK(!SCCIsTrivial(std::set<nbautils::state_t> {2, 3, 4}, swa));
}

TEST_CASE("Goal SCCS in an example automaton provided by Sven Schewe.") {
    const nbautils::SWA<int> swa = sven_example();
    CHECK(!SCCContainsAccepting(std::set<nbautils::state_t> {0, 1}, std::set<nbautils::state_t> {4}));
    CHECK(SCCContainsAccepting(std::set<nbautils::state_t> {2, 3, 4}, std::set<nbautils::state_t> {4}));
}