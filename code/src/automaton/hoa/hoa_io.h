
#pragma once

#include <iostream>
#include "../parity.h"

namespace tollk {
namespace automaton {
namespace hoa {

// Reads an automaton stored in HOA format from a given input stream. Note that potentially not all HOA features
// are supported, in which case an exception should be thrown.
DPA DPAFromHOA(std::istream* istream);
NPA NPAFromHOA(std::istream* istream);


// Writes a given automaton to a output stream.
template <typename AutomatonT>
void ToHOA(const AutomatonT& automaton, std::ostream* ostream);

template <typename AutomatonT>
std::string ToHOAStr(const AutomatonT& automaton) {
    std::stringstream ss;
    ToHOA(automaton, &ss);
    return ss.str();
}

#include "hoa_io.impl.h"


}
}
}
