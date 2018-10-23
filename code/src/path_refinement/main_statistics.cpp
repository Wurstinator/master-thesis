

#include "main_statistics.h"
#include "nbautils_post.h"
#include "visit_graph.h"
#include "path_refinement.h"

using namespace tollk;
using namespace automaton;

void PathRefinementConstructionExecutor::InitializeFlags(args::ArgumentParser* argParser) {
    ConstructionExecutor::InitializeFlags(argParser);
    useNbautilsLabelsFlag = std::make_unique<args::Flag>(*argParser, "nbautils",
                                                         "Uses state labels on the input automaton (in the output format of nbautils) to determine the congruence relation.",
                                                         args::Matcher{"nbautils"});
}

std::unique_ptr<BaseOptions> PathRefinementConstructionExecutor::ParseFlags() const {
    auto options = std::make_unique<PathRefinementOptions>();
    if (*this->useNbautilsLabelsFlag)
        options->use_nbautils_labels = this->useNbautilsLabelsFlag->Get();
    return options;
}

std::string PathRefinementConstructionExecutor::ConstructionName() const {
    return "Path Refinement";
}

void PathRefinementConstructionExecutor::LoadInput(std::istream* istream) {
    this->in_dpa = std::make_unique<automaton::DPA>(automaton::hoa::DPAFromHOA(istream, &this->stateLabels));
}

automaton::DPA PathRefinementConstructionExecutor::PerformConstruction(const BaseOptions& options_) const {
    // Make equivalence relation.
    const auto& options = dynamic_cast<const PathRefinementOptions&>(options_);
    EquivalenceRelation<automaton::state_t> R;
    if (options.use_nbautils_labels)
        R = NbautilsEquivalence(this->stateLabels);
    else
        R = automaton::LanguageEquivalentStates(*in_dpa);

    // Merge states.
    automaton::DPA out_dpa = *in_dpa;
    for (const EquivalenceRelation<automaton::state_t>::EquivClass& lambda : R.Classes())
        PathRefinementMerge_VIS(&out_dpa, lambda);
    return out_dpa;
}

std::unique_ptr<ConstructionExecutor> CreateConstructionExecutor() {
    return std::make_unique<PathRefinementConstructionExecutor>();
}