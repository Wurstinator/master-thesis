
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "swa.hh"
#include "merge_states.h"

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

TEST_CASE("Compute alm.eq. states of an example automaton provided by Sven Schewe") {
    const nbautils::SWA<int> swa = sven_example();
    //TODO
}