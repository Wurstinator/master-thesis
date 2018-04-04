
#include <iostream>
#include "swa.hh"
#include "merge_sccs.h"

int main(int argc, char** argv) {
    nbautils::SWA<int> swa;
    nbautils::SWA<std::set<int>> merged = MergeSCCs(swa);
    std::cout << "Hello World " << merged.num_states() << std::endl;
    return 0;
}
