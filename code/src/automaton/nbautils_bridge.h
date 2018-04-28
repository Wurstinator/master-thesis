
#pragma once

#include "swa.hh"
#include "parity.h"

namespace tollk {
namespace automaton {


template <typename T,template <typename... Args> class S>
NPA FromNbautils(const nbautils::SWA<T, S>& swa) {
    //TODO
};

template <typename RT1, typename RT2>
nbautils::SWA<void> ToNbautils(const TransitionAutomaton<RT1, RT2>& automaton) {
    //TODO
}


}
}