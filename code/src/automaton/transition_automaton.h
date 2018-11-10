
#pragma once

#include "finite_automaton.h"
#include <set>
#include <range/v3/view/bounded.hpp>
#include <range/v3/view/iota.hpp>
#include <range/v3/view/set_algorithm.hpp>
#include "is_specialization_base_of.h"

namespace tollk {
namespace automaton {

// Read symbols are represented by this type.
using symbol_t = unsigned char;

/*
 * Abstract extension for the base automaton class which provides basic functionality for transitions.
 */
template<typename SuccRangeT1, typename SuccRangeT2>
class TransitionAutomaton : public virtual FiniteAutomaton {
 public:
    // Iterator type used for successors.
    using SuccRangeStateSym = SuccRangeT1;
    using SuccRangeState = SuccRangeT2;

    // Constructs a new det. automaton with a given number of atomic propositions 'ap'.
    // The number of symbols will be m = 2^ap. Note that it must hold that ap < sizeof(symbol_t).
    explicit TransitionAutomaton(unsigned char atomic_propositions) : atomicPropositions(atomic_propositions) {}

    // Return a range describing a range of successors of a given state and symbol.
    virtual SuccRangeStateSym Successors(state_t q, symbol_t s) const = 0;

    // Return a range describing a range of successors of a given state.
    virtual SuccRangeState Successors(state_t q) const = 0;

    // Redirects all transitions from one state to another. O(n * m) operation.
    virtual void RedirectTransitions(state_t previous_target, state_t new_target) = 0;

    // Return m, which is the number of different symbols used by this automaton. O(1) operation.
    symbol_t SymbolsNum() const {
        return static_cast<symbol_t>(1u) << this->atomicPropositions;
    }

    // Returns a range of all valid symbols, i.e. 0 to SymbolsNum()-1.
    auto Symbols() const {
        return ranges::v3::view::ints(symbol_t(0), SymbolsNum());
    }

    // Returns whether every state-symbol combination has exactly one successor.
    virtual bool IsDeterministic() const;

    const unsigned char atomicPropositions;
};

// Compares if two automata are equal, i.e. they contain the same states and same transitions.
// Note that these operations are quite expensive.
template <typename RT1, typename RT2, typename RT3, typename RT4>
bool operator==(const TransitionAutomaton<RT1, RT2>& lhs, const TransitionAutomaton<RT3, RT4>& rhs);
template <typename RT1, typename RT2, typename RT3, typename RT4>
bool operator!=(const TransitionAutomaton<RT1, RT2>& lhs, const TransitionAutomaton<RT3, RT4>& rhs);


template <typename AutomatonT>
constexpr bool IsTransitionAutomaton() {
    return is_specialization_base_of<TransitionAutomaton, AutomatonT>::value;
}



// Implementation

template <typename RT1, typename RT2>
bool TransitionAutomaton<RT1, RT2>::IsDeterministic() const {
    for (state_t q : States()) {
        for (symbol_t s : Symbols()) {
            if (ranges::v3::distance(Successors(q, s)) != 1) {
                return false;
            }
        }
    }
    return true;
}


template <typename RT1, typename RT2, typename RT3, typename RT4>
bool operator==(const TransitionAutomaton<RT1, RT2>& lhs, const TransitionAutomaton<RT3, RT4>& rhs) {
    if (lhs.InitialState() != rhs.InitialState() || lhs.atomicPropositions != rhs.atomicPropositions)
        return false;

    const std::set<state_t> lhs_states_set(lhs.States().begin(), lhs.States().end());
    const std::set<state_t> rhs_states_set(rhs.States().begin(), rhs.States().end());
    if (lhs_states_set != rhs_states_set)
        return false;

    for (state_t q : lhs.States()) {
        for (symbol_t s : lhs.Symbols()) {
            const auto lhs_out = ranges::v3::view::bounded(lhs.Successors(q, s));
            const auto rhs_out = ranges::v3::view::bounded(rhs.Successors(q, s));
            const std::set<state_t> lhs_out_set(lhs_out.begin(), lhs_out.end());
            const std::set<state_t> rhs_out_set(rhs_out.begin(), rhs_out.end());
            const auto successor_diff = ranges::v3::view::set_symmetric_difference(lhs_out_set, rhs_out_set);
            if (!ranges::v3::empty(successor_diff))
                return false;
        }
    }

    return true;
}

template <typename RT1, typename RT2, typename RT3, typename RT4>
bool operator!=(const TransitionAutomaton<RT1, RT2>& lhs, const TransitionAutomaton<RT3, RT4>& rhs) {
    return !(lhs == rhs);
}


}  // namespace automaton
}  // namespace tollk