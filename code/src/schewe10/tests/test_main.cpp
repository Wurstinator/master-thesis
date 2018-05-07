
#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include "../../automaton/parity.h"

// main file to execute all tests

tollk::automaton::DPA sven_example() {
    tollk::automaton::DPA automaton(1);
    automaton.AddState(0);
    automaton.AddState(1);
    automaton.AddState(2);
    automaton.AddState(3);
    automaton.AddState(4);
    automaton.SetInitialState(0);
    automaton.SetLabel(0, 1);
    automaton.SetLabel(1, 1);
    automaton.SetLabel(2, 1);
    automaton.SetLabel(3, 1);
    automaton.SetLabel(4, 0);
    automaton.SetSucc(0, 0, 1);
    automaton.SetSucc(0, 1, 2);
    automaton.SetSucc(1, 0, 0);
    automaton.SetSucc(1, 1, 4);
    automaton.SetSucc(2, 0, 3);
    automaton.SetSucc(2, 1, 3);
    automaton.SetSucc(3, 0, 2);
    automaton.SetSucc(3, 1, 4);
    automaton.SetSucc(4, 0, 3);
    automaton.SetSucc(4, 1, 3);
    return automaton;
}