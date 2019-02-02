
cmake_minimum_required(VERSION 2.6)
cmake_policy(SET CMP0048 NEW)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -g")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DNDEBUG -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")

project(hello_world)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Catch2

set(CATCH2_DIR ./Catch2)
add_subdirectory(${CATCH2_DIR})

set(Catch2_HDRS
        ${CATCH2_DIR}/single_include/catch.hpp)

# range v3

set(RANGEV3_DIR ./range-v3)
add_subdirectory(${RANGEV3_DIR})

# json

set(JSON_DIR ./json)
add_subdirectory(${JSON_DIR})

# cpphoafparser

set(HOAFP_DIR ./cpphoafparser)
set(HOAFP_INCLUDE
        ${HOAFP_DIR}/include/cpphoafparser/parser/hoa_parser_exception.hh
        ${HOAFP_DIR}/include/cpphoafparser/parser/hoa_lexer.hh
        ${HOAFP_DIR}/include/cpphoafparser/parser/hoa_parser.hh
        ${HOAFP_DIR}/include/cpphoafparser/parser/hoa_parser_helper.hh
        ${HOAFP_DIR}/include/cpphoafparser/consumer/hoa_intermediate.hh
        ${HOAFP_DIR}/include/cpphoafparser/consumer/hoa_consumer.hh
        ${HOAFP_DIR}/include/cpphoafparser/consumer/hoa_consumer_exception.hh
        ${HOAFP_DIR}/include/cpphoafparser/consumer/hoa_consumer_print.hh
        ${HOAFP_DIR}/include/cpphoafparser/consumer/hoa_intermediate_trace.hh
        ${HOAFP_DIR}/include/cpphoafparser/consumer/hoa_intermediate_resolve_aliases.hh
        ${HOAFP_DIR}/include/cpphoafparser/consumer/hoa_consumer_null.hh
        ${HOAFP_DIR}/include/cpphoafparser/consumer/hoa_intermediate_check_validity.hh
        ${HOAFP_DIR}/include/cpphoafparser/util/acceptance_repository_standard.hh
        ${HOAFP_DIR}/include/cpphoafparser/util/implicit_edge_helper.hh
        ${HOAFP_DIR}/include/cpphoafparser/util/int_or_string.hh
        ${HOAFP_DIR}/include/cpphoafparser/util/dynamic_bitset.hh
        ${HOAFP_DIR}/include/cpphoafparser/util/acceptance_repository.hh
        ${HOAFP_DIR}/include/cpphoafparser/ast/boolean_expression.hh
        ${HOAFP_DIR}/include/cpphoafparser/ast/atom_acceptance.hh
        ${HOAFP_DIR}/include/cpphoafparser/ast/atom_label.hh
        )


# util
set(UTIL_INCLUDE
        src/util/equivalence_relation.h
        src/util/equivalence_relation.impl.h)
add_executable(test_equivalence_relation
        src/util/tests/test_main.cpp
        src/util/tests/test_equivalence_relation.cpp
        ${UTIL_INCLUDE}
        ${Catch2_HDRS})
target_include_directories(test_equivalence_relation PRIVATE ${CATCH2_DIR}/single_include)


# automaton module

set(AUTOMATON_FILES
        src/automaton/finite_automaton.h
        src/automaton/transition_automaton.h
        src/automaton/deterministic_automaton.h
        src/automaton/deterministic_automaton.cpp
        src/automaton/nondeterministic_automaton.h
        src/automaton/nondeterministic_automaton.cpp
        src/automaton/labelled_automaton.h
        src/automaton/labelled_automaton.impl.h
        src/automaton/util.h
        src/automaton/transition_util.h
        src/automaton/transition_util.impl.h
        src/automaton/transition_util.cpp
        src/automaton/parity_util.h
        src/automaton/parity_util.impl.h
        src/automaton/parity_util.cpp
        src/automaton/parity.cpp
        src/automaton/parity.h
        src/automaton/is_specialization_base_of.h
        src/automaton/almost_equivalent_states.h
        src/automaton/almost_equivalent_states.impl.h)

