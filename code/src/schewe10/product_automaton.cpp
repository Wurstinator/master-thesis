
#include "product_automaton.h"

std::pair<nbautils::state_t, nbautils::state_t> ProdID2CompID(nbautils::state_t q, unsigned int nstates2) {
    return std::pair<nbautils::state_t, nbautils::state_t>(q / nstates2, q % nstates2);
};

nbautils::state_t CompID2ProdID(nbautils::state_t q1, nbautils::state_t q2, unsigned int nstates2) {
    return q1 * nstates2 + q2;
};