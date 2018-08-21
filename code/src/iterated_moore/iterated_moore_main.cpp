
#include <iostream>
#include <args.hxx>
#include <io.hh>
#include "iterated_moore.h"
#include "../automaton/nbautils_bridge.h"
#include "../automaton/util.h"

// Given an det. parity automaton, performs the Schewe construction and outputs the result in the HOA format.
int main(int argc, char** argv);


// Parses the command line options.
struct Options {
    std::string input_file;
};

Options ParseArgs(int argc, char** argv) {
    args::ArgumentParser parser("Performs the Schewe construction on a given DPA.");
    args::CompletionFlag completion(parser, {"complete"});
    args::HelpFlag help_flag(parser, "help", "Display this help menu.", {'h', "help"});
    args::ValueFlag<std::string> input_file_flag(parser, "input_file", "HOA file that contains the input DPA.", {'A', "automaton"}, args::Options::Required);

    try {
        parser.ParseCLI(argc, argv);
    }
    catch (args::Completion e) {
        std::cout << e.what();
        exit(0);
    }
    catch (args::Help) {
        std::cout << parser;
        exit(0);
    }
    catch (args::ParseError e) {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        exit(1);
    }

    Options options;
    options.input_file = input_file_flag.Get();
    return options;
}


// Executes the process for one automaton.
tollk::automaton::DPA PerformConstruction(tollk::automaton::DPA dpa, const Options& options) {
    tollk::IteratedMooreQuotient(&dpa);
    return dpa;
}


tollk::automaton::NPA NPAFromHoa(const std::string& filename) {
    const std::vector<nbautils::SWA<std::string>::uptr> input_automata = nbautils::parse_hoa(filename);
    assert(input_automaton.size() == 1);
    return tollk::automaton::FromNbautils(*input_automata[0]);
}


// Main
int main(int argc, char** argv) {
    const Options options = ParseArgs(argc, argv);

    if (!std::ifstream(options.input_file)) {
        std::cerr << "Input file " << options.input_file << " does not exist or cannot be accessed." << std::endl;
        return 1;
    }

    const tollk::automaton::DPA in_dpa = tollk::automaton::DPA::FromNPA(NPAFromHoa(options.input_file));
    const tollk::automaton::DPA out_dpa = PerformConstruction(in_dpa, options);

    nbautils::print_hoa(tollk::automaton::ToNbautils(out_dpa), std::cout);
    std::cout << std::flush;

    return 0;
}