set(AUTOMATON_HOA_FILES
        src/automaton/hoa/hoa_io.h
        src/automaton/hoa/hoa_io.cpp
        src/automaton/hoa/hoa_consumer_debug.h
        src/automaton/hoa/pa_consumer.h
        src/automaton/hoa/pa_consumer.cpp
        src/automaton/hoa/dpa_consumer.h
        src/automaton/hoa/dpa_consumer.cpp
        src/automaton/hoa/npa_consumer.h
        src/automaton/hoa/npa_consumer.cpp
        src/automaton/hoa/boolean_evaluation.h
        src/automaton/hoa/hoa_io.impl.h
        src/automaton/hoa/transition_labeled_pa_consumer.cpp
        src/automaton/hoa/transition_labeled_pa_consumer.h
        src/automaton/hoa/nbautils_post.h
        src/automaton/hoa/nbautils_post.cpp)

set(AUTOMATON_INCLUDE ${AUTOMATON_FILES} ${UTIL_INCLUDE})
set(AUTOMATON_HOA_INCLUDE ${AUTOMATON_INCLUDE} ${AUTOMATON_HOA_FILES} ${HOAFP_INCLUDE})

add_library(automaton ${AUTOMATON_INCLUDE})
target_include_directories(automaton PRIVATE ${RANGEV3_DIR}/include)

add_library(automaton_with_hoa ${AUTOMATON_HOA_INCLUDE})
target_include_directories(automaton_with_hoa PRIVATE ${RANGEV3_DIR}/include ${HOAFP_DIR}/include)

set(AUTOMATON_TESTS
        src/automaton/tests/test_automata.cpp
        src/automaton/tests/helper_functions.h
        src/automaton/tests/test_hoa.cpp
        src/automaton/tests/test_parity_util.cpp
        src/automaton/tests/test_transition_util.cpp
        src/automaton/tests/test_almost_equivalent_states.cpp)

add_executable(test_automaton src/automaton/tests/test_main.cpp ${AUTOMATON_INCLUDE} ${AUTOMATON_HOA_INCLUDE} ${AUTOMATON_TESTS} ${Catch2_HDRS})
target_include_directories(test_automaton PRIVATE range-v3/include ${CATCH2_DIR}/single_include ${HOAFP_DIR}/include)


# automaton/automata
# Generates special classes of "worst case" automata and outputs them in HOA format.
set(AUTOMATON_AUTOMATA_FILES
        src/automaton/automata/max_michelle.cpp
        src/automaton/automata/max_michelle.h)

add_executable(generate_worst_automata src/automaton/automata/generate_worst_automata.cpp ${AUTOMATON_AUTOMATA_FILES} ${AUTOMATON_HOA_INCLUDE})
target_include_directories(generate_worst_automata PRIVATE ${RANGEV3_DIR}/include ${HOAFP_DIR}/include)


# transition->state based converter
add_executable(transition_based_converter src/transition_based_converter.cpp ${AUTOMATON_INCLUDE} ${AUTOMATON_HOA_INCLUDE})
target_include_directories(transition_based_converter PRIVATE range-v3/include ${HOAFP_DIR}/include)

# *_main / *_statistics
set(MAST_COMMON_HDRS_MAIN
        src/construction_main.cpp
        src/construction_main.h
        ${AUTOMATON_HOA_INCLUDE} src/construction_executioner.cpp src/construction_executioner.h)

set(MAST_COMMON_HDRS_STAT
        src/construction_statistics.cpp
        src/construction_statistics.h
        ${AUTOMATON_HOA_INCLUDE} src/construction_executioner.cpp src/construction_executioner.h)

# rawstats
add_executable(rawstats src/rawstats.cpp ${AUTOMATON_INCLUDE} ${AUTOMATON_HOA_INCLUDE})
target_include_directories(rawstats PRIVATE range-v3/include ${HOAFP_DIR}/include)


