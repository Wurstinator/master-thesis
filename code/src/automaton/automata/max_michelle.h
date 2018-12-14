
#pragma once

#include "../parity.h"

namespace tollk {
namespace automaton {

/**
 * Computes a "Max Michelle" deterministic Büchi automaton with parameter [n].
 * It is defined with state set $Q = \{ q_0, 0, 1, \dots, n \}$ and $F = \{ 0 \}$.
 * The alphabet is $\Sigma = \{0, ¬0, 1, ¬1, \dots, n, ¬n\}$.
 * Transitions are from q_0 to and state i with symbol i, and back.
 *                 from state i>0 to itself with any symbol but ¬i.
 * The symbols of the internal automaton structure are [s, b0, ..., bm], where s denotes the sign (0 or ¬0) and
 * [b0 ... bm] are a binary representation of the number. All numbers between n and 2^m redirect to a sink state.
 * All transitions that were not explicitly defined also lead to a sink state.
 * @param n The parameter definining the number of states and size of the alphabet.
 * @return A Max Michelle automaton as a DPA.
 */
DPA MaxMichelleAutomaton(unsigned int n);

}
}