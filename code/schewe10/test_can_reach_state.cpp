
#include "catch.hpp"
#include "swa.hh"
#include "can_reach_state.h"

nbautils::SWA<int> sven_example();


template<typename T>
std::set<T> VectorToSet(const std::vector<T>& vec) {
    return std::set<T>(vec.begin(), vec.end());
}

TEST_CASE("Compute (un)reachable states in an example automaton by Sven Schewe.") {
    const nbautils::SWA<int> swa = sven_example();
    using StateSet = std::set<nbautils::state_t>;

    CHECK(VectorToSet(CanReachState(swa, 0)) == StateSet{0, 1});
    CHECK(VectorToSet(CanReachState(swa, 1)) == StateSet{0, 1});
    CHECK(VectorToSet(CanReachState(swa, 2)) == StateSet{0, 1, 2, 3, 4});
    CHECK(VectorToSet(CanReachState(swa, 3)) == StateSet{0, 1, 2, 3, 4});
    CHECK(VectorToSet(CanReachState(swa, 4)) == StateSet{0, 1, 2, 3, 4});
    CHECK(VectorToSet(CannotReachState(swa, 0)) == StateSet{2, 3, 4});
    CHECK(VectorToSet(CannotReachState(swa, 1)) == StateSet{2, 3, 4});
    CHECK(VectorToSet(CannotReachState(swa, 2)) == StateSet{});
    CHECK(VectorToSet(CannotReachState(swa, 3)) == StateSet{});
    CHECK(VectorToSet(CannotReachState(swa, 4)) == StateSet{});
}