# Hopcroft
set(HOPCROFT_INCLUDE ${AUTOMATON_INCLUDE} ${UTIL_INCLUDE} ${NBAUTILS_INCLUDE})

add_executable(hopcroft_main src/hopcroft/main_statistics.h src/hopcroft/main_statistics.cpp ${HOPCROFT_INCLUDE} ${MAST_COMMON_HDRS_MAIN})
target_include_directories(hopcroft_main PRIVATE ${RANGEV3_DIR}/include ${HOAFP_DIR}/include)

add_executable(hopcroft_statistics src/hopcroft/main_statistics.h src/hopcroft/main_statistics.cpp ${HOPCROFT_INCLUDE} ${MAST_COMMON_HDRS_STAT})
target_include_directories(hopcroft_statistics PRIVATE ${RANGEV3_DIR}/include ${HOAFP_DIR}/include)


# Skipper

set(SKIPPER_HDRS
        src/skipper/skipper_automaton.h
        src/skipper/skipper_automaton.cpp)

set(SKIPPER_INCLUDE ${SKIPPER_HDRS} ${AUTOMATON_INCLUDE} ${UTIL_INCLUDE} ${NBAUTILS_INCLUDE})

add_executable(skipper_main src/skipper/main_statistics.h src/skipper/main_statistics.cpp ${SKIPPER_INCLUDE} ${MAST_COMMON_HDRS_MAIN})
target_include_directories(skipper_main PRIVATE ${RANGEV3_DIR}/include ${HOAFP_DIR}/include)

add_executable(skipper_statistics src/skipper/main_statistics.h src/skipper/main_statistics.cpp ${SKIPPER_INCLUDE} ${MAST_COMMON_HDRS_STAT})
target_include_directories(skipper_statistics PRIVATE ${RANGEV3_DIR}/include ${HOAFP_DIR}/include)

# Schewe10

set(SCHEWE10_HDRS
        src/schewe10/schewe_automaton.h
        src/schewe10/schewe_automaton.cpp)

set(SCHEWE10_INCLUDE ${SCHEWE10_HDRS} ${AUTOMATON_INCLUDE} ${UTIL_INCLUDE} ${NBAUTILS_INCLUDE})

add_library(schewe_automaton ${SCHEWE10_INCLUDE})
target_include_directories(schewe_automaton PRIVATE ${RANGEV3_DIR}/include)

add_executable(schewe_main src/schewe10/main_statistics.h src/schewe10/main_statistics.cpp ${SCHEWE10_INCLUDE} ${MAST_COMMON_HDRS_MAIN})
target_include_directories(schewe_main PRIVATE ${RANGEV3_DIR}/include ${HOAFP_DIR}/include)

add_executable(schewe_statistics src/schewe10/main_statistics.h src/schewe10/main_statistics.cpp ${SCHEWE10_INCLUDE} ${MAST_COMMON_HDRS_STAT})
target_include_directories(schewe_statistics PRIVATE ${RANGEV3_DIR}/include ${HOAFP_DIR}/include)


set(SCHEWE10_TESTS
        src/schewe10/tests/test_main.cpp
        src/schewe10/tests/test_schewe_automaton.cpp)

add_executable(test_schewe10 ${SCHEWE10_TESTS} ${Catch2_HDRS} ${SCHEWE10_INCLUDE})
target_include_directories(test_schewe10 PRIVATE ${CATCH2_DIR}/single_include range-v3/include)


# FritzWilke06

set(FRITZWILKE06_HDRS
        src/fritzwilke06/delayed_simulation.h
        src/fritzwilke06/delayed_simulation.cpp
        src/fritzwilke06/delayed_simulation_automaton.cpp
        src/fritzwilke06/delayed_simulation_automaton.h
        src/fritzwilke06/gamma.cpp
        src/fritzwilke06/gamma.h)

set(FRITZWILKE06_INCLUDE ${FRITZWILKE06_HDRS} ${AUTOMATON_INCLUDE} ${UTIL_INCLUDE})

