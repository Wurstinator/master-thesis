
#include <iostream>
#include <args.hxx>
#include <io.hh>
#include "schewe_automaton.h"
#include "../automaton/nbautils_bridge.h"
#include "../automaton/util.h"

// Given an det. parity automaton, performs the Schewe construction and outputs the result in the HOA format.
// Arguments:
//   -c  : Makes the automaton complete by introducing a new sink state.
//   -mb : Minimizes the automaton as a Moore automaton before the construction.
//   -ma : Minimizes the automaton as a Moore automaton after the construction.
//   -A  : Path to the file in which the DPA is stored in HOA format.
int main(int argc, char** argv);


// Parses the command line options.
struct Options {
    bool make_complete;
    bool minimize_before;
    bool minimize_after;
    std::string input_file;
};

Options ParseArgs(int argc, char** argv) {
    args::ArgumentParser parser("Performs the Schewe construction on a given DPA.");
    args::CompletionFlag completion(parser, {"complete"});
    args::HelpFlag help_flag(parser, "help", "Display this h elp menu.", {'h', "help"});
    args::Flag minimize_before_flag(parser, "minimize_before", "Minimize the automaton with Hopcraft before applying the construction.", {"mb"});
    args::Flag minimize_after_flag(parser, "minimize_after", "Minimize the automaton with Hopcraft after applying the construction.", {"ma"});
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
    options.minimize_before = minimize_before_flag.Get();
    options.minimize_after = minimize_after_flag.Get();
    options.input_file = input_file_flag.Get();
    return options;
}


// Executes the process for one automaton.
tollk::automaton::DPA PerformConstruction(tollk::automaton::DPA automaton, const Options& options) {
    if (options.minimize_before)
        tollk::automaton::Hopcroft(&automaton);

    tollk::ScheweAutomaton(&automaton);

    if (options.minimize_after)
        tollk::automaton::Hopcroft(&automaton);

    return automaton;
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

    const tollk::automaton::NPA npa = NPAFromHoa(options.input_file);
    const tollk::automaton::DPA dpa = tollk::automaton::DPA::FromNPA(npa);
    const tollk::automaton::DPA out = PerformConstruction(dpa, options);
    nbautils::print_hoa(tollk::automaton::ToNbautils(out), std::cout);
    std::cout << std::flush;

    return 0;
}