
#pragma once

#include "pa_consumer.h"

namespace tollk {
namespace automaton {
namespace hoa {


// Consume a HOA file from an input stream and convert it to a DPA.
class DPAConsumer : public PAConsumer {
 public:
    void notifyBodyStart() override;
    void FixInitialStates(const std::vector<unsigned int>& states) override;
    void AddTransition(state_t from, symbol_t sym, state_t to) override;

    // Returns the automaton to the outside caller.
    const DPA& GetAutomaton() const;

 protected:
    ParityAutomaton* _get_automaton() override;
    std::unique_ptr<DPA> automaton;
};




}
}
}