add_library(delayed_simulation ${FRITZWILKE06_INCLUDE})
target_include_directories(delayed_simulation PRIVATE ${RANGEV3_DIR}/include)

add_executable(fritzwilke_main src/fritzwilke06/main_statistics.h src/fritzwilke06/main_statistics.cpp ${FRITZWILKE06_INCLUDE} ${MAST_COMMON_HDRS_MAIN})
target_include_directories(fritzwilke_main PRIVATE ${RANGEV3_DIR}/include ${HOAFP_DIR}/include)

add_executable(fritzwilke_statistics src/fritzwilke06/main_statistics.h src/fritzwilke06/main_statistics.cpp ${FRITZWILKE06_INCLUDE} ${MAST_COMMON_HDRS_STAT})
target_include_directories(fritzwilke_statistics PRIVATE ${RANGEV3_DIR}/include ${HOAFP_DIR}/include)

set(FRITZWILKE06_TESTS
        src/fritzwilke06/tests/test_delayed_simulation.cpp
        src/fritzwilke06/tests/test_delayed_simulation_automaton.cpp
        src/fritzwilke06/tests/test_main.cpp)

add_executable(test_fritzwilke06 ${FRITZWILKE06_INCLUDE} ${FRITZWILKE06_TESTS} ${Catch2_HDRS})
target_include_directories(test_fritzwilke06 PRIVATE ${CATCH2_DIR}/single_include range-v3/include)


# IteratedMoore

set(ITERATEDMOORE_HDRS
        src/iterated_moore/iterated_moore.h
        src/iterated_moore/iterated_moore.cpp)

set(ITERATEDMOORE_INCLUDE ${ITERATEDMOORE_HDRS} ${AUTOMATON_INCLUDE} ${UTIL_INCLUDE})

add_library(iterated_moore ${ITERATEDMOORE_INCLUDE})
target_include_directories(iterated_moore PRIVATE ${RANGEV3_DIR}/include)

add_executable(iterated_moore_main ${ITERATEDMOORE_INCLUDE} ${AUTOMATON_HOA_INCLUDE} ${MAST_COMMON_HDRS_MAIN} src/iterated_moore/main_statistics.cpp src/iterated_moore/main_statistics.h)
target_include_directories(iterated_moore_main PRIVATE ${RANGEV3_DIR}/include ${HOAFP_DIR}/include)

add_executable(iterated_moore_statistics ${ITERATEDMOORE_INCLUDE} ${AUTOMATON_HOA_INCLUDE} ${MAST_COMMON_HDRS_STAT} src/iterated_moore/main_statistics.cpp src/iterated_moore/main_statistics.h)
target_include_directories(iterated_moore_statistics PRIVATE ${RANGEV3_DIR}/include ${HOAFP_DIR}/include)

set(ITERATEDMOORE_TESTS
        src/iterated_moore/tests/test_main.cpp
        src/iterated_moore/tests/test_iterated_moore.cpp)

add_executable(test_iterated_moore ${ITERATEDMOORE_INCLUDE} ${ITERATEDMOORE_TESTS} ${Catch2_HDRS})
target_include_directories(test_iterated_moore PRIVATE ${CATCH2_DIR}/single_include range-v3/include)


# PathRefinement

set(PATHREFIN_HDRS
        src/path_refinement/path_refinement_automaton.h
        src/path_refinement/path_refinement_automaton.cpp
        src/path_refinement/path_refinement.h
        src/path_refinement/path_refinement.cpp
        src/path_refinement/visit_graph.h
        src/path_refinement/visit_graph.cpp)

set(PATHREFIN_INCLUDE ${PATHREFIN_HDRS} ${AUTOMATON_INCLUDE} ${UTIL_INCLUDE})

add_library(path_refinement ${PATHREFIN_INCLUDE})
target_include_directories(path_refinement PRIVATE ${RANGEV3_DIR}/include)

