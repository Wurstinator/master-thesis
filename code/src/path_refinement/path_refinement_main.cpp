
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
    EquivalenceRelation<automaton::state_t> er1, er2, er3;
    er1.AddConnection(17, 24);
    er2.AddConnection(15, 18);
    er2.AddConnection(18, 22);
    er3.AddConnection(19, 20);
    //QuotientAutomatonUnsafe(&dpa, er1);
    //QuotientAutomatonUnsafe(&dpa, er2);
    //QuotientAutomatonUnsafe(&dpa, er3);
    // 17 24
    // 15 18 22
    // 19 20
    return dpa;
}
