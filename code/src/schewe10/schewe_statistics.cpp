
#include "../construction_statistics.h"
#include "schewe_automaton.h"


class ScheweConstructionExecutor : public ConstructionExecutor {
 public:
    std::string ConstructionName() const override {
        return "Schewe";
    }
};

std::unique_ptr<ConstructionExecutor> CreateConstructionExecutor() {
    return std::make_unique<ScheweConstructionExecutor>();
}


tollk::automaton::DPA PerformConstruction(tollk::automaton::DPA dpa, const BaseOptions& options) {
    tollk::ScheweAutomaton(&dpa);
    return dpa;
}