add_executable(path_refinement_main
        src/path_refinement/main_statistics.h
        src/path_refinement/main_statistics.cpp
        ${PATHREFIN_INCLUDE}
        ${AUTOMATON_HOA_INCLUDE}
        ${MAST_COMMON_HDRS_MAIN})
target_include_directories(path_refinement_main PRIVATE ${RANGEV3_DIR}/include ${HOAFP_DIR}/include)

add_executable(path_refinement_statistics
        src/path_refinement/main_statistics.h
        src/path_refinement/main_statistics.cpp
        ${PATHREFIN_INCLUDE}
        ${AUTOMATON_HOA_INCLUDE}
        ${MAST_COMMON_HDRS_STAT})
target_include_directories(path_refinement_statistics PRIVATE ${RANGEV3_DIR}/include ${HOAFP_DIR}/include)

add_executable(path_refinement_lambda_order
        src/path_refinement/main_lambda_order.cpp
        ${PATHREFIN_INCLUDE}
        ${AUTOMATON_HOA_INCLUDE})
target_include_directories(path_refinement_lambda_order PRIVATE ${RANGEV3_DIR}/include ${HOAFP_DIR}/include)

set(PATHREFIN_TESTS
        src/path_refinement/tests/test_main.cpp
        src/path_refinement/tests/test_path_refinement_automaton.cpp
        src/path_refinement/tests/test_visit_equality.cpp
        src/path_refinement/tests/test_nbautils_post.cpp)

add_executable(test_path_refinement ${PATHREFIN_INCLUDE} ${PATHREFIN_TESTS} ${AUTOMATON_HOA_INCLUDE} ${Catch2_HDRS})
target_include_directories(test_path_refinement PRIVATE ${CATCH2_DIR}/single_include range-v3/include ${HOAFP_DIR}/include)



# Threshold Moore

set(TREMOORE_HDRS
        src/threshold_moore/moore_ltk.cpp
        src/threshold_moore/moore_ltk.h
        src/threshold_moore/tm_relation.cpp
        src/threshold_moore/tm_relation.h)

set(TREMOORE_INCLUDE ${TREMOORE_HDRS} ${AUTOMATON_INCLUDE} ${UTIL_INCLUDE})

add_library(tremoore ${TREMOORE_INCLUDE})
target_include_directories(tremoore PRIVATE ${RANGEV3_DIR}/include)

add_executable(tremoore_main
        src/threshold_moore/main_statistics.h
        src/threshold_moore/main_statistics.cpp
        ${TREMOORE_INCLUDE}
        ${AUTOMATON_HOA_INCLUDE}
        ${MAST_COMMON_HDRS_MAIN})
target_include_directories(tremoore_main PRIVATE ${RANGEV3_DIR}/include ${HOAFP_DIR}/include)

add_executable(tremoore_statistics
        src/threshold_moore/main_statistics.h
        src/threshold_moore/main_statistics.cpp
        ${TREMOORE_INCLUDE}
        ${AUTOMATON_HOA_INCLUDE}
        ${MAST_COMMON_HDRS_STAT})
target_include_directories(tremoore_statistics PRIVATE ${RANGEV3_DIR}/include ${HOAFP_DIR}/include)

set(TREMOORE_TESTS
        src/threshold_moore/tests/test_main.cpp
        src/threshold_moore/tests/test_moore_ltk.cpp
        src/threshold_moore/tests/test_tm_relation.cpp)

add_executable(test_tremoore ${TREMOORE_INCLUDE} ${TREMOORE_TESTS} ${Catch2_HDRS})
target_include_directories(test_tremoore PRIVATE ${CATCH2_DIR}/single_include range-v3/include)




# LSF

set(LSF_HDRS
        src/lsf/lsf_relation.cpp
        src/lsf/lsf_relation.h)

set(LSF_INCLUDE ${LSF_HDRS} ${AUTOMATON_INCLUDE} ${UTIL_INCLUDE} ${TREMOORE_HDRS})

add_library(lsf ${LSF_HDRS})
target_include_directories(lsf PRIVATE ${RANGEV3_DIR}/include)

