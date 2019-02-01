
#include "../is_specialization_base_of.h"


inline void ToHOA_WriteAtomicPropositions(unsigned int aps, std::ostream* ostream) {
    (*ostream) << "AP: " << aps << ' ';
    for (unsigned int i = 1; i <= aps; ++i)
        (*ostream) << "\"a" << i << "\" ";
    (*ostream) << '\n';
}

inline void ToHOA_WriteParityAcceptance(parity_label_t max_prio, std::ostream* ostream) {
    (*ostream) << "acc-name: parity min even " << max_prio+1 << '\n';
    (*ostream) << "Acceptance: " << max_prio+1 << ' ';
    for (unsigned int i = 0; i <= max_prio; ++i) {
        (*ostream) << (i % 2 == 0 ? "Inf" : "Fin") << '(' << i << ")";
        if (i < max_prio)
            (*ostream) << ' ' << (i % 2 == 0 ? '|' : '&') << " (";
    }
    for (unsigned int i = 0; i < max_prio; ++i)
        (*ostream) << ')';
    (*ostream) << '\n';
}

inline void ToHOA_WriteTransition(unsigned int symbol_size, symbol_t sym, unsigned int to, std::ostream* ostream) {
    (*ostream) << '[';
    std::vector<bool> bits;
    for (unsigned int i = 0; i < symbol_size; ++i)
        bits.push_back(!!((sym >> i) & 1));
    for (unsigned int i = 0; i < bits.size(); ++i) {
        if (!bits[i])
            (*ostream) << '!';
        (*ostream) << i;
        if (i + 1 < bits.size())
            (*ostream) << " & ";
    }
    (*ostream) << "] " << to << '\n';
}

template <typename AutomatonT>
void ToHOA(const AutomatonT& automaton, std::ostream* ostream) {
    const std::map<state_t, std::string> empty_map;
    ToHOA(automaton, ostream, empty_map);
}

template <typename AutomatonT>
void ToHOA(const AutomatonT& automaton, std::ostream* ostream, const std::map<state_t, std::string>& state_labels) {
    constexpr bool is_parity_automaton = IsParityAutomaton<AutomatonT>();
    constexpr bool is_transition_automaton = IsTransitionAutomaton<AutomatonT>();
    constexpr bool is_deterministic_automaton = IsDeterministicAutomaton<AutomatonT>();
    static_assert(IsFiniteAutomaton<AutomatonT>());

    // State IDs have to be "normalized" i.e. continious IDs from 0 to n.
    std::map<state_t, unsigned int> state_indices;
    for (unsigned int i = 0; i < automaton.States().size(); ++i)
        state_indices[automaton.States()[i]] = i;

    // Count the number of priorities, if we are dealing with a parity automaton.
    parity_label_t max_prio = 0;
    if (is_parity_automaton) {
        const auto priorities = automaton.AllLabels();
        max_prio = *std::max_element(priorities.begin(), priorities.end());
    }

    // Write header.
    (*ostream) << "HOA: v1\n";
    (*ostream) << "States: " << automaton.States().size() << '\n';
    (*ostream) << "Start: " << state_indices[automaton.InitialState()] << '\n';
    if (is_transition_automaton)
        ToHOA_WriteAtomicPropositions(automaton.atomicPropositions, ostream);
    if (is_parity_automaton)
        ToHOA_WriteParityAcceptance(max_prio, ostream);
    (*ostream) << "properties: ";
    if (is_transition_automaton)
        (*ostream) << "trans-labels ";
    if (is_parity_automaton)
        (*ostream) << "state-acc colored ";
    if (is_deterministic_automaton)
        (*ostream) << "complete deterministic ";
    (*ostream) << '\n';

    // Write body.
    (*ostream) << "--BODY--\n";
    for (unsigned int p = 0; p < automaton.States().size(); ++p) {
        (*ostream) << "State: " << p;
        if (state_labels.find(automaton.States()[p]) != state_labels.end()) {
            (*ostream) << " \"";
            (*ostream) << state_labels.at(automaton.States()[p]);
            (*ostream) << '"';
        }
        if (is_parity_automaton)
            (*ostream) << " {" << automaton.GetLabel(automaton.States()[p]) << '}';
        (*ostream) << '\n';

        if (is_transition_automaton)
            for (symbol_t s : automaton.Symbols())
                for (state_t q : automaton.Successors(automaton.States()[p], s))
                    ToHOA_WriteTransition(automaton.atomicPropositions, s, state_indices[q], ostream);
    }

    // Done.
    (*ostream) << "--END--";
}


template <typename AutomatonT>
std::string AutTypeName_() {
    constexpr bool is_finite_automaton = std::is_base_of<FiniteAutomaton, AutomatonT>::value;
    constexpr bool is_parity_automaton = std::is_base_of<ParityAutomaton, AutomatonT>::value;
    constexpr bool is_transition_automaton = is_specialization_base_of<TransitionAutomaton, AutomatonT>::value;
    constexpr bool is_deterministic_automaton = std::is_base_of<DeterministicAutomaton, AutomatonT>::value;
    static_assert(is_finite_automaton);

    if (is_transition_automaton) {
        if (is_deterministic_automaton) {
            if (is_parity_automaton) {
                return "DPA";
            } else {
                return "DeterministicAutomaton";
            }
        } else {
            if (is_parity_automaton) {
                return "NPA";
            } else {
                return "NondeterministicAutomaton";
            }
        }
    } else {
        if (is_parity_automaton) {
            return "ParityAutomaton";
        } else {
            return "FiniteAutomaton";
        }
    }
}


template <typename AutomatonT>
std::string ToCPPCode(const AutomatonT& automaton) {
    constexpr bool is_finite_automaton = std::is_base_of<FiniteAutomaton, AutomatonT>::value;
    constexpr bool is_parity_automaton = std::is_base_of<ParityAutomaton, AutomatonT>::value;
    constexpr bool is_transition_automaton = is_specialization_base_of<TransitionAutomaton, AutomatonT>::value;
    constexpr bool is_deterministic_automaton = std::is_base_of<DeterministicAutomaton, AutomatonT>::value;
    static_assert(is_finite_automaton);

    // Initialization.
    std::stringstream result;
    result << AutTypeName_<AutomatonT>() << " automaton";
    if (is_transition_automaton)
        result << "(" << static_cast<int>(automaton.atomicPropositions) << ")";
    result << ";\n";

    // Setup states.
    for (state_t q : automaton.States())
        result << "automaton.AddState(" << q << ");\n";
    result << "automaton.SetInitialState(" << automaton.InitialState() << ");\n";

    // Setup labels.
    if (is_parity_automaton)
        for (state_t q : automaton.States())
            result << "automaton.SetLabel(" << q << ", " << automaton.GetLabel(q) << ");\n";

    // Setup transitions.
    if (is_transition_automaton) {
        for (state_t p : automaton.States()) {
            for (int s : automaton.Symbols()) {
                for (state_t q : automaton.Successors(p, s)) {
                    if (is_deterministic_automaton)
                        result << "automaton.SetSucc(" << p << ", " << s << ", " << q << ");\n";
                    else
                        result << "automaton.AddSucc(" << p << ", " << s << ", " << q << ");\n";
                }
            }
        }
    }

    return result.str();
}