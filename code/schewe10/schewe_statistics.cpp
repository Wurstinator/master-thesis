
#include <iostream>
#include <chrono>
#include <args.hxx>
#include "pa.hh"
#include "io.hh"
#include "schewe_automaton.h"
#include <optional>
#include <cstdlib>

// Given an det. parity automaton, performs the Schewe construction and outputs some statistics.
// Arguments:
//   -c  : Makes the automaton complete by introducing a new sink state.
//   -m  : Adds information to the output for efficiency with minimization before and/or after the construction.
//   -A  : Path to the file in which the DPA is stored in HOA format.
// Output format:
// If -m is NOT given: num_of_input_states,time_taken_in_ms,num_of_output_states
// If -m is given: num_of_input_states,time1,outnum1,time2,outnum2,time3,outnum3,time4,outnum4
//      where time(i) and outnum(i) are the stats of the construction only / minimization + construction /
//                                                          construction + min / min + constr + min
int main(int argc, char** argv);


// Parses the command line options.
struct Options {
    bool make_complete;
    bool minimize;
    bool hopcroft;
    bool remove_unreachable;
    std::string input_file;
};

Options ParseArgs(int argc, char** argv) {
    args::ArgumentParser parser("Performs the Schewe construction on a given DPA.");
    args::CompletionFlag completion(parser, {"complete"});
    args::HelpFlag help_flag(parser, "help", "Display this h elp menu.", {'h', "help"});
    args::Flag make_complete_flag(parser, "make_complete", "Make the automaton complete by introducing a sink state.",
                                  {'c'});
    args::Flag minimize_flag(parser, "minimize",
                             "Minimize the automaton with Hopcraft before and/or after applying the construction.",
                             {'m'});
    args::Flag hopcroft_flag(parser, "hopcroft", "Perform the Hopcroft algorithm for minimization.", {"hop"});
    args::Flag remove_unreachable_flag(parser, "remove_unreachable", "Remove unreachable states for minimization.", {"ru"});
    args::ValueFlag<std::string> input_file_flag(parser, "input_file", "HOA file that contains the input DPA.",
                                                 {'A', "automaton"}, args::Options::Required);

    try {
        parser.ParseCLI(argc, argv);
    }
    catch (args::Completion e) {
        std::cout << e.what();
        std::exit(EXIT_SUCCESS);
    }
    catch (args::Help) {
        std::cout << parser;
        std::exit(EXIT_SUCCESS);
    }
    catch (args::ParseError e) {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        std::exit(EXIT_FAILURE);
    }

    Options options;
    options.make_complete = make_complete_flag.Get();
    options.minimize = minimize_flag.Get();
    options.hopcroft = hopcroft_flag.Get();
    options.remove_unreachable = remove_unreachable_flag.Get();
    options.input_file = input_file_flag.Get();
    return options;
}

// Executes the process for one automaton.
struct Statistics {
    size_t original_size;

    struct Efficiency {
        long milliseconds_taken;
        size_t new_size;
    };

    Efficiency only_schewe; // no minimization before or after the construction
    Efficiency minimized_before;
    Efficiency minimized_after;
    Efficiency minimized_before_and_after;
};

using TimePoint = std::chrono::steady_clock::time_point;

TimePoint TimeNow() {
    return std::chrono::steady_clock::now();
}

long TimeDiffMS(const TimePoint& a, const TimePoint& b) {
    return std::chrono::duration_cast<std::chrono::milliseconds>(b - a).count();
}

template<typename TagT>
void MinimizePA(nbautils::SWA<TagT>* automaton, const Options& options) {
    if (options.remove_unreachable) {
        const std::function<std::vector<nbautils::state_t>(nbautils::state_t)> get_succs =
                [automaton](nbautils::state_t q) {
                    return automaton->succ(q);
                };
        assert(automaton->get_init().size() == 1);
        const nbautils::state_t initial = automaton->get_init()[0];
        std::vector<nbautils::state_t> unreachable = nbautils::unreachable_states(automaton->states(), initial,
                                                                                  get_succs);
        std::sort(unreachable.begin(), unreachable.end());
        automaton->remove_states(unreachable);
    }
    if (options.hopcroft)
        nbautils::minimize_pa(*automaton);
}

