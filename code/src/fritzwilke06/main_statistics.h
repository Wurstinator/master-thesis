
#pragma once

#include "../construction_main.h"
#include "../automaton/hoa/hoa_io.h"
#include <iostream>

namespace tollk {

struct FritzWilkeOptions : public BaseOptions {
    bool reset_at_sccs = false;
};


class FritzWilkeConstructionExecutor : public ConstructionExecutor {
 private:
    std::unique_ptr<args::Flag> resetAtSCCsFlag;

 public:
    void InitializeFlags(args::ArgumentParser* argParser) override;

    std::unique_ptr<BaseOptions> ParseFlags() const override;

    std::string ConstructionName() const override;

    automaton::DPA PerformConstruction(const BaseOptions& options) const override;
};

}
