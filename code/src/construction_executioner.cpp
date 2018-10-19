//
// Created by wurst on 19.10.18.
//

#include "construction_executioner.h"
#include "automaton/hoa/hoa_io.h"

using namespace tollk;

void ConstructionExecutor::InitializeFlags(args::ArgumentParser* argParser) {
    argParser->Description("Performs the " + ConstructionName() + " construction on a given DPA.");
}

std::unique_ptr<BaseOptions> ConstructionExecutor::ParseFlags() const {
    return std::make_unique<BaseOptions>();
}

std::string ConstructionExecutor::ConstructionName() const {
    return "foo";
}

void ConstructionExecutor::LoadInput(std::istream* istream) {
    this->in_dpa = std::make_unique<tollk::automaton::DPA>(tollk::automaton::hoa::DPAFromHOA(istream));
}

const tollk::automaton::DPA& ConstructionExecutor::GetInDPA() const {
    return *this->in_dpa;
}
