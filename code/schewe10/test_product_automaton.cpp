
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "swa.hh"
#include "product_automaton.h"

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


TEST_CASE("The product automaton of an example automaton provided by Sven Schewe with itself.") {
    const nbautils::SWA<int> swa = sven_example();
    const nbautils::SWA<std::pair<int ,int>> product = ProductAutomaton(swa, swa);
    REQUIRE(product.num_states() == 25);

    CHECK(product.get_init() == std::vector<nbautils::state_t> {CompID2ProdID(0, 0, swa.num_states())});

    // Introduce some helper abbreviations to shorten the checks.
    const unsigned int swa_states = swa.num_states();
    const auto succ_set = [&product,swa_states](nbautils::state_t p, nbautils::state_t q, nbautils::sym_t sym) {
        const std::vector<nbautils::state_t> succ_vec = product.succ(CompID2ProdID(p, q, swa_states), sym);
        return std::set<nbautils::state_t>(succ_vec.begin(), succ_vec.end());
    };
    const auto singset = [swa_states](nbautils::state_t p, nbautils::state_t q) {
        return std::set<nbautils::state_t> {CompID2ProdID(p, q, swa_states)};
    };

    // Check transitions.
    CHECK(succ_set(0, 0, 0) == singset(1, 1));
    CHECK(succ_set(0, 0, 1) == singset(2, 2));
    CHECK(succ_set(0, 1, 0) == singset(1, 0));
    CHECK(succ_set(0, 1, 1) == singset(2, 4));
    CHECK(succ_set(0, 2, 0) == singset(1, 3));
    CHECK(succ_set(0, 2, 1) == singset(2, 3));
    CHECK(succ_set(0, 3, 0) == singset(1, 2));
    CHECK(succ_set(0, 3, 1) == singset(2, 4));
    CHECK(succ_set(0, 4, 0) == singset(1, 3));
    CHECK(succ_set(0, 4, 1) == singset(2, 3));

    CHECK(succ_set(1, 0, 0) == singset(0, 1));
    CHECK(succ_set(1, 0, 1) == singset(4, 2));
    CHECK(succ_set(1, 1, 0) == singset(0, 0));
    CHECK(succ_set(1, 1, 1) == singset(4, 4));
    CHECK(succ_set(1, 2, 0) == singset(0, 3));
    CHECK(succ_set(1, 2, 1) == singset(4, 3));
    CHECK(succ_set(1, 3, 0) == singset(0, 2));
    CHECK(succ_set(1, 3, 1) == singset(4, 4));
    CHECK(succ_set(1, 4, 0) == singset(0, 3));
    CHECK(succ_set(1, 4, 1) == singset(4, 3));

    CHECK(succ_set(2, 0, 0) == singset(3, 1));
    CHECK(succ_set(2, 0, 1) == singset(3, 2));
    CHECK(succ_set(2, 1, 0) == singset(3, 0));
    CHECK(succ_set(2, 1, 1) == singset(3, 4));
    CHECK(succ_set(2, 2, 0) == singset(3, 3));
    CHECK(succ_set(2, 2, 1) == singset(3, 3));
    CHECK(succ_set(2, 3, 0) == singset(3, 2));
    CHECK(succ_set(2, 3, 1) == singset(3, 4));
    CHECK(succ_set(2, 4, 0) == singset(3, 3));
    CHECK(succ_set(2, 4, 1) == singset(3, 3));

    CHECK(succ_set(3, 0, 0) == singset(2, 1));
    CHECK(succ_set(3, 0, 1) == singset(4, 2));
    CHECK(succ_set(3, 1, 0) == singset(2, 0));
    CHECK(succ_set(3, 1, 1) == singset(4, 4));
    CHECK(succ_set(3, 2, 0) == singset(2, 3));
    CHECK(succ_set(3, 2, 1) == singset(4, 3));
    CHECK(succ_set(3, 3, 0) == singset(2, 2));
    CHECK(succ_set(3, 3, 1) == singset(4, 4));
    CHECK(succ_set(3, 4, 0) == singset(2, 3));
    CHECK(succ_set(3, 4, 1) == singset(4, 3));

    CHECK(succ_set(4, 0, 0) == singset(3, 1));
    CHECK(succ_set(4, 0, 1) == singset(3, 2));
    CHECK(succ_set(4, 1, 0) == singset(3, 0));
    CHECK(succ_set(4, 1, 1) == singset(3, 4));
    CHECK(succ_set(4, 2, 0) == singset(3, 3));
    CHECK(succ_set(4, 2, 1) == singset(3, 3));
    CHECK(succ_set(4, 3, 0) == singset(3, 2));
    CHECK(succ_set(4, 3, 1) == singset(3, 4));
    CHECK(succ_set(4, 4, 0) == singset(3, 3));
    CHECK(succ_set(4, 4, 1) == singset(3, 3));
}