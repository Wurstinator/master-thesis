
#include "pa_consumer.h"
#include "boolean_evaluation.h"

namespace tollk {
namespace automaton {
namespace hoa {



void PAConsumer::addStartStates(const int_list& stateConjunction) {
    this->initial_states = stateConjunction;
}

void PAConsumer::setAPs(const std::vector<std::string>& aps) {
    this->aps = aps.size();
}

void PAConsumer::setAcceptanceCondition(unsigned int numberOfSets, acceptance_expr::ptr accExpr) {
    //TODO
}

void PAConsumer::provideAcceptanceName(const std::string& name, const std::vector<cpphoafparser::IntOrString>& extraInfo) {
    bool is_min = false, is_even = false;
    for (const cpphoafparser::IntOrString& info : extraInfo) {
        if (info.isInteger())
            continue;
        const std::string& info_str = info.getString();
        if (info_str == "min")
            is_min = true;
        if (info_str == "even")
            is_even = true;
    }

    if (name == "parity" && is_even && is_min)
        this->is_parity_mineven = true;
}

void PAConsumer::notifyBodyStart() {
    if (!this->is_parity_mineven)
        throw std::logic_error("HOA does not contain the 'parity' tag.");
}

void PAConsumer::addProperties(const std::vector<std::string>& properties) {
    for (const std::string& property : properties) {
        if (property == "deterministic")
            is_deterministic = true;
    }
}

void PAConsumer::addState(unsigned int id, std::shared_ptr<std::string> info, label_expr::ptr labelExpr,
                           std::shared_ptr<int_list> accSignature) {
    _get_automaton()->AddState(id);
    if (static_cast<bool>(accSignature))
        this->_get_automaton()->SetLabel(id, accSignature->front());
}


void PAConsumer::addEdgeWithLabel(unsigned int stateId, label_expr::ptr labelExpr, const int_list& conjSuccessors,
                                   std::shared_ptr<int_list> accSignature) {
    // Collect all symbols which satisfy the expression.
    std::vector<symbol_t> hit_symbols;
    for (symbol_t s = 0; s < static_cast<symbol_t>(1u) << this->aps; ++s) {
        const SymbolInterpreter si {s};
        if (EvaluateHOABooleanExpr(*labelExpr, si))
            hit_symbols.push_back(s);
    }

    for (symbol_t s : hit_symbols)
        for (state_t q : conjSuccessors)
            AddTransition(stateId, s, q);
}

void PAConsumer::notifyEnd() {
    FixInitialStates(this->initial_states);
}



}
}
}