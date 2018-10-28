
#pragma once
#include "dpa_consumer.h"
#include "../parity.h"

/**
 * This consumer class is used for the special case of DPAs generated by nbautils. They have transition based priorities
 * which have to be converted. They can also contain labels (in the form of strings) on their states.
 */

namespace tollk {
namespace automaton {
namespace hoa {

class TransitionLabeledPAConsumer : public DPAConsumer {
 public:
    void addState(unsigned int id, std::shared_ptr<std::string> info, label_expr::ptr labelExpr,
                  std::shared_ptr<int_list> accSignature) override;
    void addEdgeWithLabel(unsigned int stateId, label_expr::ptr labelExpr, const int_list& conjSuccessors,
                          std::shared_ptr<int_list> accSignature) override;
    void notifyEnd() override;

    const std::map<std::pair<state_t, parity_label_t>, state_t>& GetPairIndices() const;
    const std::map<state_t, std::string>& GetStateLabels() const;

 protected:
    struct Transition {
        state_t p;
        symbol_t s;
        state_t q;
        parity_label_t k;
    };

    // Temporary storage of the automaton to construct it only after reading the whole file.
    std::set<state_t> recorded_states;
    std::set<parity_label_t> recorded_priorities;
    std::vector<Transition> transitions;

    std::map<std::pair<state_t, parity_label_t>, state_t> pair_indices;
    std::map<state_t, std::string> state_labels;
};



}
}
}