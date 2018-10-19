
#include "transition_labeled_pa_consumer.h"
#include "boolean_evaluation.h"
#include "../transition_util.h"


namespace tollk {
namespace automaton {
namespace hoa {

void TransitionLabeledPAConsumer::addState(unsigned int id, std::shared_ptr<std::string> info,
                                           cpphoafparser::BooleanExpression<cpphoafparser::AtomLabel>::ptr labelExpr,
                                           std::shared_ptr<int_list> accSignature) {
    recorded_states.insert(id);
    if (static_cast<bool>(info))
        this->state_labels[id] = *info;
}

void TransitionLabeledPAConsumer::addEdgeWithLabel(unsigned int stateId,
                                                   cpphoafparser::BooleanExpression<cpphoafparser::AtomLabel>::ptr labelExpr,
                                                   const int_list& conjSuccessors,
                                                   std::shared_ptr<int_list> accSignature) {
    // Collect all symbols which satisfy the expression.
    std::vector<symbol_t> hit_symbols;
    for (symbol_t s = 0; s < static_cast<symbol_t>(1u) << this->aps; ++s) {
        const SymbolInterpreter si{s};
        if (EvaluateHOABooleanExpr(*labelExpr, si))
            hit_symbols.push_back(s);
    }

    // Get the parity color of that transition.
    assert(static_cast<bool>(accSignature));
    const parity_label_t label_priority = accSignature->front();
    this->recorded_priorities.insert(label_priority);

    // Memorize the transitions, to add them later.
    for (symbol_t s : hit_symbols)
        for (state_t q : conjSuccessors)
            this->transitions.push_back(Transition{stateId, s, q, label_priority});
}

void TransitionLabeledPAConsumer::notifyEnd() {
    // Create the states.
    for (state_t q : this->recorded_states) {
        for (parity_label_t k : this->recorded_priorities) {
            const std::pair<state_t, parity_label_t> pair_index(q, k);
            const state_t new_id = _get_automaton()->States().size();
            _get_automaton()->AddState(new_id);
            _get_automaton()->SetLabel(new_id, k);
            this->pair_indices[pair_index] = new_id;
        }
    }

    // Add transitions.
    for (const Transition& transition : this->transitions) {
        for (parity_label_t k : this->recorded_priorities) {
            const std::pair<state_t, parity_label_t> pair_from(transition.p, k);
            const std::pair<state_t, parity_label_t> pair_to(transition.q, transition.k);
            AddTransition(this->pair_indices[pair_from], transition.s, this->pair_indices[pair_to]);
        }
    }

    DPAConsumer::notifyEnd();

    // Remove unreachable states.
    std::unordered_set<state_t> unreachable_states(_get_automaton()->States().begin(),
                                                   _get_automaton()->States().end());
    for (state_t q : ReachableStates(*this->automaton, _get_automaton()->InitialState()))
        unreachable_states.erase(q);
    for (state_t q : unreachable_states)
        _get_automaton()->RemoveState(q);
}


const std::map<std::pair<state_t, parity_label_t>, state_t>& TransitionLabeledPAConsumer::GetPairIndices() const {
    return this->pair_indices;
}

const std::map<state_t, std::string>& TransitionLabeledPAConsumer::GetStateLabels() const {
    return this->state_labels;
}

}
}
}