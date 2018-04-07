
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "swa.hh"
#include "backwards_automaton.h"
#include <map>
#include <set>

nbautils::SWA<int> sven_example() {
    nbautils::SWA<int> swa;
    swa.add_state(0);
    swa.add_state(1);
    swa.add_state(2);
    swa.add_state(3);
    swa.add_state(4);
    swa.set_init(std::vector<nbautils::state_t>{0});
    swa.set_aps(std::vector<std::string>{"a"});
    swa.set_accs(4, std::vector<nbautils::acc_t> {0});
    swa.set_succs(0, 0, std::vector<nbautils::state_t> {1});
    swa.set_succs(0, 1, std::vector<nbautils::state_t> {2});
    swa.set_succs(1, 0, std::vector<nbautils::state_t> {0});
    swa.set_succs(1, 1, std::vector<nbautils::state_t> {4});
    swa.set_succs(2, 0, std::vector<nbautils::state_t> {3});
    swa.set_succs(2, 1, std::vector<nbautils::state_t> {3});
    swa.set_succs(3, 0, std::vector<nbautils::state_t> {2});
    swa.set_succs(3, 1, std::vector<nbautils::state_t> {4});
    swa.set_succs(4, 0, std::vector<nbautils::state_t> {3});
    swa.set_succs(4, 1, std::vector<nbautils::state_t> {3});
    return swa;
}

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