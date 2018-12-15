

#include "main_statistics.h"
#include "../automaton/parity_util.h"
#include "lsf_relation.h"
#include "../automaton/hoa/nbautils_post.h"

using namespace tollk;
using namespace automaton;

void LSFConstructionExecutor::InitializeFlags(args::ArgumentParser* argParser) {
    ConstructionExecutor::InitializeFlags(argParser);
    useNbautilsLabelsFlag = std::make_unique<args::Flag>(*argParser, "nbautils",
                                                         "Uses state labels on the input automaton (in the output format of nbautils) to determine the congruence relation.",
                                                         args::Matcher{"nbautils"});
}

std::unique_ptr<BaseOptions> LSFConstructionExecutor::ParseFlags() const {
    auto options = std::make_unique<LSFOptions>();
    if (*this->useNbautilsLabelsFlag)
        options->use_nbautils_labels = this->useNbautilsLabelsFlag->Get();
    return options;
}

std::string LSFConstructionExecutor::ConstructionName() const {
    return "LSF";
}

void LSFConstructionExecutor::LoadInput(std::istream* istream) {
    this->in_dpa = std::make_unique<automaton::DPA>(hoa::DPAFromHOA(istream, &this->stateLabels));
}

automaton::DPA LSFConstructionExecutor::PerformConstruction(const BaseOptions& options_) const {
    const auto& options = dynamic_cast<const LSFOptions&>(options_);

    automaton::DPA dpa = *this->in_dpa;
    const std::vector<state_t> original_states = dpa.States();
    const std::unordered_set<automaton::parity_label_t> labels = dpa.AllLabels();
    const std::set<automaton::parity_label_t> sorted_labels(labels.begin(), labels.end());
    for (automaton::parity_label_t k : sorted_labels) {

        EquivalenceRelation<automaton::state_t> sim;
        if (options.use_nbautils_labels) {
            sim = NbautilsEquivalence(this->stateLabels);
            std::set<state_t> removed_states(original_states.begin(), original_states.end());
            for (state_t q : dpa.States())
                removed_states.erase(q);
            for (state_t q : removed_states)
                sim.Remove(q);
        } else {
            sim = LanguageEquivalentStates(dpa);
        }

        LSFPerformMerge(&dpa, k, sim);
    }
    return dpa;
}

std::unique_ptr<ConstructionExecutor> CreateConstructionExecutor() {
    return std::make_unique<LSFConstructionExecutor>();
}