add_executable(lsf_main
        src/lsf/main_statistics.h
        src/lsf/main_statistics.cpp
        ${LSF_INCLUDE}
        ${AUTOMATON_HOA_INCLUDE}
        ${MAST_COMMON_HDRS_MAIN})
target_include_directories(lsf_main PRIVATE ${RANGEV3_DIR}/include ${HOAFP_DIR}/include)

add_executable(lsf_statistics
        src/lsf/main_statistics.h
        src/lsf/main_statistics.cpp
        ${LSF_INCLUDE}
        ${AUTOMATON_HOA_INCLUDE}
        ${MAST_COMMON_HDRS_STAT})
target_include_directories(lsf_statistics PRIVATE ${RANGEV3_DIR}/include ${HOAFP_DIR}/include)


set(LSF_TESTS
        src/lsf/tests/test_main.cpp
        src/lsf/tests/test_lsf_relation.cpp)

add_executable(test_lsf ${LSF_INCLUDE} ${LSF_TESTS} ${Catch2_HDRS})
target_include_directories(test_lsf PRIVATE ${CATCH2_DIR}/single_include range-v3/include)



# Simple Langeq

add_executable(simplelange_main src/simple_langeq/main_statistics.h src/simple_langeq/main_statistics.cpp ${MAST_COMMON_HDRS_MAIN})
target_include_directories(simplelange_main PRIVATE ${RANGEV3_DIR}/include ${HOAFP_DIR}/include)




# EVERYTHING

set(EVERYTHING_INCLUDE
        ${AUTOMATON_INCLUDE}
        ${SKIPPER_INCLUDE}
        ${SCHEWE10_INCLUDE}
        ${FRITZWILKE06_INCLUDE}
        ${ITERATEDMOORE_INCLUDE}
        ${PATHREFIN_INCLUDE}
        ${TREMOORE_INCLUDE}
        ${LSF_INCLUDE})

add_executable(everything_main
        src/everything/main_statistics.h
        src/everything/main_statistics.cpp
        ${EVERYTHING_INCLUDE}
        ${AUTOMATON_HOA_INCLUDE}
        ${MAST_COMMON_HDRS_MAIN})
target_include_directories(everything_main PRIVATE ${RANGEV3_DIR}/include ${HOAFP_DIR}/include)


add_executable(everything_statistics
        src/everything/main_statistics.h
        src/everything/main_statistics.cpp
        ${EVERYTHING_INCLUDE}
        ${AUTOMATON_HOA_INCLUDE}
        ${MAST_COMMON_HDRS_STAT})
target_include_directories(everything_statistics PRIVATE ${RANGEV3_DIR}/include ${HOAFP_DIR}/include)


set(EVERYTHING_SAFRA_INCLUDE
        ${AUTOMATON_INCLUDE}
        ${SKIPPER_INCLUDE}
        ${SCHEWE10_INCLUDE}
        ${FRITZWILKE06_INCLUDE}
        ${ITERATEDMOORE_INCLUDE}
        ${PATHREFIN_INCLUDE}
        ${TREMOORE_INCLUDE}
        ${LSF_INCLUDE})

add_executable(everything_safra_main
        src/everything_safra/main_statistics.h
        src/everything_safra/main_statistics.cpp
        ${EVERYTHING_INCLUDE}
        ${AUTOMATON_HOA_INCLUDE}
        ${MAST_COMMON_HDRS_MAIN})
target_include_directories(everything_safra_main PRIVATE ${RANGEV3_DIR}/include ${HOAFP_DIR}/include)


add_executable(everything_safra_statistics
        src/everything_safra/main_statistics.h
        src/everything_safra/main_statistics.cpp
        ${EVERYTHING_INCLUDE}
        ${AUTOMATON_HOA_INCLUDE}
        ${MAST_COMMON_HDRS_STAT})
target_include_directories(everything_safra_statistics PRIVATE ${RANGEV3_DIR}/include ${HOAFP_DIR}/include)