
#pragma once

#include "cpphoafparser/consumer/hoa_consumer_null.hh"
#include "../parity.h"

namespace tollk {
namespace automaton {
namespace hoa {

// Abstract base class to consume a HOA file from an input stream and convert it to an automaton.
class PAConsumer : public cpphoafparser::HOAConsumerNull {
 public:
    void addStartStates(const int_list& stateConjunction) override;
    void setAPs(const std::vector<std::string>& aps) override;
    void setAcceptanceCondition(unsigned int numberOfSets, acceptance_expr::ptr accExpr) override;
    void provideAcceptanceName(const std::string& name, const std::vector<cpphoafparser::IntOrString>& extraInfo) override;
    void notifyBodyStart() override;
    void addProperties(const std::vector<std::string>& properties) override;
    void addState(unsigned int id, std::shared_ptr<std::string> info, label_expr::ptr labelExpr,
                  std::shared_ptr<int_list> accSignature) override;
    void addEdgeWithLabel(unsigned int stateId, label_expr::ptr labelExpr, const int_list& conjSuccessors,
                          std::shared_ptr<int_list> accSignature) override;
    void notifyEnd() override;


    // If one initial state is provided, updates the internal automaton to match that. If multiple are given,
    // introduces a new initial state to mimic their behaviour.
    virtual void FixInitialStates(const std::vector<unsigned int>& states) = 0;
    // Adds a labeled edge to the automaton (and removes the old one, for DPA).
    virtual void AddTransition(state_t from, symbol_t sym, state_t to) = 0;

    virtual const std::map<state_t, std::string>& GetStateLabels() const;

 protected:
    virtual ParityAutomaton* _get_automaton() = 0;

    std::map<state_t, std::string> state_labels;

    unsigned long aps;
    std::vector<unsigned int> initial_states;
    bool is_parity_mineven = false;
    bool is_deterministic = false;
};


}
}
}
