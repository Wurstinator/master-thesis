
#include "../construction_statistics.h"
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
    const std::map<tollk::automaton::state_t, tollk::automaton::parity_label_t> normalized_prios = tollk::automaton::NormalizePriorities(dpa);
    for (const std::pair<const tollk::automaton::state_t, tollk::automaton::parity_label_t>& kv_pair : normalized_prios)
        dpa.SetLabel(kv_pair.first, kv_pair.second);
    tollk::IteratedMooreQuotient(&dpa);
    return dpa;
}
