
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../deterministic_automaton.h"
#include "../nondeterministic_automaton.h"

// main file to execute all tests

using namespace tollk::automaton;

NondeterministicAutomaton TestAutomaton1() {
    NondeterministicAutomaton automaton(1);
    automaton.AddState(0);
    automaton.AddState(1);
    automaton.AddState(2);
    automaton.AddState(3);
    automaton.SetInitialState(0);
    automaton.AddSucc(0, 0, 0);
    automaton.AddSucc(0, 0, 1);
    automaton.AddSucc(1, 0, 2);
    automaton.AddSucc(1, 1, 2);
    automaton.AddSucc(2, 0, 1);
    automaton.AddSucc(2, 1, 3);
    automaton.AddSucc(3, 0, 3);
    return automaton;
}

DeterministicAutomaton TestAutomaton2() {
    DeterministicAutomaton automaton(1);
    automaton.AddState(0);
    automaton.AddState(1);
    automaton.AddState(2);
    automaton.AddState(3);
    automaton.SetInitialState(0);
    automaton.SetSucc(0, 0, 0);
    automaton.SetSucc(0, 0, 1);
    automaton.SetSucc(1, 0, 2);
    automaton.SetSucc(1, 0, 3);
    automaton.SetSucc(2, 0, 0);
    automaton.SetSucc(2, 0, 1);
    automaton.SetSucc(3, 0, 3);
    automaton.SetSucc(3, 0, 3);
    return automaton;
}

DeterministicAutomaton TestAutomaton3() {
    DeterministicAutomaton automaton(1);
    automaton.AddState(0);
    automaton.AddState(1);
    automaton.AddState(2);
    automaton.AddState(3);
    automaton.AddState(4);
    automaton.SetInitialState(0);
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

DeterministicAutomaton TestAutomaton4() {
    DeterministicAutomaton automaton(1);
    automaton.AddState(2);
    automaton.AddState(1);
    automaton.SetInitialState(1);
    automaton.SetSucc(2, 0, 2);
    automaton.SetSucc(2, 1, 1);
    automaton.SetSucc(1, 0, 2);
    automaton.SetSucc(1, 1, 1);
    return automaton;
}