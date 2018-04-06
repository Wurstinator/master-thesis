
#include <iostream>
#include "swa.hh"
#include "schewe10/equivalence_relation.h"
#include "schewe10/almost_equivalent_states.h"

int main() {
    nbautils::SWA<int> swa;
    swa.add_state(0);
    swa.set_init(std::vector<nbautils::state_t>{0});
    swa.set_aps(std::vector<std::string>{"a"});
    swa.set_succs(0, 0, std::vector<nbautils::state_t> {0});

    EquivalenceRelation<nbautils::state_t> foo = PriorityAlmostEquivalence(swa);
    std::cout << "Hello World " << std::endl;
    return 0;
}
