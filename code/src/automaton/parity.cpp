
#include "parity.h"


namespace tollk {
namespace automaton {


bool ParityAutomaton::IsBuchi() const {
    for (const std::pair<const state_t, Label>& kv_pair : this->labels)
        if (kv_pair.second > 1)
            return false;
    return true;
}



}
}