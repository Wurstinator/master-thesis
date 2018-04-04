
#include <iostream>
#include "swa.hh"
#include "merge_sccs.h"
#include "find_goal_sccs.h"
#include "equivalence_relation.h"

int main() {
    nbautils::SWA<int> swa;
    swa.add_state(0);
    swa.set_init(std::vector<nbautils::state_t>{0});
    swa.set_aps(std::vector<std::string>{"a"});
    swa.set_succs(0, 0, std::vector<nbautils::state_t> {0});
    nbautils::SWA<std::set<int>> merged = MergeSCCs(swa);
    std::vector<std::set<nbautils::state_t>> goal_sccs = FindGoalSCCs(swa);
    std::cout << "Hello World " << swa.num_states() << std::endl;
    EquivalenceRelation<int> foo;
    return 0;
}
