
#include "delayed_simulation.h"
#include "delayed_simulation_automaton.h"
#include "../automaton/parity.h"
#include "../automaton/util.h"
#include "gamma.h"


namespace tollk {



EquivalenceRelation<automaton::state_t> DelayedSimulationEquivalence(const automaton::DPA& dpa) {
    // Create the delayed simulation Büchi automaton.
    boost::bimap<automaton::state_t, DelayedSimulationAutomatonState> state_indices;
    automaton::DPA delayed_simulation_automaton = DelayedSimulationAutomaton(dpa, &Gamma_de, &state_indices);

    // Invert the final states.
    for (automaton::state_t q : delayed_simulation_automaton.States())
        delayed_simulation_automaton.SetLabel(q, 1 - delayed_simulation_automaton.GetLabel(q));

    // Check which states have an empty language.
    const std::unordered_set<automaton::state_t> empty_lang_states = automaton::BuchiEmptyStates(delayed_simulation_automaton);

    // Extract from those states the pairs in which the third component is a checkmark (no obligation to fulfill).
    std::set<std::pair<automaton::state_t, automaton::state_t>> desim_pairs;
    for (automaton::state_t q : empty_lang_states) {
        const DelayedSimulationAutomatonState& desim_state = state_indices.left.at(q);
        if (!desim_state.obligation)
            desim_pairs.insert(std::make_pair(desim_state.p, desim_state.q));
    }

    // Collect equivalent state pairs.
    EquivalenceRelation<automaton::state_t> relation;
    for (const std::pair<automaton::state_t, automaton::state_t>& pair : desim_pairs) {
        const std::pair<automaton::state_t, automaton::state_t> inv_pair(pair.second, pair.first);
        if (desim_pairs.find(inv_pair) != desim_pairs.end())
            relation.AddConnection(pair.first, pair.second);
    }
    return relation;
}







}