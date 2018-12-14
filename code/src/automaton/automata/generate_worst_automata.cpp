
#include <args.hxx>
#include <iostream>
#include <fstream>
#include "../hoa/hoa_io.h"
#include "max_michelle.h"

struct Options {
    unsigned int parameter_n = 0;
    std::optional<std::string> output_file;
};

// Parser
Options ParseArgs(int argc, char** argv) {
    args::ArgumentParser parser("Performs a construction on a given DPA.");
    args::CompletionFlag completion(parser, {"complete"});
    args::HelpFlag help_flag(parser, "help", "Display this help menu.", {'h', "help"});
    args::ValueFlag<unsigned int> parameter_n_flag(parser, "n", "Parameter 'n'.", {"pn"});
    args::ValueFlag<std::string> output_file_flag(parser, "output_file", "Output target file. Default is stdout.", {'O', "output"});

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

    Options options;
    if (parameter_n_flag)
        options.parameter_n = parameter_n_flag.Get();
    if (output_file_flag)
        options.output_file = output_file_flag.Get();
    return options;
}


// Main
int main(int argc, char** argv) {
    const Options options = ParseArgs(argc, argv);
    const tollk::automaton::DPA dpa = tollk::automaton::MaxMichelleAutomaton(options.parameter_n);

    if (options.output_file) {
        std::ofstream file(*options.output_file);
        tollk::automaton::hoa::ToHOA(dpa, &file);
    } else {
        tollk::automaton::hoa::ToHOA(dpa, &std::cout);
        std::cout << std::flush;
    }

    return 0;
}