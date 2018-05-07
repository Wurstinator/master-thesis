
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

TEST_CASE("Test PriorityAlmostEquivalence.") {
    tollk::automaton::DPA dpa(1);
    dpa.AddState(0);
    dpa.AddState(1);
    dpa.AddState(2);
    dpa.SetInitialState(0);
    dpa.SetSucc(0, 0, 2);
    dpa.SetSucc(0, 1, 1);
    dpa.SetSucc(1, 0, 2);
    dpa.SetSucc(1, 1, 1);
    dpa.SetSucc(2, 0, 2);
    dpa.SetSucc(2, 1, 2);
    dpa.SetLabel(0, 2);
    dpa.SetLabel(1, 2);
    dpa.SetLabel(2, 1);
    const tollk::EquivalenceRelation<tollk::automaton::state_t> relation = tollk::PriorityAlmostEquivalence(dpa);
    REQUIRE(relation.Classes().size() == 2);
    CHECK(relation.IsEquiv(0, 0));
    CHECK(relation.IsEquiv(0, 1));
    CHECK(!relation.IsEquiv(0, 2));
    CHECK(relation.IsEquiv(1, 0));
    CHECK(relation.IsEquiv(1, 1));
    CHECK(!relation.IsEquiv(1, 2));
    CHECK(!relation.IsEquiv(2, 0));
    CHECK(!relation.IsEquiv(2, 1));
    CHECK(relation.IsEquiv(2, 2));
}