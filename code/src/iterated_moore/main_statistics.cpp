
#include "main_statistics.h"
#include "iterated_moore.h"

using namespace tollk;

std::string IMConstructionExecutor::ConstructionName() const {
    return "Iterated Moore";
}

automaton::DPA IMConstructionExecutor::PerformConstruction(const BaseOptions& options_) const {
    automaton::DPA dpa = *this->in_dpa;
    const std::map<tollk::automaton::state_t, tollk::automaton::parity_label_t> normalized_prios = tollk::automaton::NormalizePriorities(
            dpa);
    for (const std::pair<const tollk::automaton::state_t, tollk::automaton::parity_label_t>& kv_pair : normalized_prios)
        dpa.SetLabel(kv_pair.first, kv_pair.second);
    tollk::IteratedMooreQuotient(&dpa);
    return dpa;
}


std::unique_ptr<ConstructionExecutor> CreateConstructionExecutor() {
    return std::make_unique<IMConstructionExecutor>();
}
