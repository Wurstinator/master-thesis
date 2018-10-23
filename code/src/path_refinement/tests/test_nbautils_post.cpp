
#include "catch.hpp"
#include "../nbautils_post.h"

TEST_CASE("Test NbautilsEquivalence.") {
    std::map<tollk::automaton::state_t, std::string> labels;
    labels[0] = "{1}";
    labels[2] = "{2, 3}";
    labels[3] = "{2, 3, 9}";
    labels[4] = "{9, 2, 3}";
    labels[5] = "{2, 3, 12, 9}";
    labels[6] = "foo {2, 3}, bar {}, 12, baz {9}";
    const tollk::EquivalenceRelation<tollk::automaton::state_t> relation = tollk::NbautilsEquivalence(labels);
    CHECK(!relation.IsEquiv(0, 2));
    CHECK(!relation.IsEquiv(0, 3));
    CHECK(!relation.IsEquiv(0, 4));
    CHECK(!relation.IsEquiv(0, 5));
    CHECK(!relation.IsEquiv(0, 6));
    CHECK(!relation.IsEquiv(2, 3));
    CHECK(!relation.IsEquiv(2, 4));
    CHECK(!relation.IsEquiv(2, 5));
    CHECK(!relation.IsEquiv(2, 6));
    CHECK(relation.IsEquiv(3, 4));
    CHECK(!relation.IsEquiv(3, 5));
    CHECK(relation.IsEquiv(3, 6));
    CHECK(!relation.IsEquiv(4, 5));
    CHECK(relation.IsEquiv(4, 6));
    CHECK(!relation.IsEquiv(5, 6));
}
