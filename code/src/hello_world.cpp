
#include <iostream>
#include "swa.hh"
#include "schewe10/equivalence_relation.h"
#include "schewe10/almost_equivalent_states.h"
#include "schewe10/schewe_automaton.h"
#include <chrono>

nbautils::SWA<int> sven_example() {
    nbautils::SWA<int> swa;
    swa.add_state(0);
    swa.add_state(1);
    swa.add_state(2);
    swa.add_state(3);
    swa.add_state(4);
    swa.set_init(std::vector<nbautils::state_t>{0});
    swa.set_aps(std::vector<std::string>{"a"});
    swa.set_accs(4, std::vector<nbautils::acc_t> {0});
    swa.set_succs(0, 0, std::vector<nbautils::state_t> {1});
    swa.set_succs(0, 1, std::vector<nbautils::state_t> {2});
    swa.set_succs(1, 0, std::vector<nbautils::state_t> {0});
    swa.set_succs(1, 1, std::vector<nbautils::state_t> {4});
    swa.set_succs(2, 0, std::vector<nbautils::state_t> {3});
    swa.set_succs(2, 1, std::vector<nbautils::state_t> {3});
    swa.set_succs(3, 0, std::vector<nbautils::state_t> {2});
    swa.set_succs(3, 1, std::vector<nbautils::state_t> {4});
    swa.set_succs(4, 0, std::vector<nbautils::state_t> {3});
    swa.set_succs(4, 1, std::vector<nbautils::state_t> {3});
    return swa;
}

long MeasureMS(std::function<void()> f) {
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    f();
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
}

int main() {
    const nbautils::SWA<int> swa = sven_example();
    const nbautils::SWA<int> schewe = ScheweAutomaton(swa);

    long time = MeasureMS([&swa,&schewe]() {ScheweAutomaton(swa);});

    std::cout << "Hello World " << std::endl;
    std::cout << time << " ms" << std::endl;
    return 0;
}
