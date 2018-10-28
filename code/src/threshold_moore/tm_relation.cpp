
#include "tm_relation.h"
#include "moore_ltk.h"

namespace tollk {

using namespace automaton;

EquivalenceRelation<state_t> TMRelation(const DPA& automaton, const EquivalenceRelation<state_t>& R) {
    return EquivalenceRelation<state_t>::Intersection(R, AllMoore_LTK(automaton));
}

}