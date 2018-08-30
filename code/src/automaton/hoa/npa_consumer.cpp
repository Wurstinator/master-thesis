
#include "npa_consumer.h"
#include "boolean_evaluation.h"

namespace tollk {
namespace automaton {
namespace hoa {


void NPAConsumer::notifyBodyStart() {
    PAConsumer::notifyBodyStart();
    this->automaton = std::make_unique<NPA>(this->aps);
}

void NPAConsumer::FixInitialStates(const std::vector<unsigned int>& states) {
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

void NPAConsumer::AddTransition(state_t from, symbol_t sym, state_t to)  {
    this->automaton->AddSucc(from, sym, to);
}

const tollk::automaton::NPA& NPAConsumer::GetAutomaton() const {
    return *automaton;
}

ParityAutomaton* NPAConsumer::_get_automaton() {
    return automaton.get();
}


}
}
}