
#include "gamma.h"

namespace tollk {


using namespace automaton;


bool lte_checkmark(const std::optional<parity_label_t>& lhs, const std::optional<parity_label_t>& rhs) {
    return lhs == rhs || lt_checkmark(lhs, rhs);
}

bool lt_checkmark(const std::optional<parity_label_t>& lhs, const std::optional<parity_label_t>& rhs) {
    if (!lhs)
        return false;
    else
        return !rhs || lhs.value() < rhs.value();
}

std::optional<parity_label_t> Gamma_de(parity_label_t i, parity_label_t j, const std::optional<parity_label_t>& k_or_not) {
    const bool i_even = (i % 2 == 0);
    const bool j_even = (j % 2 == 0);
    const bool j_better_than_i = (j_even && j <= i) || (!i_even && i <= j) || (j_even && !i_even);
    const parity_label_t k = k_or_not.value_or(std::max(i, j));
    if (!i_even && i <= k && j_better_than_i)
        return std::nullopt;
    else if (j_even && j <= k && j_better_than_i)
        return std::nullopt;
    else
        return std::min(std::min(i, j), k);
}


}