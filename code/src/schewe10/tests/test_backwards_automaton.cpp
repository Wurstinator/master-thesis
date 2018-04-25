
#include "catch.hpp"
#include "swa.hh"
#include "../backwards_automaton.h"
#include <map>
#include <set>

nbautils::SWA<int> sven_example();

template <typename T>
std::set<T> VectorToSet(const std::vector<T>& vec) {
    return std::set<T>(vec.begin(), vec.end());
}

TEST_CASE("") {
    const nbautils::SWA<int> swa = sven_example();
    const nbautils::SWA<int> backw = BackwardsAutomaton(swa);
    REQUIRE(backw.num_states() == 5);

    using StateSet = std::set<nbautils::state_t>;

    CHECK(VectorToSet(backw.succ(0, 0)) == StateSet {1});
    CHECK(VectorToSet(backw.succ(0, 1)) == StateSet {});
    CHECK(VectorToSet(backw.succ(1, 0)) == StateSet {0});
    CHECK(VectorToSet(backw.succ(1, 1)) == StateSet {});
    CHECK(VectorToSet(backw.succ(2, 0)) == StateSet {3});
    CHECK(VectorToSet(backw.succ(2, 1)) == StateSet {0});
    CHECK(VectorToSet(backw.succ(3, 0)) == StateSet {2,4});
    CHECK(VectorToSet(backw.succ(3, 1)) == StateSet {2,4});
    CHECK(VectorToSet(backw.succ(4, 0)) == StateSet {});
    CHECK(VectorToSet(backw.succ(4, 1)) == StateSet {3,1});
}