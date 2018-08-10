
#include "parity.h"


namespace tollk {
namespace automaton {


bool ParityAutomaton::IsBuchi() const {
    for (const std::pair<const state_t, Label>& kv_pair : this->labels)
        if (kv_pair.second > 1)
            return false;
    return true;
}


NPA NPA::FromDPA(const DPA& dpa) {
    NPA npa(NondeterministicAutomaton::FromTransitionAutomaton(dpa));
    for (state_t q : npa.States())
        npa.SetLabel(q, dpa.GetLabel(q));
    return npa;
}



DPA DPA::FromNPA(const NPA& npa) {
    DPA dpa = DeterministicAutomaton::FromTransitionAutomaton(npa);
    for (state_t q : dpa.States())
        dpa.SetLabel(q, npa.GetLabel(q));
    return dpa;
}


}
}