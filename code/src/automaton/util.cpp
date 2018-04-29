
#include "util.h"

using namespace tollk;
using namespace automaton;

void tollk::automaton::MergeSCCs(NondeterministicAutomaton* automaton, SCCCollection* sccs) {
    SCCCollection sccs_ = StronglyConnectedComponents(*automaton);
    for (const SCCCollection::SCC& scc : sccs_.sccs) {
        automaton->MergeStates(scc);
    }
    if (sccs != nullptr)
        *sccs = std::move(sccs_);
}


DeterministicAutomaton ProductAutomaton(const DeterministicAutomaton& automaton1, const DeterministicAutomaton& automaton2) {
    assert(automaton1.atomicPropositions == automaton2.atomicPropositions);

    DeterministicAutomaton product(automaton1.atomicPropositions);
}