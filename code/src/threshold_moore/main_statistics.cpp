

#include "main_statistics.h"
#include "tm_relation.h"
#include "../automaton/parity_util.h"

using namespace tollk;

std::string ThresholdMooreConstructionExecutor::ConstructionName() const {
    return "ThresholdMoore";
}

automaton::DPA ThresholdMooreConstructionExecutor::PerformConstruction(const BaseOptions&) const {
    automaton::DPA dpa = *this->in_dpa;
    const EquivalenceRelation<automaton::state_t> language_equivalence = automaton::LanguageEquivalentStates(dpa);
    const EquivalenceRelation<automaton::state_t> TM = tollk::TMRelation(dpa, language_equivalence);
    automaton::QuotientAutomatonUnsafe(&dpa, TM);
    return dpa;
}

std::unique_ptr<ConstructionExecutor> CreateConstructionExecutor() {
    return std::make_unique<ThresholdMooreConstructionExecutor>();
}