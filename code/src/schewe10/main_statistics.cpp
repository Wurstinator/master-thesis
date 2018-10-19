

#include "main_statistics.h"
#include "schewe_automaton.h"

using namespace tollk;

std::string ScheweConstructionExecutor::ConstructionName() const {
    return "Schewe";
}

automaton::DPA ScheweConstructionExecutor::PerformConstruction(const BaseOptions& options_) const {
    automaton::DPA dpa = *this->in_dpa;
    tollk::ScheweAutomaton(&dpa);
    tollk::automaton::Hopcroft(&dpa);
    return dpa;
}

std::unique_ptr<ConstructionExecutor> CreateConstructionExecutor() {
    return std::make_unique<ScheweConstructionExecutor>();
}