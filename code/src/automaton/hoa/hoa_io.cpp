
#include "hoa_io.h"
#include "cpphoafparser/consumer/hoa_consumer.hh"
#include "cpphoafparser/parser/hoa_parser.hh"
#include "transition_labeled_pa_consumer.h"
#include "dpa_consumer.h"
#include "npa_consumer.h"
#include "hoa_consumer_debug.h"


namespace tollk {
namespace automaton {
namespace hoa {

template <typename ConsumerT>
cpphoafparser::HOAConsumer::ptr ConsumeHOA(std::istream* istream, ConsumerT* consumer) {
    cpphoafparser::HOAConsumer::ptr shared_consumer(consumer);
    try {
        cpphoafparser::HOAParser::parse(*istream, shared_consumer);
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return shared_consumer;
}

DPA DPAFromHOA(std::istream* istream, std::map<state_t, std::string>* state_labels) {
    const cpphoafparser::HOAConsumer::ptr consumer = ConsumeHOA(istream, new DPAConsumer());
    const auto& cast_consumer = dynamic_cast<const DPAConsumer&>(*consumer);
    if (state_labels != nullptr)
        *state_labels = cast_consumer.GetStateLabels();
    return cast_consumer.GetAutomaton();
}

NPA NPAFromHOA(std::istream* istream, std::map<state_t, std::string>* state_labels) {
    const cpphoafparser::HOAConsumer::ptr consumer = ConsumeHOA(istream, new NPAConsumer());
    const auto& cast_consumer = dynamic_cast<const NPAConsumer&>(*consumer);
    if (state_labels != nullptr)
        *state_labels = cast_consumer.GetStateLabels();
    return cast_consumer.GetAutomaton();
}

DPA TransitionBasedDPAFromHOA(std::istream* istream, std::map<std::pair<state_t, parity_label_t>, state_t>* pair_indices, std::map<state_t, std::string>* state_labels) {
    const cpphoafparser::HOAConsumer::ptr consumer = ConsumeHOA(istream, new TransitionLabeledPAConsumer());
    const auto& cast_consumer = dynamic_cast<const TransitionLabeledPAConsumer&>(*consumer);
    if (pair_indices != nullptr)
        *pair_indices = cast_consumer.GetPairIndices();
    if (state_labels != nullptr)
        *state_labels = cast_consumer.GetStateLabels();
    return cast_consumer.GetAutomaton();
}

}
}
}