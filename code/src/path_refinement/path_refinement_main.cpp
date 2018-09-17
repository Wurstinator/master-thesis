
#include "../construction_main.h"
#include "path_refinement.h"

using namespace tollk;

class IMConstructionExecutor : public ConstructionExecutor {
 public:
    std::string ConstructionName() const override {
        return "path refinement";
    }
};

std::unique_ptr<ConstructionExecutor> CreateConstructionExecutor() {
    return std::make_unique<IMConstructionExecutor>();
}


automaton::DPA PerformConstruction(automaton::DPA dpa, const BaseOptions& options) {
    const EquivalenceRelation<automaton::state_t> R = LanguageEquivalentStates(dpa);
    for (const EquivalenceRelation<automaton::state_t>::EquivClass& clas : R.Classes()) {
        const EquivalenceRelation<automaton::state_t> pr = PathRefinementEquivalence(dpa, clas);
        QuotientAutomatonUnsafe(&dpa, pr);
    }
    return dpa;
}
