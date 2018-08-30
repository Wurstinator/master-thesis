
/*
 * This file contains common functionality used by/for the *_main programs for each construction.
 */

#pragma once

#include <iostream>
#include <string>
#include <args.hxx>
#include "automaton/util.h"

// Inherit from this class to add additional options.
struct BaseOptions {
    virtual ~BaseOptions() = default;
    std::string input_file;
};


class ConstructionExecutor {
 public:
    ConstructionExecutor() = default;
    virtual ~ConstructionExecutor() = default;

    // Sets up custom flags to use as options.
    virtual void InitializeFlags(args::ArgumentParser* argParser);

    // Parses flags into an options object.
    virtual std::unique_ptr<BaseOptions> ParseFlags();

    // Only used if "InitializeFlags" is not overridden. Should return the name of the construction (e.g. "Schewe").
    virtual std::string ConstructionName() const;
};

// Have to be defined by the includer of this file.
std::unique_ptr<ConstructionExecutor> CreateConstructionExecutor();
tollk::automaton::DPA PerformConstruction(tollk::automaton::DPA, const BaseOptions&);

// Parses given args into options. In this process, ConstructionExecutor::InitializeFlags and ::ParseFlags are called.
std::unique_ptr<BaseOptions> ParseArgs(int argc, char** argv, ConstructionExecutor* specialization);

// Reads in a HOA file and converts it to a native automaton object.
tollk::automaton::NPA NPAFromHoa(const std::string& filename);

// Main
int main(int argc, char** argv);