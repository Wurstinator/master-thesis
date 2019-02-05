

#include "main_statistics.h"
#include "tm_relation.h"
#include "../automaton/parity_util.h"
#include "../automaton/hoa/nbautils_post.h"

using namespace tollk;
using namespace automaton;

void ThresholdMooreConstructionExecutor::InitializeFlags(args::ArgumentParser* argParser) {
    ConstructionExecutor::InitializeFlags(argParser);
    useNbautilsLabelsFlag = std::make_unique<args::Flag>(*argParser, "nbautils",
                                                         "Uses state labels on the input automaton (in the output format of nbautils) to determine the congruence relation.",
                                                         args::Matcher{"nbautils"});
}

std::unique_ptr<BaseOptions> ThresholdMooreConstructionExecutor::ParseFlags() const {
    auto options = std::make_unique<ThresholdMooreOptions>();
    if (*this->useNbautilsLabelsFlag)
        options->use_nbautils_labels = this->useNbautilsLabelsFlag->Get();
    return options;
}

std::string ThresholdMooreConstructionExecutor::ConstructionName() const {
    return "ThresholdMoore";
}

void ThresholdMooreConstructionExecutor::LoadInput(std::istream* istream) {
    this->in_dpa = std::make_unique<automaton::DPA>(automaton::hoa::DPAFromHOA(istream, &this->stateLabels));
}

automaton::DPA ThresholdMooreConstructionExecutor::PerformConstruction(const BaseOptions& options_) const {
    automaton::DPA dpa = *this->in_dpa;

    // Make equivalence relation.
    const auto& options = dynamic_cast<const ThresholdMooreOptions&>(options_);
    EquivalenceRelation<automaton::state_t> R;
    if (options.use_nbautils_labels) {
        R = NbautilsEquivalence(this->stateLabels);
        R.RestrictDomain(dpa.States());
    } else {
        R = automaton::LanguageEquivalentStates(*in_dpa);
    }

    // Merge states.
    const EquivalenceRelation<automaton::state_t> TM = tollk::TMRelation(dpa, R);
    automaton::QuotientAutomatonUnsafe(&dpa, TM);
    return dpa;
}

std::unique_ptr<ConstructionExecutor> CreateConstructionExecutor() {
    return std::make_unique<ThresholdMooreConstructionExecutor>();
}