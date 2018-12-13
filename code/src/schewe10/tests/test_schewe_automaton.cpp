
#include "catch.hpp"
#include "../../skipper/skipper_automaton.h"
#include "../../automaton/parity.h"
#include "../../automaton/util.h"

tollk::automaton::DPA sven_example();

TEST_CASE("Compute the Schewe automaton of an example DPA provided by Sven Schewe.") {
    tollk::automaton::DPA dpa = sven_example();
    const tollk::automaton::DPA dpa_copy = dpa;
    tollk::ScheweAutomaton(&dpa);

    REQUIRE(dpa.States() == dpa_copy.States());
    CHECK(dpa.Succ(0, 0) == dpa_copy.Succ(0, 0));
    CHECK(dpa.Succ(1, 0) == dpa_copy.Succ(1, 0));
    CHECK(dpa.Succ(2, 0) == dpa_copy.Succ(2, 0));
    CHECK(dpa.Succ(2, 1) == dpa_copy.Succ(2, 1));
    CHECK(dpa.Succ(3, 0) == dpa_copy.Succ(3, 0));
    CHECK(dpa.Succ(3, 1) == dpa_copy.Succ(3, 1));
    CHECK(dpa.Succ(4, 0) == dpa_copy.Succ(4, 0));
    CHECK(dpa.Succ(4, 1) == dpa_copy.Succ(4, 1));

    CHECK(dpa.Succ(0, 1) == dpa.Succ(1, 1));
    CHECK((dpa.Succ(0, 1) == 2 || dpa.Succ(0, 1) == 4));
}


TEST_CASE("Compute the minimized Schewe automaton of an example DPA provided by Sven Schewe.") {
    tollk::automaton::DPA dpa = sven_example();
    const tollk::automaton::DPA dpa_copy = dpa;
    tollk::ScheweAutomaton(&dpa);
    tollk::automaton::Hopcroft(&dpa);

    CHECK(dpa.States().size() == 4);
}
