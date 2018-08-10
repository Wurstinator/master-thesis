
#include <catch.hpp>
#include "../delayed_simulation_automaton.h"
#include "../gamma.h"

using namespace tollk;
using namespace automaton;

TEST_CASE("Test DelayedSimulationAutomaton.") {
    DPA automaton(0);
    automaton.AddState(0);
    automaton.AddState(1);
    automaton.SetSucc(0, 0, 1);
    automaton.SetSucc(1, 0, 0);
    automaton.SetLabel(0, 0);
    automaton.SetLabel(1, 1);

    boost::bimap<state_t, DelayedSimulationAutomatonState> state_indices;
    const DPA desim_automaton = DelayedSimulationAutomaton(automaton, &Gamma_de, false, &state_indices);

    REQUIRE(desim_automaton.States().size() == 12);

    const state_t q000 = state_indices.right.at(DelayedSimulationAutomatonState {0, 0, 0});
    const state_t q010 = state_indices.right.at(DelayedSimulationAutomatonState {0, 1, 0});
    const state_t q100 = state_indices.right.at(DelayedSimulationAutomatonState {1, 0, 0});
    const state_t q110 = state_indices.right.at(DelayedSimulationAutomatonState {1, 1, 0});
    const state_t q001 = state_indices.right.at(DelayedSimulationAutomatonState {0, 0, 1});
    const state_t q011 = state_indices.right.at(DelayedSimulationAutomatonState {0, 1, 1});
    const state_t q101 = state_indices.right.at(DelayedSimulationAutomatonState {1, 0, 1});
    const state_t q111 = state_indices.right.at(DelayedSimulationAutomatonState {1, 1, 1});
    const state_t q00c = state_indices.right.at(DelayedSimulationAutomatonState {0, 0, std::nullopt});
    const state_t q01c = state_indices.right.at(DelayedSimulationAutomatonState {0, 1, std::nullopt});
    const state_t q10c = state_indices.right.at(DelayedSimulationAutomatonState {1, 0, std::nullopt});
    const state_t q11c = state_indices.right.at(DelayedSimulationAutomatonState {1, 1, std::nullopt});

    CHECK(desim_automaton.GetLabel(q000) == 1);
    CHECK(desim_automaton.GetLabel(q010) == 1);
    CHECK(desim_automaton.GetLabel(q100) == 1);
    CHECK(desim_automaton.GetLabel(q110) == 1);
    CHECK(desim_automaton.GetLabel(q001) == 1);
    CHECK(desim_automaton.GetLabel(q011) == 1);
    CHECK(desim_automaton.GetLabel(q101) == 1);
    CHECK(desim_automaton.GetLabel(q111) == 1);
    CHECK(desim_automaton.GetLabel(q00c) == 0);
    CHECK(desim_automaton.GetLabel(q01c) == 0);
    CHECK(desim_automaton.GetLabel(q10c) == 0);
    CHECK(desim_automaton.GetLabel(q11c) == 0);

    CHECK(desim_automaton.Succ(q000, 0) == q11c);
    CHECK(desim_automaton.Succ(q010, 0) == q100);
    CHECK(desim_automaton.Succ(q100, 0) == q01c);
    CHECK(desim_automaton.Succ(q110, 0) == q000);
    CHECK(desim_automaton.Succ(q001, 0) == q11c);
    CHECK(desim_automaton.Succ(q011, 0) == q100);
    CHECK(desim_automaton.Succ(q101, 0) == q01c);
    CHECK(desim_automaton.Succ(q111, 0) == q00c);
    CHECK(desim_automaton.Succ(q00c, 0) == q11c);
    CHECK(desim_automaton.Succ(q01c, 0) == q100);
    CHECK(desim_automaton.Succ(q10c, 0) == q01c);
    CHECK(desim_automaton.Succ(q11c, 0) == q00c);
}