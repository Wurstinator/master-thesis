

#include "main_statistics.h"
#include "skipper_automaton.h"

using namespace tollk;

std::string SkipperConstructionExecutor::ConstructionName() const {
    return "Skip Merge";
}

automaton::DPA SkipperConstructionExecutor::PerformConstruction(const BaseOptions& options_) const {
    automaton::DPA dpa = *this->in_dpa;
    tollk::SkipperAutomaton(&dpa);
    tollk::automaton::Hopcroft(&dpa);
    return dpa;
}

std::unique_ptr<ConstructionExecutor> CreateConstructionExecutor() {
    return std::make_unique<SkipperConstructionExecutor>();
}