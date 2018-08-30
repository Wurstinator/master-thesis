

#include "catch.hpp"
#include "../hoa/hoa_io.h"

using namespace tollk;
using namespace automaton;

DPA TestAutomaton5();

TEST_CASE("Test FromHOADPA.") {
    const std::string hoa = R""(HOA: v1
States: 5
Start: 1
AP: 1 "a1"
acc-name: parity min even 4
Acceptance: 4 Inf(0) | (Fin(1) & (Inf(2) | Fin(3)))
properties: trans-labels explicit-labels state-acc colored complete
properties: deterministic
--BODY--
State: 0 {1}
[t] 4
State: 1 {2}
[0] 3
[!0] 4
State: 2 {1}
[t] 3
State: 3 {2}
[0] 2
[!0] 1
State: 4 {3}
[0] 2
[!0] 0
--END--
)"";
    std::stringstream ss(hoa);
    DPA dpa = hoa::DPAFromHOA(&ss);

    CHECK(dpa.States().size() == 5);
    CHECK(dpa.atomicPropositions == 1);
    CHECK(dpa.InitialState() == 1);
    CHECK(dpa.HasState(0));
    CHECK(dpa.HasState(1));
    CHECK(dpa.HasState(2));
    CHECK(dpa.HasState(3));
    CHECK(dpa.HasState(4));
    CHECK(dpa.Succ(0, 0) == 4);
    CHECK(dpa.Succ(0, 1) == 4);
    CHECK(dpa.Succ(1, 0) == 4);
    CHECK(dpa.Succ(1, 1) == 3);
    CHECK(dpa.Succ(2, 0) == 3);
    CHECK(dpa.Succ(2, 1) == 3);
    CHECK(dpa.Succ(3, 0) == 1);
    CHECK(dpa.Succ(3, 1) == 2);
    CHECK(dpa.Succ(4, 0) == 0);
    CHECK(dpa.Succ(4, 1) == 2);
}


TEST_CASE("Test ToHOAStr.") {
    const DPA automaton = TestAutomaton5();
    const std::string hoa = hoa::ToHOAStr(automaton);
    std::stringstream ss(hoa);
    const DPA decoded = hoa::DPAFromHOA(&ss);

    CHECK(automaton.States() == decoded.States());
    CHECK(automaton.InitialState() == decoded.InitialState());
    for (state_t p : automaton.States())
        for (symbol_t s : automaton.Symbols())
            CHECK(automaton.Succ(p, s) == decoded.Succ(p, s));
    for (state_t p : automaton.States())
        CHECK(automaton.GetLabel(p) == decoded.GetLabel(p));
}