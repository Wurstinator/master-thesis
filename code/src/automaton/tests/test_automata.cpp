
#include "catch.hpp"

#include "../finite_automaton.h"
#include "../transition_automaton.h"
#include "../labelled_automaton.h"
#include "../deterministic_automaton.h"
#include "../nondeterministic_automaton.h"

TEST_CASE("foo") {
    tollk::automaton::DeterministicAutomaton x(1);
    tollk::automaton::NondeterministicAutomaton y(1);
}
