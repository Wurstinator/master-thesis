
#include "../construction_main.h"
#include "iterated_moore.h"

class IMConstructionExecutor : public ConstructionExecutor {
 public:
    std::string ConstructionName() const override {
        return "iterated Moore";
    }
};

std::unique_ptr<ConstructionExecutor> CreateConstructionExecutor() {
    return std::make_unique<IMConstructionExecutor>();
}


tollk::automaton::DPA PerformConstruction(tollk::automaton::DPA dpa, const BaseOptions& options) {
    tollk::IteratedMooreQuotient(&dpa);
    return dpa;
}
