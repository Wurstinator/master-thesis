
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

const std::function<automaton::parity_label_t(const EquivalenceRelation<automaton::state_t>::EquivClass&)> MinPrioritySelector(const automaton::LabelledAutomaton<automaton::parity_label_t>& automaton) {
    return [&automaton](const EquivalenceRelation<automaton::state_t>::EquivClass& clas) {
        const auto compare_prio = [&automaton](automaton::state_t p, automaton::state_t q) {
            return automaton.GetLabel(p) < automaton.GetLabel(q);
        };
        const automaton::state_t min_state = *std::min_element(clas.begin(), clas.end(), compare_prio);
        return automaton.GetLabel(min_state);
    };
}

automaton::DPA PerformConstruction(automaton::DPA dpa, const BaseOptions& options) {
    const EquivalenceRelation<automaton::state_t> R = LanguageEquivalentStates(dpa);
    for (const EquivalenceRelation<automaton::state_t>::EquivClass& clas : R.Classes()) {
        const EquivalenceRelation<automaton::state_t> pr = PathRefinementEquivalence(dpa, clas);
        QuotientAutomatonLabelled(&dpa, pr, MinPrioritySelector(dpa));
    }
    return dpa;
}
