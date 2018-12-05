

#include "main_statistics.h"

using namespace tollk;
using namespace automaton;

std::string SimpleLangeqConstructionExecutor::ConstructionName() const {
    return "SimpleLangeq";
}

automaton::DPA SimpleLangeqConstructionExecutor::PerformConstruction(const BaseOptions& options_) const {
    automaton::DPA dpa = *this->in_dpa;
    const EquivalenceRelation<automaton::state_t> langequiv = automaton::LanguageEquivalentStates(dpa);

    EquivalenceRelation<automaton::state_t> R;
    for (state_t p : dpa.States()) {
        R.AddConnection(p, p);
        for (state_t q : dpa.States()) {
            if (dpa.GetLabel(p) == dpa.GetLabel(q) && langequiv.IsEquiv(p, q))
                R.AddConnection(p, q);
        }
    }

    QuotientAutomatonUnsafe(&dpa, R);
    return dpa;
}

std::unique_ptr<ConstructionExecutor> CreateConstructionExecutor() {
    return std::make_unique<SimpleLangeqConstructionExecutor>();
}