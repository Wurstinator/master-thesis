
#include "construction_statistics.h"
#include "automaton/parity.h"
#include "../../json/single_include/nlohmann/json.hpp"
#include <fstream>
#include "automaton/hoa/hoa_io.h"


void ConstructionExecutor::InitializeFlags(args::ArgumentParser* argParser) {
    argParser->Description("Performs the " + ConstructionName() + " construction on a given DPA.");
}

std::unique_ptr<BaseOptions> ConstructionExecutor::ParseFlags() {
    return std::make_unique<BaseOptions>();
}

std::string ConstructionExecutor::ConstructionName() const {
    return "foo";
}


// Parser
std::unique_ptr<BaseOptions> ParseArgs(int argc, char** argv, ConstructionExecutor* specialization) {
    args::ArgumentParser parser("Performs a construction on a given DPA.");
    args::CompletionFlag completion(parser, {"complete"});
    args::HelpFlag help_flag(parser, "help", "Display this help menu.", {'h', "help"});
    args::ValueFlag<unsigned int> size_limit_flag(parser, "size_limit", "Cancel the process if the input automaton exeeds a certain size.", {"size_limit"});
    args::ValueFlag<std::string> output_flag(parser, "output", "Define the output format.", {'o', "output"});
    args::ValueFlag<std::string> input_file_flag(parser, "input_file", "HOA file that contains the input DPA.", {'A', "automaton"}, args::Options::Required);
    specialization->InitializeFlags(&parser);

    try {
        parser.ParseCLI(argc, argv);
    }
    catch (args::Completion& e) {
        std::cout << e.what();
        exit(0);
    }
    catch (args::Help&) {
        std::cout << parser;
        exit(0);
    }
    catch (args::ParseError& e) {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        exit(1);
    }

    std::unique_ptr<BaseOptions> options = specialization->ParseFlags();
    if (size_limit_flag)
        options->size_limit = size_limit_flag.Get();
    options->input_file = input_file_flag.Get();
    if (output_flag) {
        if (output_flag.Get() == "json")
            options->output_type = JSON;
        else if (output_flag.Get() == "csv")
            options->output_type = CSV;
        else {
            std::cerr << "Invalid output format specified. Allowed are 'json' or 'csv'." << std::endl;
            std::exit(EXIT_FAILURE);
        }
    }
    return options;
}


tollk::automaton::NPA NPAFromHoa(const std::string& filename) {
    std::ifstream file(filename);
    return tollk::automaton::hoa::NPAFromHOA(&file);
}



// Measurement
using TimePoint = std::chrono::steady_clock::time_point;

TimePoint TimeNow() {
    return std::chrono::steady_clock::now();
}

long TimeDiffMS(const TimePoint& a, const TimePoint& b) {
    return std::chrono::duration_cast<std::chrono::milliseconds>(b - a).count();
}

size_t CountSCCs(const tollk::automaton::DPA& automaton) {
    return StronglyConnectedComponents(automaton).sccs.size();
}



// Given a parity automaton and a range of states, returns the smallest priority among them.
template <typename Rng>
tollk::automaton::parity_label_t MinParity(const tollk::automaton::ParityAutomaton& automaton, const Rng& range) {
    const auto get_label = [&automaton](tollk::automaton::state_t q) {return automaton.GetLabel(q);};
    return ranges::v3::min(ranges::v3::view::transform(range, get_label));
}

Statistics PerformConstructionMeasurement(tollk::automaton::DPA dpa, const BaseOptions& options) {
    Statistics data{};
    data.original_size = dpa.States().size();
    data.number_of_sccs = CountSCCs(dpa);
    TimePoint time = TimeNow();
    tollk::automaton::DPA final_dpa = PerformConstruction(dpa, options);
    data.milliseconds_taken = TimeDiffMS(time, TimeNow());
    data.new_size = final_dpa.States().size();
    return data;
}



// Output
void PrintCSV(const Statistics& stat) {
    std::cout << stat.input_file << ','
              << stat.original_size << ','
              << stat.number_of_sccs << ','
              << stat.milliseconds_taken << ','
              << stat.new_size
              << std::endl;
}

void PrintJSON(const Statistics& stat) {
    nlohmann::json j;
    j["input_file"] = stat.input_file;
    j["original_size"] = stat.original_size;
    j["original_number_of_sccs"] = stat.number_of_sccs;
    j["milliseconds"] = stat.milliseconds_taken;
    j["new_size"] = stat.new_size;
    std::cout << j << std::endl;
}


// Main
int main(int argc, char** argv) {
    const std::unique_ptr<ConstructionExecutor> construction_executor = CreateConstructionExecutor();
    const std::unique_ptr<const BaseOptions> options = ParseArgs(argc, argv, construction_executor.get());

    if (!std::ifstream(options->input_file)) {
        std::cerr << "Input file " << options->input_file << " does not exist or cannot be accessed." << std::endl;
        return 1;
    }

    const tollk::automaton::NPA npa = NPAFromHoa(options->input_file);
    const tollk::automaton::DPA dpa = tollk::automaton::DPA::FromNondeterministic(npa);

    if (options->size_limit && dpa.States().size() > options->size_limit.value()) {
        std::cerr << "Automaton of size " << dpa.States().size() << " is too big." << std::endl;
        return 1;
    }

    Statistics stats = PerformConstructionMeasurement(dpa, *options);
    stats.input_file = options->input_file;
    switch (options->output_type) {
        case JSON:
            PrintJSON(stats);
            break;
        case CSV:
            PrintCSV(stats);
            break;
    }

    return 0;
}