
#include "dpa_consumer.h"
#include "boolean_evaluation.h"

namespace tollk {
namespace automaton {
namespace hoa {


void DPAConsumer::notifyBodyStart() {
    PAConsumer::notifyBodyStart();
    if (!this->is_deterministic)
        throw std::logic_error("HOA does not contain the 'deterministic' tag.");
    this->automaton = std::make_unique<DPA>(this->aps);
}

void DPAConsumer::FixInitialStates(const std::vector<unsigned int>& states) {
    if (states.size() == 0)
        return;
    if (states.size() == 1) {
        this->automaton->SetInitialState(states.front());
        return;
    }

    // Add a new initial state.
    state_t init = 0;
    while (this->automaton->HasState(init))
        init++;
    this->automaton->AddState(init);
    for (state_t p : states) {
        for (symbol_t s : this->automaton->Symbols()) {
            for (state_t q : this->automaton->Successors(p, s)) {
                AddTransition(init, s, q);
            }
        }
    }
}

void DPAConsumer::AddTransition(state_t from, symbol_t sym, state_t to)  {
    this->automaton->SetSucc(from, sym, to);
}

const tollk::automaton::DPA& DPAConsumer::GetAutomaton() const {
    return *automaton;
}

ParityAutomaton* DPAConsumer::_get_automaton() {
    return automaton.get();
}

}
}
}