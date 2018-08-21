
#include <catch.hpp>
#include "../iterated_moore.h"

using namespace tollk;
using namespace automaton;

TEST_CASE("Test IteratedMooreRelation.") {
    DPA dpa(1);
    dpa.AddState(0);
    dpa.AddState(1);
    dpa.AddState(2);
    dpa.SetInitialState(0);
    dpa.SetSucc(0, 0, 0);
    dpa.SetSucc(0, 1, 1);
    dpa.SetSucc(1, 0, 2);
    dpa.SetSucc(1, 1, 2);
    dpa.SetSucc(2, 0, 2);
    dpa.SetSucc(2, 1, 2);
    dpa.SetLabel(0, 1);
    dpa.SetLabel(1, 0);
    dpa.SetLabel(2, 1);

    const EquivalenceRelation<state_t> im_relation = IteratedMooreRelation(dpa);
    //TODO
}