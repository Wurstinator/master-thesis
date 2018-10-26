
#include <fstream>
#include <string>
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include "visit_graph.h"
#include "../automaton/hoa/hoa_io.h"
#include "path_refinement.h"
#include "../automaton/util.h"

using namespace tollk;
using namespace automaton;

// Reads a filename from the first passed cmd argument and from that file reads a DPA in hoa format.
DPA ReadInput(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Please specify automaton input file." << std::endl;
        exit(1);
    }

    const std::string filename(argv[1]);

    if (!std::ifstream(filename)) {
        std::cerr << "Input file " << filename << " does not exist or cannot be accessed." << std::endl;
        exit(1);
    }

    std::cout << filename << '\n';
    std::ifstream file(filename);
    return hoa::DPAFromHOA(&file);
}

// Computes language equivalent classes of an automaton, removes trivial ones, and returns the rest sorted.
std::vector<std::set<state_t>> Lambdas(const DPA& dpa) {
    const EquivalenceRelation<state_t> R = LanguageEquivalentStates(dpa);
    std::vector<std::set<state_t>> lambdas = R.Classes();
    const auto is_trivial = [](const std::set<state_t>& l) { return l.size() < 2; };
    lambdas.erase(std::remove_if(lambdas.begin(), lambdas.end(), is_trivial), lambdas.end());
    std::sort(lambdas.begin(), lambdas.end());
    return lambdas;
}

// Takes in a list of equivalence classes in a certain order and prints how many states this order removes the automaton.
std::string IteratePermutation(DPA dpa, const std::vector<std::set<state_t>>& lambdas) {
    std::stringstream result;
    std::vector<unsigned long> sizes;
    for (const std::set<state_t>& lambda : lambdas) {
        result << lambdas.size() << ' ';
        sizes.push_back(dpa.States().size());
        PathRefinementMerge_VIS(&dpa, lambda);
    }
    sizes.push_back(dpa.States().size());
    result << " :  ";
    for (unsigned long s : sizes)
        result << s << ' ';
    return result.str();
}

int main(int argc, char** argv) {
    const DPA automaton = ReadInput(argc, argv);
    std::vector<std::set<state_t>> lambdas = Lambdas(automaton);
    if (lambdas.empty())
        return 0;

    // For each permutation of the classes, perform complete PR reduction and print.
    do {
        std::cout << IteratePermutation(automaton, lambdas) << std::endl;
    } while (std::next_permutation(lambdas.begin(), lambdas.end()));
    std::cout << std::endl;
}