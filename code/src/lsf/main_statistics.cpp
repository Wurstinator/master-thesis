

#include "main_statistics.h"
#include "../automaton/parity_util.h"
#include "lsf_relation.h"

using namespace tollk;

std::string LSFConstructionExecutor::ConstructionName() const {
    return "LSF";
}

automaton::DPA LSFConstructionExecutor::PerformConstruction(const BaseOptions&) const {
    automaton::DPA dpa = *this->in_dpa;
    const std::unordered_set<automaton::parity_label_t> labels = dpa.AllLabels();
    const std::set<automaton::parity_label_t> sorted_labels(labels.begin(), labels.end());
    for (automaton::parity_label_t k : sorted_labels)
        LSFPerformMerge(&dpa, k);
    return dpa;
}

std::unique_ptr<ConstructionExecutor> CreateConstructionExecutor() {
    return std::make_unique<LSFConstructionExecutor>();
}