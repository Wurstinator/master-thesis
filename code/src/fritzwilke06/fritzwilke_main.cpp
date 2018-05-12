
#include <iostream>
#include <args.hxx>
#include <io.hh>
#include <range/v3/algorithm/min.hpp>
#include <range/v3/view/transform.hpp>
#include "delayed_simulation.h"
#include "../automaton/nbautils_bridge.h"
#include "../automaton/util.h"

// Given an det. parity automaton, performs the Schewe construction and outputs the result in the HOA format.
// Arguments:
//   -A  : Path to the file in which the DPA is stored in HOA format.
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


// Given a parity automaton and a range of states, returns the smallest priority among them.
template <typename Rng>
tollk::automaton::parity_label_t MinParity(const tollk::automaton::ParityAutomaton& automaton, const Rng& range) {
    const auto get_label = [&automaton](tollk::automaton::state_t q) {return automaton.GetLabel(q);};
    return ranges::v3::min(ranges::v3::view::transform(range, get_label));
}


// Executes the process for one automaton.
tollk::automaton::DPA PerformConstruction(tollk::automaton::DPA dpa, const Options& options) {
    const tollk::EquivalenceRelation<tollk::automaton::state_t> desim_relation = tollk::DelayedSimulationEquivalence(dpa);
    tollk::automaton::NPA npa = tollk::automaton::NPA::FromDPA(dpa);
    const std::function<tollk::automaton::parity_label_t(const tollk::EquivalenceRelation<tollk::automaton::state_t>::EquivClass&)> f = std::bind(&MinParity<tollk::EquivalenceRelation<tollk::automaton::state_t>::EquivClass>, npa, std::placeholders::_1);
    tollk::automaton::QuotientAutomaton(&npa, desim_relation, f);
    return tollk::automaton::DPA::FromNPA(npa);
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
