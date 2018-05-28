
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

    // These states are triples (p, q, k). For each pair (p, q), collect the lowest value k such that (p, q, k) has
    // an empty language.
    std::map<std::pair<automaton::state_t, automaton::state_t>, std::optional<automaton::parity_label_t>> minimal_obligations;
    for (automaton::state_t q : empty_lang_states) {
        const DelayedSimulationAutomatonState& desim_state = state_indices.left.at(q);
        const std::pair<automaton::state_t, automaton::state_t> pq_pair(desim_state.p, desim_state.q);
        const std::optional<automaton::parity_label_t>& obligation = desim_state.obligation;

        // Check if the new obligation is the smallest one so far and update the map if so.
        auto iter = minimal_obligations.find(pq_pair);
        if (iter == minimal_obligations.end()) {
            minimal_obligations.insert(std::make_pair(pq_pair, obligation));
        } else if (lt_checkmark(obligation, iter->second)) {
            iter->second = obligation;
        }
    }

    // Extract those pairs (p, q) in which the lowest value is at most Gamma(c(p), c(q), check).
    std::set<std::pair<automaton::state_t, automaton::state_t>> desim_pairs;
    for (const std::pair<std::pair<automaton::state_t, automaton::state_t>, std::optional<automaton::parity_label_t>>& kv_pair : minimal_obligations) {
        const std::optional<automaton::parity_label_t>& minimal_obligation = kv_pair.second;
        const std::optional<automaton::parity_label_t> required_obligation = Gamma_de(dpa.GetLabel(kv_pair.first.first), dpa.GetLabel(kv_pair.first.second), std::nullopt);
        if (lte_checkmark(minimal_obligation, required_obligation))
            desim_pairs.insert(kv_pair.first);
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