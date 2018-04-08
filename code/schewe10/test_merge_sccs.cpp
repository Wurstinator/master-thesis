
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "swa.hh"
#include "merge_sccs.h"

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

template<typename T>
std::set<T> VectorToSet(const std::vector<T>& vec) {
    return std::set<T>(vec.begin(), vec.end());
}

template<typename TagT>
void CheckEquivalentAutomata(const nbautils::SWA<TagT>& aut1, const nbautils::SWA<TagT>& aut2) {
    CHECK(aut1.states() == aut2.states());
    CHECK(aut1.get_init() == aut2.get_init());
    for (nbautils::state_t q : aut1.states())
        for (nbautils::sym_t sym = 0; sym < aut1.num_syms(); ++sym)
            CHECK(aut1.succ(q, sym) == aut2.succ(q, sym));
    for (nbautils::state_t q : aut1.states())
        CHECK(aut1.get_accs(q) == aut2.get_accs(q));
}

TEST_CASE("Compute alm.eq. states of an example automaton provided by Sven Schewe") {
    const nbautils::SWA<int> swa = sven_example();
    const nbautils::SWA<std::set<int>> merge1 = MergeSCCs(swa, nullptr);
    std::map<std::set<nbautils::state_t>, nbautils::state_t> representatives;
    const nbautils::SWA<std::set<int>> merge2 = MergeSCCs(swa, &representatives);

    using StateSet = std::set<nbautils::state_t>;

    // First check.
    CHECK(VectorToSet(merge1.states()) == StateSet {0, 2});
    CHECK(VectorToSet(merge1.get_init()) == StateSet {0});

    CHECK(VectorToSet(merge1.succ(0, 0)) == StateSet {0});
    CHECK(VectorToSet(merge1.succ(0, 1)) == StateSet {2});
    CHECK(VectorToSet(merge1.succ(2, 0)) == StateSet {2});
    CHECK(VectorToSet(merge1.succ(2, 1)) == StateSet {2});

    CHECK(merge1.get_accs(0) == std::vector<nbautils::acc_t> {});
    CHECK(merge1.get_accs(2) == std::vector<nbautils::acc_t> {0});

    // Second check.
    CheckEquivalentAutomata(merge1, merge2);

    // Check representatives.
    REQUIRE(representatives.size() == 2);
    CHECK(representatives[StateSet {0, 1}] == 0);
    CHECK(representatives[StateSet {2, 3, 4}] == 2);
}