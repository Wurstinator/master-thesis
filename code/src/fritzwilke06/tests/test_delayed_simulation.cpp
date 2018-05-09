
#include <catch.hpp>
#include "../delayed_simulation.h"

using namespace tollk;
using namespace automaton;

TEST_CASE("Test DelayedSimulationEquivalence.") {
    DPA dpa(1);
    dpa.AddState(0);
    dpa.AddState(1);
    dpa.AddState(2);
    dpa.SetInitialState(0);
    dpa.SetSucc(0, 0, 2);
    dpa.SetSucc(0, 1, 1);
    dpa.SetSucc(1, 0, 0);
    dpa.SetSucc(1, 1, 1);
    dpa.SetSucc(2, 0, 2);
    dpa.SetSucc(2, 1, 1);
    dpa.SetLabel(0, 1);
    dpa.SetLabel(1, 2);
    dpa.SetLabel(2, 1);

    const EquivalenceRelation<state_t> desim_relation = DelayedSimulationEquivalence(dpa);

    CHECK(desim_relation.Classes().size() == 2);
    CHECK(desim_relation.IsEquiv(0, 0));
    CHECK(!desim_relation.IsEquiv(0, 1));
    CHECK(desim_relation.IsEquiv(0, 2));
    CHECK(!desim_relation.IsEquiv(1, 0));
    CHECK(desim_relation.IsEquiv(1, 1));
    CHECK(!desim_relation.IsEquiv(1, 2));
    CHECK(desim_relation.IsEquiv(2, 0));
    CHECK(!desim_relation.IsEquiv(2, 1));
    CHECK(desim_relation.IsEquiv(2, 2));
}