
// Prints some general information about a given HOA in JSON format.


#include "../../json/single_include/nlohmann/json.hpp"
#include <args.hxx>
#include <iostream>
#include <fstream>
#include "automaton/parity.h"
#include "automaton/util.h"
#include "automaton/hoa/hoa_io.h"

using namespace tollk;
using namespace automaton;

std::string FilenameFromArgs(int argc, char** argv) {
    args::ArgumentParser parser("Performs a construction on a given DPA.");
    args::CompletionFlag completion(parser, {"complete"});
    args::HelpFlag help_flag(parser, "help", "Display this help menu.", {'h', "help"});
    args::Positional<std::string> input_file_flag(parser, "input_file", "HOA file that contains the input DPA.",
                                                  args::Options::Required);

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

NPA ReadNPA(const std::string& filename) {
    std::ifstream file(filename);
    return hoa::NPAFromHOA(&file);
}

DPA ReadDPA(const std::string& filename) {
    std::ifstream file(filename);
    return hoa::DPAFromHOA(&file);
}

EquivalenceRelation<state_t> LanguageClasses(const NPA&);

bool IsPriorityNormalized(const NPA&);

bool IsMooreMinimized(const NPA&);

EquivalenceRelation<state_t> LanguageClasses(const DPA& dpa) {
    return LanguageEquivalentStates(dpa);
}

bool IsPriorityNormalized(const DPA& dpa) {
    const std::map<state_t, parity_label_t> normalized_prios = NormalizePriorities(dpa);
    for (state_t q : dpa.States())
        if (dpa.GetLabel(q) != normalized_prios.at(q))
            return false;
    return true;
}

bool IsMooreMinimized(const DPA& dpa) {
    DPA copy = dpa;
    Hopcroft(&copy);
    return dpa.States().size() == copy.States().size();
}

template<typename AutomatonT>
nlohmann::json AnalyzeAutomaton(const AutomatonT& automaton) {
    static_assert(IsFiniteAutomaton<AutomatonT>());
    static_assert(IsParityAutomaton<AutomatonT>());
    static_assert(IsTransitionAutomaton<AutomatonT>());
    constexpr bool is_deterministic_automaton = IsDeterministicAutomaton<AutomatonT>();

    nlohmann::json json;
    json["states"] = automaton.States().size();
    json["priorities"] = automaton.AllLabels().size();
    json["is_deterministic"] = is_deterministic_automaton;
    json["sccs"] = StronglyConnectedComponents(automaton).sccs.size();
    if (is_deterministic_automaton) {
        const EquivalenceRelation<state_t> language = LanguageClasses(automaton);
        json["nontrivial_language_classes"] = std::count_if(language.Classes().begin(), language.Classes().end(),
                                                            [](const EquivalenceRelation<state_t>::EquivClass& c) {
                                                                return c.size() > 1;
                                                            });
        json["average_language_class_size"] = std::accumulate(language.Classes().begin(), language.Classes().end(), 0,
                                                              [](size_t i,
                                                                 const EquivalenceRelation<state_t>::EquivClass& c) {
                                                                  return i + c.size();
                                                              }) / static_cast<double>(language.Classes().size());
        json["is_priority_minimized"] = IsPriorityNormalized(automaton);
        json["is_moore_minimized"] = IsMooreMinimized(automaton);
    }
    return json;
}

int main(int argc, char** argv) {
    const std::string input_file = FilenameFromArgs(argc, argv);
    const NPA npa = ReadNPA(input_file);
    nlohmann::json j;
    if (npa.IsDeterministic()) {
        j = AnalyzeAutomaton(ReadDPA(input_file));
    } else {
        j = AnalyzeAutomaton(npa);
    }
    j["filename"] = input_file;
    std::cout << j << std::endl;
    return 0;
}