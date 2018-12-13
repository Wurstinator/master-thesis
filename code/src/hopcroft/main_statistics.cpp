

#include "main_statistics.h"

using namespace tollk;

std::string HopcroftConstructionExecutor::ConstructionName() const {
    return "Hopcroft";
}

automaton::DPA HopcroftConstructionExecutor::PerformConstruction(const BaseOptions& options_) const {
    automaton::DPA dpa = *this->in_dpa;
    tollk::automaton::Hopcroft(&dpa);
    return dpa;
}

std::unique_ptr<ConstructionExecutor> CreateConstructionExecutor() {
    return std::make_unique<HopcroftConstructionExecutor>();
}