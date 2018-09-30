
// Tests on some automata that the two ways to compute PR yield the same result.

#include <catch.hpp>

#include "../../automaton/parity.h"
#include "../../automaton/parity_util.h"
#include "../path_refinement.h"
#include "../visit_graph.h"

using namespace tollk;
using namespace automaton;

DPA TestAutomaton6();
EquivalenceRelation<state_t> TestAutomaton6_LangEquiv();


TEST_CASE("Test NondeterministicAutomaton::FromTransitionAutomaton.") {
    const DPA automaton = TestAutomaton6();
    const EquivalenceRelation<state_t> language_equivalence = TestAutomaton6_LangEquiv();

    for (const EquivalenceRelation<state_t>::EquivClass& lambda : language_equivalence.Classes()) {
        const EquivalenceRelation<state_t> game_result = PathRefinementEquivalence(automaton, lambda);
        const EquivalenceRelation<state_t> vis_result = PathRefinementEquivalence_VIS(automaton, lambda);
        CHECK(game_result == vis_result);
    }
}
