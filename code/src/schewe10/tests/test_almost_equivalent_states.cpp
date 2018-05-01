
#include "catch.hpp"
#include "swa.hh"
#include "../almost_equivalent_states.h"
#include "../../automaton/parity.h"

tollk::automaton::DPA sven_example();

TEST_CASE("Compute alm.eq. states of an example automaton provided by Sven Schewe") {
    const tollk::EquivalenceRelation<tollk::automaton::state_t> relation = tollk::PriorityAlmostEquivalence(sven_example());
    REQUIRE(relation.Classes().size() == 3);
    CHECK(relation.IsEquiv(0, 0));
    CHECK(relation.IsEquiv(0, 1));
    CHECK(!relation.IsEquiv(0, 2));
    CHECK(!relation.IsEquiv(0, 3));
    CHECK(!relation.IsEquiv(0, 4));
    CHECK(relation.IsEquiv(1, 0));
    CHECK(relation.IsEquiv(1, 1));
    CHECK(!relation.IsEquiv(1, 2));
    CHECK(!relation.IsEquiv(1, 3));
    CHECK(!relation.IsEquiv(1, 4));
    CHECK(!relation.IsEquiv(2, 0));
    CHECK(!relation.IsEquiv(2, 1));
    CHECK(relation.IsEquiv(2, 2));
    CHECK(!relation.IsEquiv(2, 3));
    CHECK(relation.IsEquiv(2, 4));
    CHECK(!relation.IsEquiv(3, 0));
    CHECK(!relation.IsEquiv(3, 1));
    CHECK(!relation.IsEquiv(3, 2));
    CHECK(relation.IsEquiv(3, 3));
    CHECK(!relation.IsEquiv(3, 4));
    CHECK(!relation.IsEquiv(4, 0));
    CHECK(!relation.IsEquiv(4, 1));
    CHECK(relation.IsEquiv(4, 2));
    CHECK(!relation.IsEquiv(4, 3));
    CHECK(relation.IsEquiv(4, 4));
}