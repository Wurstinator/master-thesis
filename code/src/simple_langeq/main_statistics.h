
#pragma once

#include "../construction_main.h"
#include "../automaton/hoa/hoa_io.h"
#include <iostream>

namespace tollk {

class SimpleLangeqConstructionExecutor : public ConstructionExecutor {
 public:
    std::string ConstructionName() const override;
    automaton::DPA PerformConstruction(const BaseOptions& options) const override;
};

}
