
#include <iostream>
#include <chrono>
#include <args.hxx>
#include "pa.hh"
#include "io.hh"
#include "schewe_automaton.h"
#include "../automaton/nbautils_bridge.h"
#include "../automaton/util.h"
#include "../../json/single_include/nlohmann/json.hpp"
#include <optional>
#include <cstdlib>
#include <json/json.h>

using namespace tollk::automaton;

// Given an det. parity automaton, performs the Schewe construction (and removes unreachable states afterwards)
// and outputs some statistics in JSON format.
int main(int argc, char** argv);

const static size_t SIZE_LIMIT = 400;

enum OUTPUT_TYPE {
    CSV,
    JSON
};


// Parses the command line options.
struct Options {
    bool no_size_limit;
    std::string input_file;
    OUTPUT_TYPE output_type = JSON;
};

Options ParseArgs(int argc, char** argv) {
    args::ArgumentParser parser("Performs the Schewe construction on a given DPA.");
    args::CompletionFlag completion(parser, {"complete"});
    args::HelpFlag help_flag(parser, "help", "Display this help menu.", {'h', "help"});
    args::Flag no_size_limit_flag(parser, "no_size_limit", "Do not cancel the process even if the automaton size exceeds a threshold.", {"no_size_limit"});
    args::ValueFlag<std::string> output_flag(parser, "output", "Define the output format.", {'o', "output"});
    args::ValueFlag<std::string> input_file_flag(parser, "input_file", "HOA file that contains the input DPA.",
                                                 {'A', "automaton"}, args::Options::Required);

    try {
        parser.ParseCLI(argc, argv);
    }
    catch (args::Completion& e) {
        std::cout << e.what();
        std::exit(EXIT_SUCCESS);
    }
    catch (args::Help&) {
        std::cout << parser;
        std::exit(EXIT_SUCCESS);
    }
    catch (args::ParseError& e) {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        std::exit(EXIT_FAILURE);
    }

    Options options;
    options.no_size_limit = no_size_limit_flag.Get();
    options.input_file = input_file_flag.Get();
    if (output_flag) {
        if (output_flag.Get() == "json")
            options.output_type = JSON;
        else if (output_flag.Get() == "csv")
            options.output_type = CSV;
        else {
            std::cerr << "Invalid output format specified. Allowed are 'json' or 'csv'." << std::endl;
            std::exit(EXIT_FAILURE);
        }

    }
    return options;
}

// Executes the process for one automaton.
struct Statistics {
    size_t original_size;
    size_t number_of_sccs;
    long milliseconds_taken;
    size_t new_size;
};

using TimePoint = std::chrono::steady_clock::time_point;

TimePoint TimeNow() {
    return std::chrono::steady_clock::now();
}

long TimeDiffMS(const TimePoint& a, const TimePoint& b) {
    return std::chrono::duration_cast<std::chrono::milliseconds>(b - a).count();
}

size_t CountSCCs(const DPA& automaton) {
    return StronglyConnectedComponents(automaton).sccs.size();
}

void RemoveUnreachableStates(tollk::automaton::DPA* automaton) {
    std::unordered_set<state_t> unreachable_states(automaton->States().begin(), automaton->States().end());
    for (state_t q : ReachableStates(*automaton, automaton->InitialState()))
        unreachable_states.erase(q);

    for (state_t q : unreachable_states)
        automaton->RemoveState(q);
}


Statistics PerformConstruction(tollk::automaton::DPA automaton) {
    Statistics data{};
    data.original_size = automaton.States().size();
    data.number_of_sccs = CountSCCs(automaton);

    TimePoint time = TimeNow();
    tollk::ScheweAutomaton(&automaton);
    RemoveUnreachableStates(&automaton);
    data.milliseconds_taken = TimeDiffMS(time, TimeNow());
    data.new_size = automaton.States().size();

    return data;
}

void PrintCSV(const Statistics& stat) {
    std::cout << stat.original_size << ','
              << stat.number_of_sccs << ','
              << stat.milliseconds_taken << ','
              << stat.new_size
              << std::endl;
}

void PrintJSON(const Statistics& stat) {
    nlohmann::json j;
    j["original_size"] = stat.original_size;
    j["original_number_of_sccs"] = stat.number_of_sccs;
    j["milliseconds"] = stat.milliseconds_taken;
    j["new_size"] = stat.new_size;
    std::cout << j << std::endl;
}

tollk::automaton::NPA NPAFromHoa(const std::string& filename) {
    const std::vector<nbautils::SWA<std::string>::uptr> input_automata = nbautils::parse_hoa(filename);
    assert(input_automaton.size() == 1);
    return FromNbautils(*input_automata[0]);
}

// Main
int main(int argc, char** argv) {
    const Options options = ParseArgs(argc, argv);

    if (!std::ifstream(options.input_file)) {
        std::cerr << "Input file " << options.input_file << " does not exist or cannot be accessed." << std::endl;
        return 1;
    }

    const NPA npa = NPAFromHoa(options.input_file);
    const DPA dpa = DPA::FromNPA(npa);

    if (!options.no_size_limit && dpa.States().size() > SIZE_LIMIT) {
        std::cerr << "Automaton of size " << dpa.States().size() << " is too big." << std::endl;
        return 1;
    }

    const Statistics stats = PerformConstruction(dpa);
    switch (options.output_type) {
        case JSON:
            PrintJSON(stats);
            break;
        case CSV:
            PrintCSV(stats);
            break;
    }

    return 0;
}