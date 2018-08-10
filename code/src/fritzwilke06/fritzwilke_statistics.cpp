
#include <iostream>
#include <chrono>
#include <args.hxx>
#include "pa.hh"
#include "io.hh"
#include "delayed_simulation.h"
#include "../automaton/nbautils_bridge.h"
#include "../automaton/util.h"
#include "../../json/single_include/nlohmann/json.hpp"
#include <optional>
#include <cstdlib>
#include <json/json.h>
#include <optional>

using namespace tollk::automaton;

// Given an det. parity automaton, performs the Schewe construction (and removes unreachable states afterwards)
// and outputs some statistics in JSON format.
int main(int argc, char** argv);


enum OUTPUT_TYPE {
    CSV,
    JSON
};


// Parses the command line options.
struct Options {
    std::optional<unsigned int> size_limit;
    std::string input_file;
    OUTPUT_TYPE output_type = JSON;
    bool reset_at_sccs = false;
};

Options ParseArgs(int argc, char** argv) {
    args::ArgumentParser parser("Performs the FritzWilke reduction on a given DPA.");
    args::CompletionFlag completion(parser, {"complete"});
    args::HelpFlag help_flag(parser, "help", "Display this help menu.", {'h', "help"});
    args::ValueFlag<unsigned int> size_limit_flag(parser, "size_limit", "Cancel the process if the input automaton exeeds a certain size.", {"size_limit"});
    args::ValueFlag<std::string> output_flag(parser, "output", "Define the output format.", {'o', "output"});
    args::ValueFlag<std::string> input_file_flag(parser, "input_file", "HOA file that contains the input DPA.",
                                                 {'A', "automaton"}, args::Options::Required);
    args::Flag reset_at_sccs_flag(parser, "reset_at_sccs", "Optimization that resets de.sim. obligations at every SCC change.", {"reset_opt"});

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
    if (size_limit_flag)
        options.size_limit = size_limit_flag.Get();
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
    if (reset_at_sccs_flag)
        options.reset_at_sccs = reset_at_sccs_flag.Get();
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

// Given a parity automaton and a range of states, returns the smallest priority among them.
template <typename Rng>
tollk::automaton::parity_label_t MinParity(const tollk::automaton::ParityAutomaton& automaton, const Rng& range) {
    const auto get_label = [&automaton](tollk::automaton::state_t q) {return automaton.GetLabel(q);};
    return ranges::v3::min(ranges::v3::view::transform(range, get_label));
}

Statistics PerformConstruction(tollk::automaton::DPA dpa, const Options& options) {
    Statistics data{};
    data.original_size = dpa.States().size();
    data.number_of_sccs = CountSCCs(dpa);
    TimePoint time = TimeNow();

    const tollk::EquivalenceRelation<tollk::automaton::state_t> desim_relation = tollk::DelayedSimulationEquivalence(dpa, options.reset_at_sccs);
    tollk::automaton::NPA npa = tollk::automaton::NPA::FromDPA(dpa);
    const std::function<tollk::automaton::parity_label_t(const tollk::EquivalenceRelation<tollk::automaton::state_t>::EquivClass&)> f = std::bind(&MinParity<tollk::EquivalenceRelation<tollk::automaton::state_t>::EquivClass>, npa, std::placeholders::_1);
    tollk::automaton::QuotientAutomaton(&npa, desim_relation, f);
    const tollk::automaton::DPA final_dpa = tollk::automaton::DPA::FromNPA(npa);

    data.milliseconds_taken = TimeDiffMS(time, TimeNow());
    data.new_size = final_dpa.States().size();
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

    if (options.size_limit && dpa.States().size() > options.size_limit.value()) {
        std::cerr << "Automaton of size " << dpa.States().size() << " is too big." << std::endl;
        return 1;
    }

    const Statistics stats = PerformConstruction(dpa, options);
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