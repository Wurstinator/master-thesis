
#include "../construction_statistics.h"
#include "path_refinement.h"
#include "visit_graph.h"

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

tollk::automaton::DPA PerformConstruction(tollk::automaton::DPA dpa, const BaseOptions& options) {
    const EquivalenceRelation<automaton::state_t> R = LanguageEquivalentStates(dpa);
    for (const EquivalenceRelation<automaton::state_t>::EquivClass& lambda : R.Classes())
        PathRefinementMerge_VIS(&dpa, lambda);
    return dpa;
}
