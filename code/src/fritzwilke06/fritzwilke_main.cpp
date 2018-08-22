



#include "../construction_main.h"
#include "delayed_simulation.h"

struct FritzWilkeOptions : public BaseOptions {
    bool reset_at_sccs = false;
};

class FritzWilkeConstructionExecutor : public ConstructionExecutor {
 private:
    std::unique_ptr<args::Flag> resetAtSCCsFlag;

 public:
    void InitializeFlags(args::ArgumentParser* argParser) override {
        ConstructionExecutor::InitializeFlags(argParser);
        resetAtSCCsFlag = std::make_unique<args::Flag>(*argParser, "reset_at_sccs", "Optimization that resets de.sim. obligations at every SCC change.", args::Matcher {"reset_opt"});
    }

    std::unique_ptr<BaseOptions> ParseFlags() override {
        auto options = std::make_unique<FritzWilkeOptions>();
        if (*this->resetAtSCCsFlag)
            options->reset_at_sccs = this->resetAtSCCsFlag->Get();
        return options;
    }

    std::string ConstructionName() const override {
        return "Fritz-Wilke";
    }
};

std::unique_ptr<ConstructionExecutor> CreateConstructionExecutor() {
    return std::make_unique<FritzWilkeConstructionExecutor>();
}


// Given a parity automaton and a range of states, returns the smallest priority among them.
template <typename Rng>
tollk::automaton::parity_label_t MinParity(const tollk::automaton::ParityAutomaton& automaton, const Rng& range) {
    const auto get_label = [&automaton](tollk::automaton::state_t q) {return automaton.GetLabel(q);};
    return ranges::v3::min(ranges::v3::view::transform(range, get_label));
}


// Executes the process for one automaton.
tollk::automaton::DPA PerformConstruction(tollk::automaton::DPA dpa, const BaseOptions& options_) {
    const FritzWilkeOptions& options = dynamic_cast<const FritzWilkeOptions&>(options_);
    const tollk::EquivalenceRelation<tollk::automaton::state_t> desim_relation = tollk::DelayedSimulationEquivalence(dpa, options.reset_at_sccs);
    tollk::automaton::NPA npa = tollk::automaton::NPA::FromDPA(dpa);
    const std::function<tollk::automaton::parity_label_t(const tollk::EquivalenceRelation<tollk::automaton::state_t>::EquivClass&)> f = std::bind(&MinParity<tollk::EquivalenceRelation<tollk::automaton::state_t>::EquivClass>, npa, std::placeholders::_1);
    tollk::automaton::QuotientAutomaton(&npa, desim_relation, f);
    return tollk::automaton::DPA::FromNPA(npa);
}
