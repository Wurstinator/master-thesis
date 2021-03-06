
#include "construction_main.h"
#include <fstream>
#include <iostream>
#include "automaton/hoa/hoa_io.h"

using namespace tollk;

std::unique_ptr<BaseOptions> ParseArgs(int argc, char** argv, ConstructionExecutor* specialization) {
    args::ArgumentParser parser("Performs a construction on a given DPA.");
    args::CompletionFlag completion(parser, {"complete"});
    args::HelpFlag help_flag(parser, "help", "Display this help menu.", {'h', "help"});
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
    options->input_file = input_file_flag.Get();
    return options;
}


int main(int argc, char** argv) {
    const std::unique_ptr<ConstructionExecutor> construction_executor = CreateConstructionExecutor();
    const std::unique_ptr<const BaseOptions> options = ParseArgs(argc, argv, construction_executor.get());

    if (!std::ifstream(options->input_file)) {
        std::cerr << "Input file " << options->input_file << " does not exist or cannot be accessed." << std::endl;
        return 1;
    }

    {
        std::ifstream input_file(options->input_file);
        construction_executor->LoadInput(&input_file);
    }
    const tollk::automaton::DPA out_dpa = construction_executor->PerformConstruction(*options);

    tollk::automaton::hoa::ToHOA(out_dpa, &std::cout);
    std::cout << std::flush;

    return 0;
}