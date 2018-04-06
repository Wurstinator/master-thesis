
#pragma once

#include "swa.hh"
#include <utility>

// Conversion functions from the index of a state in the product automaton to the indices of its components, and back.
// The second parameter is the number of states in the second automaton.
std::pair<nbautils::state_t, nbautils::state_t> ProdID2CompID(nbautils::state_t q, unsigned int nstates2) {
    return std::pair<nbautils::state_t, nbautils::state_t>(q / nstates2, q % nstates2);
};

nbautils::state_t CompID2ProdID(nbautils::state_t q1, nbautils::state_t q2, unsigned int nstates2) {
    return q1 * nstates2 + q2;
};

// Given two automata, creates a product automaton of the two, meaning that states are now pairs of states
// and transitions are applied component-wise. The acceptance properties are left unset. A state is initial iff
// both components are initial in their respective automaton. It is assumed that the the automata use the same
// alphabet.
// Important! The input automata have to be normalized to allow for easy conversion between state indices within the
// function.
template <typename TagT1, typename TagT2>
nbautils::SWA<std::pair<TagT1, TagT2>> ProductAutomaton(const nbautils::SWA<TagT1>& automaton1, const nbautils::SWA<TagT2>& automaton2) {
    assert(automaton1.get_aps() == automaton2.get_aps());

    // Create the automaton object.
    nbautils::SWA<std::pair<TagT1, TagT2>> product;
    product.set_name("product " + automaton1.get_name() + " " + automaton2.get_name());
    product.set_aps(automaton1.get_aps());

    // Add states to the automaton.
    for (nbautils::state_t q1 : automaton1.states()) {
        for (nbautils::state_t q2 : automaton2.states()) {
            product.add_state(CompID2ProdID(q1, q2, automaton2.num_states()));
        }
    }

    // Assign tags to the new states.
    for (nbautils::state_t q : product.states()) {
        const auto ids = ProdID2CompID(q, automaton2.num_states());
        TagT1 tag1;
        if (automaton1.tag->hasi(ids.first))
            tag1 = automaton1.tag->geti(ids.first);
        else if (std::is_default_constructible<TagT1>::value)
            tag1 = TagT1();
        else
            continue;

        TagT1 tag2;
        if (automaton2.tag->hasi(ids.second))
            tag2 = automaton2.tag->geti(ids.second);
        else if (std::is_default_constructible<TagT2>::value)
            tag2 = TagT2();
        else
            continue;

        product.tag->put(std::make_pair(std::move(tag1), std::move(tag2)), q);
    }

    // Set initial states.
    std::vector<nbautils::state_t> initial_states;
    for (nbautils::state_t q1 : automaton1.get_init()) {
        for (nbautils::state_t q2 : automaton2.get_init()) {
            initial_states.push_back(CompID2ProdID(q1, q2, automaton2.num_states()));
        }
    }
    product.set_init(std::move(initial_states));

    // Assign transitions.
    for (nbautils::state_t q : product.states()) {
        for (nbautils::sym_t sym = 0; sym < product.num_syms(); ++sym) {
            std::vector<nbautils::state_t> successors;
            const auto ids = ProdID2CompID(q, automaton2.num_states());
            for (nbautils::state_t q1 : automaton1.succ(ids.first, sym)) {
                for (nbautils::state_t q2 : automaton2.succ(ids.second, sym)) {
                    successors.push_back(CompID2ProdID(q1, q2, automaton2.num_states()));
                }
            }
            product.set_succs(q, sym, std::move(successors));
        }
    }

    return product;
};