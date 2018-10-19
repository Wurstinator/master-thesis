
#pragma once

#include <string>
#include <args.hxx>
#include "automaton/util.h"

namespace tollk {

// only used for statistics
enum OUTPUT_TYPE {
    CSV,
    JSON
};

// Inherit from this class to add additional options.
struct BaseOptions {
    virtual ~BaseOptions() = default;
    std::string input_file; // filename of the input file
    std::optional<unsigned int> size_limit; // skip automata larger than this
    OUTPUT_TYPE output_type = JSON; // only used for statistics
};

class ConstructionExecutor {
 public:
    ConstructionExecutor() = default;

    virtual ~ConstructionExecutor() = default;

    // Sets up custom flags to use as options.
    virtual void InitializeFlags(args::ArgumentParser* argParser);

    // Parses flags into an options object.
    virtual std::unique_ptr<BaseOptions> ParseFlags() const;

    // Only used if "InitializeFlags" is not overridden. Should return the name of the construction (e.g. "Schewe").
    virtual std::string ConstructionName() const;

    // From an input stream, reads a DPA and stores it internally.
    virtual void LoadInput(std::istream* istream);

    // Runs the state reduction algorithm.
    virtual tollk::automaton::DPA PerformConstruction(const BaseOptions&) const = 0;

    // Returns the input DPA (only call after LoadInput).
    const tollk::automaton::DPA& GetInDPA() const;

 protected:
    std::unique_ptr<tollk::automaton::DPA> in_dpa;
};

}