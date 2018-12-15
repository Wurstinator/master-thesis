
#include "nbautils_post.h"
#include <unordered_set>
#include <regex>

namespace tollk {

using namespace automaton;

std::set<unsigned int> ParseStateLabel(const std::string& label);

// An example for a label is
// "N: {}	(AC: {}, AB: {}):3	D: ()	M: ({7,12,15}:1, {14}:2, {0,1,2,3,4,5,6,8,9,10,11,13,16,18,19}:0)"
// The numbers within the curly braces {} mark states from the non-deterministic automaton. Two states are
// language-equivalent if the sets of all those numbers are the same.
EquivalenceRelation<state_t> NbautilsEquivalence(const std::map<state_t, std::string>& state_labels) {
    std::map<std::set<unsigned int>, std::vector<state_t>> set_state_map;
    for (const std::pair<const state_t, std::string>& kv_pair : state_labels)
        set_state_map[ParseStateLabel(kv_pair.second)].push_back(kv_pair.first);

    EquivalenceRelation<state_t> relation;
    for (const std::pair<const std::set<unsigned int>, std::vector<state_t>>& kv_pair : set_state_map) {
        const state_t p = kv_pair.second.front();
        for (state_t q : kv_pair.second)
            relation.AddConnection(p, q);
    }

    return relation;
}

unsigned int atoui(const std::string& s) {
    std::stringstream ss;
    ss << s;
    unsigned int x;
    ss >> x;
    return x;
}

std::set<unsigned int> ParseStateLabel(const std::string& label) {
    const std::regex regex(R"!!(\{.*?\})!!");
    const std::vector<std::smatch> matches(std::sregex_iterator(label.begin(), label.end(), regex), std::sregex_iterator());

    std::set<unsigned int> result;
    for (const std::smatch& match : matches) {
        const std::string match_string = match.str();
        const std::regex regex2(R"!!(\d+)!!");
        const std::vector<std::smatch> numbers(std::sregex_iterator(match_string.begin(), match_string.end(), regex2), std::sregex_iterator());
        for (const std::smatch& number_match : numbers)
            result.insert(atoui(number_match.str()));
    }
    return result;
}

}