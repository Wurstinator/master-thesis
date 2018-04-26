
#pragma once

#include <range/v3/view/iota.hpp>
#include "finite_automaton.h"

namespace tollk {
namespace automaton {

// Read symbols are represented by this type.
using symbol_t = unsigned char;

/*
 * Abstract extension for the base automaton class which provides basic functionality for transitions.
 */
template<typename SuccRangeT1, typename SuccRangeT2>
class TransitionAutomaton : public FiniteAutomaton {
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

    const unsigned char atomicPropositions;
};

}  // namespace automaton
}  // namespace tollk