
#pragma once

#include "../construction_main.h"
#include "../automaton/hoa/hoa_io.h"
#include <iostream>

namespace tollk {

class EverythingSafraConstructionExecutor : public ConstructionExecutor {
 public:
    std::string ConstructionName() const override;
    automaton::DPA PerformConstruction(const BaseOptions& options) const override;
    void LoadInput(std::istream* istream) override;

 private:
    std::map<automaton::state_t, std::string> stateLabels;
};

}
