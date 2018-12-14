
#include "max_michelle.h"
#include <cmath>

namespace tollk {
namespace automaton {


DPA MaxMichelleAutomaton(unsigned int n) {
    // Calculate the needed APs
    const unsigned int logn = static_cast<unsigned int>(std::ceil(std::log(n + 1) / std::log(2)));

    // Set up basic automaton.
    DPA automaton(logn + 1);
    for (unsigned int i = 0; i <= n + 1; ++i) {
        automaton.AddState(i);
        automaton.SetLabel(i, i == 0 ? 0 : 1);
    }
    // 0, ..., n are the number states. n+1 is "q_0$. n+2 is the sink state.
    const state_t sink = n+2;
    automaton.SetInitialState(n + 1);

    // Set up transitions.
    for (symbol_t s : automaton.Symbols()) {
        const bool sign = (s & (1u << logn)) != 0;
        const unsigned int number = s & ((1u << logn) - 1);
        if (number > n) {
            for (state_t q : automaton.States())
                automaton.SetSucc(q, s, sink);
        } else {
            for (state_t q : automaton.States()) {
                if (q == automaton.InitialState()) {
                    automaton.SetSucc(q, s, sign ? number : sink);
                } else if (q == number) {
                    automaton.SetSucc(q, s, sign ? automaton.InitialState() : sink);
                } else {
                    automaton.SetSucc(q, s, q == 0 ? sink : q);
                }
            }
        }
    }

    automaton.AddState(sink);
    for (symbol_t s : automaton.Symbols())
        automaton.SetSucc(sink, s, sink);

    return automaton;
}

}
}