
/**
 * Read in a transition-based DPA from a HOA file and output it again as an equivalent state-based DPA.
 */


#include <args.hxx>
#include "automaton/hoa/hoa_io.h"
#include <fstream>
#include <iostream>

using namespace tollk::automaton;

std::string ParseFilename(int argc, char** argv) {
    args::ArgumentParser parser("Performs a construction on a given DPA.");
    args::CompletionFlag completion(parser, {"complete"});
    args::HelpFlag help_flag(parser, "help", "Display this help menu.", {'h', "help"});
    args::ValueFlag<std::string> input_file_flag(parser, "input_file", "HOA file that contains the input DPA.", {'A', "automaton"}, args::Options::Required);

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

    return input_file_flag.Get();
}


tollk::automaton::NPA NPAFromHoa(const std::string& filename) {
    std::ifstream file(filename);
    return tollk::automaton::hoa::NPAFromHOA(&file);
}


int main(int argc, char** argv) {
    const std::string filename = ParseFilename(argc, argv);

    if (!std::ifstream(filename)) {
        std::cerr << "Input file " << filename << " does not exist or cannot be accessed." << std::endl;
        return 1;
    }

    // Read the input.
    std::ifstream input_file(filename);
    std::map<std::pair<state_t, parity_label_t>, state_t> pair_indices;
    std::map<state_t, std::string> state_labels;
    const tollk::automaton::DPA dpa = tollk::automaton::hoa::TransitionBasedDPAFromHOA(&input_file, &pair_indices, &state_labels);

    // State labels have to be adapted to the new states.
    std::map<state_t, std::string> new_state_labels;
    for (const std::pair<const std::pair<state_t, parity_label_t>, state_t>& kv_pairs : pair_indices)
        new_state_labels[kv_pairs.second] = state_labels.at(kv_pairs.first.first);

    tollk::automaton::hoa::ToHOA(dpa, &std::cout, new_state_labels);
    std::cout << std::flush;

    return 0;
}