

#include "main_statistics.h"
#include "../schewe10/schewe_automaton.h"
#include "../iterated_moore/iterated_moore.h"
#include "../threshold_moore/tm_relation.h"
#include "../lsf/lsf_relation.h"
#include "../fritzwilke06/delayed_simulation.h"
#include "../path_refinement/visit_graph.h"
#include "../automaton/hoa/nbautils_post.h"

using namespace tollk;
using namespace automaton;

std::string EverythingSafraConstructionExecutor::ConstructionName() const {
    return "Everything Safra";
}

void EverythingSafraConstructionExecutor::LoadInput(std::istream* istream) {
    this->in_dpa = std::make_unique<automaton::DPA>(hoa::DPAFromHOA(istream, &this->stateLabels));
}


// Given a parity automaton and a range of states, returns the smallest priority among them.
template <typename Rng>
tollk::automaton::parity_label_t MinParity(const tollk::automaton::ParityAutomaton& automaton, const Rng& range) {
    const auto get_label = [&automaton](tollk::automaton::state_t q) {return automaton.GetLabel(q);};
    return ranges::v3::min(ranges::v3::view::transform(range, get_label));
}

DPA EverythingSafraConstructionExecutor::PerformConstruction(const BaseOptions& options_) const {
    DPA dpa = *this->in_dpa;

    // Normalize c
    const std::map<state_t, parity_label_t> normalized_prios = NormalizePriorities(
            dpa);
    for (const std::pair<const state_t, parity_label_t>& kv_pair : normalized_prios)
        dpa.SetLabel(kv_pair.first, kv_pair.second);

    // Safra Language equivalence
    EquivalenceRelation<state_t> language_equivalence = NbautilsEquivalence(this->stateLabels);
    const auto update_langequiv = [&language_equivalence, &dpa]() {
        language_equivalence.RestrictDomain(dpa.States());
    };

    // Schewe & Hopcroft
    Hopcroft(&dpa);
    update_langequiv();
    ScheweAutomaton(&dpa, language_equivalence);
    Hopcroft(&dpa);

    // Iterated Moore
    IteratedMooreQuotient(&dpa);

    // Threshold Moore
    update_langequiv();
    const EquivalenceRelation<state_t> TM = TMRelation(dpa, language_equivalence);
    QuotientAutomatonUnsafe(&dpa, TM);

    // LSF
    const std::unordered_set<parity_label_t> labels = dpa.AllLabels();
    const std::set<parity_label_t> sorted_labels(labels.begin(), labels.end());
    for (parity_label_t k : sorted_labels) {
        update_langequiv();
        LSFPerformMerge(&dpa, k, language_equivalence);
    }

    // FritzWilke
    //const EquivalenceRelation<state_t> desim_relation = DelayedSimulationEquivalence(dpa, false);
    //const std::function<parity_label_t(const EquivalenceRelation<state_t>::EquivClass&)> f = std::bind(&MinParity<EquivalenceRelation<state_t>::EquivClass>, dpa, std::placeholders::_1);
    //QuotientAutomatonLabelled(&dpa, desim_relation, f);

    // Path Refinement
    update_langequiv();
    for (const EquivalenceRelation<state_t>::EquivClass& lambda : language_equivalence.Classes())
        PathRefinementMerge_VIS(&dpa, lambda);


    return dpa;
}

std::unique_ptr<ConstructionExecutor> CreateConstructionExecutor() {
    return std::make_unique<EverythingSafraConstructionExecutor>();
}