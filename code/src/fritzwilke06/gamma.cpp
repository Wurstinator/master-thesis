
#include "gamma.h"

namespace tollk {


using namespace automaton;

std::optional<parity_label_t> Gamma_de(parity_label_t i, parity_label_t j, std::optional<parity_label_t> k_or_not) {
    const bool i_even = (i % 2 == 0);
    const bool j_even = (j % 2 == 0);

    if (k_or_not) {
        const parity_label_t k = k_or_not.value();
        if (!i_even && !j_even && i <= j && i <= k)
            return std::optional<parity_label_t>();
        if (i_even && j_even && j <= i && j <= k)
            return std::optional<parity_label_t>();
        if (!i_even && j_even && (i <= k || j <= k))
            return std::optional<parity_label_t>();
        return std::min(std::min(i, j), k);
    } else {
        if (!i_even && !j_even && i <= j)
            return std::optional<parity_label_t>();
        if (i_even && j_even && j <= i)
            return std::optional<parity_label_t>();
        if (!i_even && j_even)
            return std::optional<parity_label_t>();
        return std::min(i, j);
    }
}


}