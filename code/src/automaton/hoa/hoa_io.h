
#pragma once

#include <iostream>
#include "../parity.h"

namespace tollk {
namespace automaton {
namespace hoa {

// Reads an automaton stored in HOA format from a given input stream. Note that potentially not all HOA features
// are supported, in which case an exception should be thrown.
DPA DPAFromHOA(std::istream* istream, std::map<state_t, std::string>* state_labels = nullptr);
NPA NPAFromHOA(std::istream* istream, std::map<state_t, std::string>* state_labels = nullptr);
DPA TransitionBasedDPAFromHOA(std::istream* istream, std::map<std::pair<state_t, parity_label_t>, state_t>* pair_indices = nullptr, std::map<state_t, std::string>* state_labels = nullptr);


// Writes a given automaton to a output stream. Optionally, state labels can be written as well.
template <typename AutomatonT>
void ToHOA(const AutomatonT& automaton, std::ostream* ostream);
template <typename AutomatonT>
void ToHOA(const AutomatonT& automaton, std::ostream* ostream, const std::map<state_t, std::string>& state_labels);

template <typename AutomatonT>
std::string ToHOAStr(const AutomatonT& automaton) {
    std::stringstream ss;
    ToHOA(automaton, &ss);
    return ss.str();
}


// Writes a given automaton to C++ code.
template <typename AutomatonT>
std::string ToCPPCode(const AutomatonT& automaton);

#include "hoa_io.impl.h"


}
}
}
