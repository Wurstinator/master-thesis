
#include "path_refinement.h"
#include "path_refinement_automaton.h"
#include "../automaton/util.h"

namespace tollk {

using namespace automaton;

EquivalenceRelation<state_t> PathRefinementEquivalence(const DPA& dpa, const EquivalenceRelation<state_t>::EquivClass& lambda) {
    // Collect a set of all "initial states" that the PR automaton can have.
    std::set<PathRefinementAutomatonState> initial_states;
    for (state_t p : dpa.States())
        for (state_t q : dpa.States())
            initial_states.insert(PathRefinementInitialState(dpa, p, q));

    // Find all universal states in the lambda-PR automaton.
    boost::bimap<state_t, PathRefinementAutomatonState> state_indices;
    const DeterministicAutomaton pr_automaton = PathRefinementAutomaton(dpa, lambda, &state_indices);
    const std::unordered_set<state_t> universal_states = NotReachingStates(pr_automaton, ranges::v3::view::single(0));

    // Build the interesection of initial states and universal states in lambda.
    EquivalenceRelation<state_t> result;
    for (state_t coded_universal : universal_states) {
        PathRefinementAutomatonState decoded_universal = state_indices.left.at(coded_universal);
        const state_t p = decoded_universal.p;
        const state_t q = decoded_universal.q;
        if (lambda.find(p) != lambda.end()
            && lambda.find(q) != lambda.end()
            && initial_states.find(decoded_universal) != initial_states.end())
            result.AddConnection(p, q);
    }

    return result;
}


void PathRefinementMerge(automaton::DPA* dpa, const EquivalenceRelation<automaton::state_t>::EquivClass& lambda) {
    const EquivalenceRelation<automaton::state_t> pr = PathRefinementEquivalence(*dpa, lambda);
    std::function<automaton::parity_label_t(const EquivalenceRelation<automaton::state_t>::EquivClass&)> min_priority_selector =
        [dpa](const EquivalenceRelation<automaton::state_t>::EquivClass& kappa) {
        const auto compare_prio = [&dpa](automaton::state_t p, automaton::state_t q) {return dpa->GetLabel(p) < dpa->GetLabel(q);};
        const automaton::state_t min_state = *std::min_element(kappa.begin(), kappa.end(), compare_prio);
        return dpa->GetLabel(min_state);
    };
    QuotientAutomatonLabelled(dpa, pr, min_priority_selector);
}


}