
#pragma once

#include "../construction_main.h"
#include "../automaton/hoa/hoa_io.h"
#include <iostream>

namespace tollk {

struct PathRefinementOptions : public BaseOptions {
    bool use_nbautils_labels = false;
};


class PathRefinementConstructionExecutor : public ConstructionExecutor {
 private:
    std::unique_ptr<args::Flag> useNbautilsLabelsFlag;
    std::map<automaton::state_t, std::string> stateLabels;

 public:
    void InitializeFlags(args::ArgumentParser* argParser) override;

    std::unique_ptr<BaseOptions> ParseFlags() const override;

    std::string ConstructionName() const override;

    void LoadInput(std::istream* istream) override;

    automaton::DPA PerformConstruction(const BaseOptions& options) const override;
};

}
