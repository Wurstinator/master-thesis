
#pragma once

#include "swa.hh"
#include "parity.h"
#include "is_specialization_base_of.h"

namespace tollk {
namespace automaton {

// Converts a given nbautils automaton to a nondeterministic parity automaton. In general this involves copying all
// states and transitions and potentially switching around the priorities if the acceptance condition is not min-even.
// The only special case is when the SWA has multiple initial states. In that case, a new initial state is introduced
// to the resulting automaton.
template <typename T,template <typename... Args> class S>
NPA FromNbautils(const nbautils::SWA<T, S>& swa);

// Converts a given automaton to a SWA. The passed automaton model should inherit from TransitionAutomaton and/or
// LabelledAutomaton if the construction should be complete. Otherwise the resulting SWA might e.g. have no
// transitions.
template <typename AutomatonT, typename TagT=void*>
nbautils::SWA<TagT> ToNbautils(const AutomatonT& base_automaton) {
    static_assert(std::is_base_of<FiniteAutomaton, AutomatonT>::value);
    constexpr bool is_transition_automaton = is_specialization_base_of<TransitionAutomaton, AutomatonT>::value;
    constexpr bool is_labelled_automaton = std::is_base_of<LabelledAutomaton<parity_label_t>, AutomatonT>::value;
    using transition_t = typename is_specialization_base_of<TransitionAutomaton, AutomatonT>::base_type;
    using labelled_t = LabelledAutomaton<parity_label_t>;

    static_assert(std::is_base_of<LabelledAutomaton<parity_label_t>, AutomatonT>::value);
    static_assert(is_specialization_base_of<TransitionAutomaton, AutomatonT>::value);
    static_assert(std::is_same<transition_t, void>::value);
    static_assert(std::is_same<transition_t, TransitionAutomaton<const std::unordered_set<state_t>&, ranges::v3::any_view<state_t>>>::value);

    // If the given automaton is a transition automaton, get its atomic propositions.
    std::vector<std::string> swa_aps;
    if constexpr (is_transition_automaton) {
        //const transition_t& transition_automaton = base_automaton;
        //for (unsigned int i = 0; i < transition_automaton.atomicPropositions; ++i)
        //    swa_aps.emplace_back('a', i);
    }

    nbautils::SWA<TagT> result(nbautils::Acceptance::PARITY, "", swa_aps, std::vector<nbautils::state_t> {base_automaton.InitialState()});
    result.set_patype(nbautils::PAType::MIN_EVEN);

    // Add states.
    for (state_t q : base_automaton.States()) {
        result.add_state(q);
        if constexpr (is_labelled_automaton) {
            const labelled_t& label_automaton = base_automaton;
            result.set_accs(q, std::vector<nbautils::acc_t>{label_automaton.GetLabel(q)});
        }
    }

    // Add transitions.
    /*if constexpr (is_transition_automaton) {
        const transition_t& transition_automaton = base_automaton;
        for (state_t p : transition_automaton.States()) {
            for (symbol_t s : transition_automaton.Symbols()) {
                const typename transition_t::SuccRangeStateSym successors = transition_automaton.Successors(p, s);
                result.set_succs(p, s, std::vector<nbautils::state_t>(successors.begin(), successors.end()));
            }
        }
    }*/

    return result;
}


}
}





#include "nbautils_bridge.impl.h"