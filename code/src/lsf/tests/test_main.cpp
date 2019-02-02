
#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include "../../automaton/parity.h"

// main file to execute all tests

using namespace tollk;
using namespace automaton;

DPA example1() {
    DPA automaton(2);
    automaton.AddState(0);
    automaton.AddState(1);
    automaton.AddState(2);
    automaton.AddState(3);
    automaton.AddState(4);
    automaton.SetInitialState(0);
    automaton.SetLabel(0, 0);
    automaton.SetLabel(1, 1);
    automaton.SetLabel(2, 2);
    automaton.SetLabel(3, 1);
    automaton.SetLabel(4, 2);
    automaton.SetSucc(0, 0, 3);
    automaton.SetSucc(0, 1, 2);
    automaton.SetSucc(0, 2, 0);
    automaton.SetSucc(0, 3, 0);
    automaton.SetSucc(1, 0, 2);
    automaton.SetSucc(1, 1, 2);
    automaton.SetSucc(1, 2, 0);
    automaton.SetSucc(1, 3, 0);
    automaton.SetSucc(2, 0, 1);
    automaton.SetSucc(2, 1, 4);
    automaton.SetSucc(2, 2, 0);
    automaton.SetSucc(2, 3, 0);
    automaton.SetSucc(3, 0, 3);
    automaton.SetSucc(3, 1, 4);
    automaton.SetSucc(3, 2, 0);
    automaton.SetSucc(3, 3, 0);
    automaton.SetSucc(4, 0, 3);
    automaton.SetSucc(4, 1, 4);
    automaton.SetSucc(4, 2, 0);
    automaton.SetSucc(4, 3, 0);
    return automaton;
}
