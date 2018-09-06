
#include "parity.h"


namespace tollk {
namespace automaton {

bool ParityIsBuchi(const ParityAutomaton& automaton) {
    for (state_t q : automaton.States())
        if (automaton.GetLabel(q) > 1)
            return false;
    return true;
}



}
}