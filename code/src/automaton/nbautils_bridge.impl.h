
#include <algorithm>

namespace tollk {
namespace automaton {

// Given a SWA and a set of parities, returns an equivalent parity in min-even format. If the set contains multiple
// values, only consider the "most dominant".
template <typename T,template <typename... Args> class S>
parity_label_t ConvertFromSWAParity(const nbautils::SWA<T, S>& swa, const std::vector<nbautils::acc_t>& acceptance) {
    if (swa.get_acceptance() == nbautils::Acceptance::BUCHI)
        return acceptance.empty() ? 1 : 0;
    if (swa.get_acceptance() != nbautils::Acceptance::PARITY)
        throw "ConvertFromSWAParity : swa uses imcompatible acceptance";

    // Parity automaton.
    if (acceptance.empty()) {
        // If the acceptance set is empty, simply select the least dominant priority that occurs in the automaton.
        if (swa.get_patype() == nbautils::PAType::MIN_EVEN || swa.get_patype() == nbautils::PAType::MIN_ODD) {
            nbautils::acc_t* max_acc = nullptr;
            for (nbautils::state_t q : swa.states())
                for (nbautils::acc_t acc : swa.get_accs(q))
                    max_acc = (max_acc == nullptr || acc > *max_acc ? &acc : max_acc);
            return max_acc == nullptr ? 0 : *max_acc;
        } else {
            return 0;
        }
    }

    nbautils::acc_t even_parity;
    if (swa.get_patype() == nbautils::PAType::MIN_EVEN || swa.get_patype() == nbautils::PAType::MIN_ODD) {
        even_parity = *std::min_element(acceptance.begin(), acceptance.end());
    } else {
        const nbautils::acc_t dominant_acc = *std::max_element(acceptance.begin(), acceptance.end());
        nbautils::acc_t max_acc = dominant_acc;
        for (nbautils::state_t q : swa.states())
            for (nbautils::acc_t acc : swa.get_accs(q))
                max_acc = (acc > max_acc ? acc : max_acc);
        even_parity = max_acc - dominant_acc;
    }

    if (swa.get_patype() == nbautils::PAType::MAX_ODD || swa.get_patype() == nbautils::PAType::MIN_ODD)
        return even_parity + 1;
    else
        return even_parity;
}


template <typename T,template <typename... Args> class S>
NPA FromNbautils(const nbautils::SWA<T, S>& swa) {
    assert(swa.get_acceptance() == nbautils::BUCHI || swa.get_acceptance() == nbautils::PARITY);

    // Create NPA with states.
    NPA result(swa.get_aps().size());
    for (nbautils::state_t q : swa.states()) {
        result.AddState(q);
        result.SetLabel(q, ConvertFromSWAParity(swa, swa.get_accs(q)));
    }

    // Adapt initial state.
    if (swa.get_init().size() == 0) {
        result.SetInitialState(result.States().front());
    } else if (swa.get_init().size() == 1) {
        result.SetInitialState(swa.get_init().front());
    } else {
        // Find a free index that is not used as a state.
        state_t new_init = 0;
        while (result.HasState(new_init))
            new_init++;
        result.AddState(new_init);
        result.SetInitialState(new_init);

        // Add new transitions to the initial states in the SWA.
        for (nbautils::state_t swa_init : swa.get_init())
            for (symbol_t s : result.Symbols())
                result.AddSucc(new_init, s, swa_init);
    }

    // Copy transitions.
    for (nbautils::state_t q : swa.states()) {
        for (nbautils::sym_t s = 0; s < swa.num_syms(); ++s) {
            for (nbautils::state_t succ : swa.succ(q, s)) {
                result.AddSucc(q, s, succ);
            }
        }
    }

    return result;
}


}
}