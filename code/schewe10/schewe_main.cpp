
#include <iostream>
#include <args.hxx>
#include "pa.hh"
#include "io.hh"
#include "schewe_automaton.h"

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
    args::Flag make_complete_flag(parser, "make_complete", "Make the automaton complete by introducing a sink state.", {'c'});
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
    options.make_complete = make_complete_flag.Get();
    options.minimize_before = minimize_before_flag.Get();
    options.minimize_after = minimize_after_flag.Get();
    options.input_file = input_file_flag.Get();
    return options;
}


template<typename TagT>
void MinimizePA(nbautils::SWA<TagT>* automaton) {
    const std::function<std::vector<nbautils::state_t>(nbautils::state_t)> get_succs =
            [automaton](nbautils::state_t q) {
                return automaton->succ(q);
            };
    assert(automaton->get_init().size() == 1);
    const nbautils::state_t initial = automaton->get_init()[0];
    std::vector<nbautils::state_t> unreachable = nbautils::unreachable_states(automaton->states(), initial, get_succs);
    std::sort(unreachable.begin(), unreachable.end());
    automaton->remove_states(unreachable);
    nbautils::minimize_pa(*automaton);
}

// Executes the process for one automaton.
nbautils::SWA<std::string> PerformConstruction(nbautils::SWA<std::string> automaton, const Options& options) {
    if (options.make_complete)
        nbautils::make_complete(automaton);

    if (options.minimize_before)
        MinimizePA(&automaton);

    ScheweAutomaton(&automaton);

    if (options.minimize_after)
        MinimizePA(&automaton);

    return automaton;
}


// Main
int main(int argc, char** argv) {
    const Options options = ParseArgs(argc, argv);

    if (!std::ifstream(options.input_file)) {
        std::cerr << "Input file " << options.input_file << " does not exist or cannot be accessed." << std::endl;
        return 1;
    }

    std::vector<nbautils::SWA<std::string>::uptr> input_automata = nbautils::parse_hoa(options.input_file);

    for (const nbautils::SWA<std::string>::uptr& aut : input_automata) {
        const nbautils::SWA<std::string> out = PerformConstruction(*aut, options);
        nbautils::print_hoa(out, std::cout);
        std::cout << std::flush;
    }

    return 0;
}