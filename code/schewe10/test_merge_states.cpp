
#include "catch.hpp"
#include "swa.hh"
#include "merge_states.h"

nbautils::SWA<int> sven_example();

TEST_CASE("Test FindMergeRepresentative") {
    const nbautils::SWA<int> swa = sven_example();
    const std::vector<nbautils::state_t> set1 {0, 1};
    const std::vector<nbautils::state_t> set2 {1, 0};
    const std::vector<nbautils::state_t> set3 {2, 3, 4};
    CHECK(FindMergeRepresentative(swa, set1.begin(), set1.end()) == 0);
    CHECK(FindMergeRepresentative(swa, set2.begin(), set2.end()) == 0);
    CHECK(FindMergeRepresentative(swa, set3.begin(), set3.end()) == 2);
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

TEST_CASE("Test MergeStates") {
    const nbautils::SWA<int> swa = sven_example();

    nbautils::SWA<int> merge1 = swa;
    const nbautils::state_t rep1 = MergeStates(&merge1, std::set<nbautils::state_t> {0});
    nbautils::SWA<int> merge2 = swa;
    const nbautils::state_t rep2 = MergeStates(&merge2, std::set<nbautils::state_t> {0, 1});
    nbautils::SWA<int> merge3 = swa;
    const nbautils::state_t rep3 = MergeStates(&merge3, std::set<nbautils::state_t> {2, 3, 4});

    using StateSet = std::set<nbautils::state_t>;

    // First merge check.
    CHECK(rep1 == 0);
    CheckEquivalentAutomata(merge1, swa);

    // Second merge check.
    CHECK(rep2 == 0);
    CHECK(VectorToSet(merge2.states()) == StateSet {0, 2, 3, 4});
    CHECK(VectorToSet(merge2.get_init()) == StateSet {0});

    CHECK(VectorToSet(merge2.succ(0, 0)) == StateSet {0});
    CHECK(VectorToSet(merge2.succ(0, 1)) == StateSet {2, 4});
    CHECK(VectorToSet(merge2.succ(2, 0)) == StateSet {3});
    CHECK(VectorToSet(merge2.succ(2, 1)) == StateSet {3});
    CHECK(VectorToSet(merge2.succ(3, 0)) == StateSet {2});
    CHECK(VectorToSet(merge2.succ(3, 1)) == StateSet {4});
    CHECK(VectorToSet(merge2.succ(4, 0)) == StateSet {3});
    CHECK(VectorToSet(merge2.succ(4, 1)) == StateSet {3});

    CHECK(merge2.get_accs(0) == std::vector<nbautils::acc_t> {});
    CHECK(merge2.get_accs(2) == std::vector<nbautils::acc_t> {});
    CHECK(merge2.get_accs(3) == std::vector<nbautils::acc_t> {});
    CHECK(merge2.get_accs(4) == std::vector<nbautils::acc_t> {0});

    // Third merge check.
    CHECK(rep3 == 2);
    CHECK(VectorToSet(merge3.states()) == StateSet {0, 1, 2});
    CHECK(VectorToSet(merge3.get_init()) == StateSet {0});

    CHECK(VectorToSet(merge3.succ(0, 0)) == StateSet {1});
    CHECK(VectorToSet(merge3.succ(0, 1)) == StateSet {2});
    CHECK(VectorToSet(merge3.succ(1, 0)) == StateSet {0});
    CHECK(VectorToSet(merge3.succ(1, 1)) == StateSet {2});
    CHECK(VectorToSet(merge3.succ(2, 0)) == StateSet {2});
    CHECK(VectorToSet(merge3.succ(2, 1)) == StateSet {2});

    CHECK(merge3.get_accs(0) == std::vector<nbautils::acc_t> {});
    CHECK(merge3.get_accs(1) == std::vector<nbautils::acc_t> {});
    CHECK(merge3.get_accs(2) == std::vector<nbautils::acc_t> {0});
}
