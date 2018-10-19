

#include "main_statistics.h"
#include "delayed_simulation.h"

using namespace tollk;

void FritzWilkeConstructionExecutor::InitializeFlags(args::ArgumentParser* argParser) {
    ConstructionExecutor::InitializeFlags(argParser);
    resetAtSCCsFlag = std::make_unique<args::Flag>(*argParser, "reset_at_sccs", "Optimization that resets de.sim. obligations at every SCC change.", args::Matcher {"reset_opt"});
}

std::unique_ptr<BaseOptions> FritzWilkeConstructionExecutor::ParseFlags() const {
    auto options = std::make_unique<FritzWilkeOptions>();
    if (*this->resetAtSCCsFlag)
        options->reset_at_sccs = this->resetAtSCCsFlag->Get();
    return options;
}

std::string FritzWilkeConstructionExecutor::ConstructionName() const {
    return "Fritz-Wilke";
}

// Given a parity automaton and a range of states, returns the smallest priority among them.
template <typename Rng>
tollk::automaton::parity_label_t MinParity(const tollk::automaton::ParityAutomaton& automaton, const Rng& range) {
    const auto get_label = [&automaton](tollk::automaton::state_t q) {return automaton.GetLabel(q);};
    return ranges::v3::min(ranges::v3::view::transform(range, get_label));
}

automaton::DPA FritzWilkeConstructionExecutor::PerformConstruction(const BaseOptions& options_) const {
    const FritzWilkeOptions& options = dynamic_cast<const FritzWilkeOptions&>(options_);
    automaton::DPA dpa = *this->in_dpa;
    const tollk::EquivalenceRelation<tollk::automaton::state_t> desim_relation = tollk::DelayedSimulationEquivalence(dpa, options.reset_at_sccs);
    tollk::automaton::NPA npa = tollk::automaton::NPA::FromDeterministic(dpa);
    const std::function<tollk::automaton::parity_label_t(const tollk::EquivalenceRelation<tollk::automaton::state_t>::EquivClass&)> f = std::bind(&MinParity<tollk::EquivalenceRelation<tollk::automaton::state_t>::EquivClass>, npa, std::placeholders::_1);
    tollk::automaton::QuotientAutomatonLabelled(&npa, desim_relation, f);
    return tollk::automaton::DPA::FromNondeterministic(npa);
}

std::unique_ptr<ConstructionExecutor> CreateConstructionExecutor() {
    return std::make_unique<FritzWilkeConstructionExecutor>();
}