Statistics PerformConstruction_NoMinimizations(nbautils::SWA<std::string> automaton, const Options& options) {
    Statistics data{};
    data.original_size = automaton.num_states();
    TimePoint time_begin = TimeNow();
    ScheweAutomaton(&automaton);
    TimePoint time_end = TimeNow();
    data.only_schewe.milliseconds_taken = TimeDiffMS(time_begin, time_end);
    data.only_schewe.new_size = automaton.num_states();
    return data;
}

Statistics PerformConstruction_Complete(nbautils::SWA<std::string> automaton, const Options& options) {
    Statistics data{};
    data.original_size = automaton.num_states();
    nbautils::SWA<std::string> minimized = automaton;

    TimePoint time1 = TimeNow();
    MinimizePA(&minimized, options);
    TimePoint time2 = TimeNow();
    ScheweAutomaton(&automaton);
    TimePoint time3 = TimeNow();
    ScheweAutomaton(&minimized);
    TimePoint time4 = TimeNow();
    data.only_schewe.new_size = automaton.num_states();
    data.only_schewe.milliseconds_taken = TimeDiffMS(time2, time3);
    data.minimized_before.new_size = minimized.num_states();
    data.minimized_before.milliseconds_taken = TimeDiffMS(time1, time2) + TimeDiffMS(time3, time4);

    MinimizePA(&automaton, options);
    TimePoint time5 = TimeNow();
    MinimizePA(&minimized, options);
    TimePoint time6 = TimeNow();
    data.minimized_after.new_size = automaton.num_states();
    data.minimized_after.milliseconds_taken = TimeDiffMS(time2, time3) + TimeDiffMS(time4, time5);
    data.minimized_before_and_after.new_size = minimized.num_states();
    data.minimized_before_and_after.milliseconds_taken =
            TimeDiffMS(time1, time2) + TimeDiffMS(time3, time4) + TimeDiffMS(time5, time6);

    return data;
}

// Evaluate the taken data
void EvaluateStats(const std::vector<Statistics>& data, bool complete) {
    for (const Statistics& stat : data) {
        std::cout << stat.original_size << ','
                  << stat.only_schewe.milliseconds_taken << ','
                  << stat.only_schewe.new_size;
        if (complete) {
            std::cout << ','
                      << stat.minimized_before.milliseconds_taken << ','
                      << stat.minimized_before.new_size << ','
                      << stat.minimized_after.milliseconds_taken << ','
                      << stat.minimized_after.new_size << ','
                      << stat.minimized_before_and_after.milliseconds_taken << ','
                      << stat.minimized_before_and_after.new_size;
        }
        std::cout << std::endl;
    }
}


// Main
int main(int argc, char** argv) {
    const Options options = ParseArgs(argc, argv);

    if (!std::ifstream(options.input_file)) {
        std::cerr << "Input file " << options.input_file << " does not exist or cannot be accessed." << std::endl;
        return 1;
    }

    if (!options.hopcroft && !options.remove_unreachable)
        std::cout << "Warning! No minimization techniques have been specified." << std::endl;

    std::vector<nbautils::SWA<std::string>::uptr> input_automata = nbautils::parse_hoa(options.input_file);

    std::vector<Statistics> stats;

    for (const nbautils::SWA<std::string>::uptr& aut : input_automata) {
        if (options.make_complete)
            nbautils::make_complete(*aut);
        const auto f = (options.minimize ? &PerformConstruction_Complete : &PerformConstruction_NoMinimizations);
        stats.push_back(f(*aut, options));
    }

    EvaluateStats(stats, options.minimize);

    return 0;
}