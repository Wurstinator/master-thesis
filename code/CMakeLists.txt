
cmake_minimum_required (VERSION 2.6)
cmake_policy(SET CMP0048 NEW)
set (CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -g")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DNDEBUG -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")

project (hello_world)

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

# automaton module

set(AUTOMATON_FILES
        src/automaton/finite_automaton.h
        src/automaton/transition_automaton.h
        src/automaton/deterministic_automaton.h
        src/automaton/deterministic_automaton.cpp
        src/automaton/nondeterministic_automaton.h
        src/automaton/nondeterministic_automaton.cpp
        src/automaton/labelled_automaton.h
        src/automaton/util.h
        src/automaton/util.impl.h
        src/automaton/util.cpp
        src/automaton/parity.cpp
        src/automaton/parity.h
        src/automaton/nbautils_bridge.h
        src/automaton/nbautils_bridge.impl.h
        src/automaton/is_specialization_base_of.h)

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
        src/automaton/hoa/hoa_io.impl.h)

set(AUTOMATON_INCLUDE ${AUTOMATON_FILES} src/equivalence_relation.h)
set(AUTOMATON_HOA_INCLUDE ${AUTOMATON_INCLUDE} ${AUTOMATON_HOA_FILES} ${HOAFP_INCLUDE})

add_library(automaton ${AUTOMATON_INCLUDE})
target_include_directories(automaton PRIVATE ${RANGEV3_DIR}/include)

add_library(automaton_with_hoa ${AUTOMATON_INCLUDE} ${AUTOMATON_HOA_INCLUDE})
target_include_directories(automaton_with_hoa PRIVATE ${RANGEV3_DIR}/include)

set(AUTOMATON_TESTS
        src/automaton/tests/test_automata.cpp
        src/automaton/tests/test_util.cpp
        src/automaton/tests/helper_functions.h
        src/automaton/tests/test_hoa.cpp)

add_executable(test_automaton src/automaton/tests/test_main.cpp ${AUTOMATON_INCLUDE} ${AUTOMATON_HOA_INCLUDE} ${AUTOMATON_TESTS} ${Catch2_HDRS})
target_include_directories(test_automaton PRIVATE range-v3/include ${CATCH2_DIR}/single_include ${HOAFP_DIR}/include)


# *_main / *_statistics
set(MAST_COMMON_HDRS_MAIN
        src/construction_main.cpp
        src/construction_main.h
        ${AUTOMATON_HOA_INCLUDE})

set(MAST_COMMON_HDRS_STAT
        src/construction_statistics.cpp
        src/construction_statistics.h
        ${AUTOMATON_HOA_INCLUDE})

# Schewe10

set(SCHEWE10_HDRS
        src/schewe10/almost_equivalent_states.h
        src/schewe10/almost_equivalent_states.impl.h
        src/schewe10/schewe_automaton.h
        src/schewe10/schewe_automaton.cpp)

set(SCHEWE10_INCLUDE ${SCHEWE10_HDRS} ${AUTOMATON_INCLUDE} src/equivalence_relation.h ${NBAUTILS_INCLUDE})

add_library(schewe_automaton ${SCHEWE10_INCLUDE})
target_include_directories(schewe_automaton PRIVATE ${RANGEV3_DIR}/include)

add_executable(schewe_main src/schewe10/schewe_main.cpp ${SCHEWE10_INCLUDE} ${MAST_COMMON_HDRS_MAIN})
target_include_directories(schewe_main PRIVATE ${RANGEV3_DIR}/include ${HOAFP_DIR}/include)

add_executable(schewe_statistics src/schewe10/schewe_statistics.cpp ${SCHEWE10_INCLUDE} ${MAST_COMMON_HDRS_STAT})
target_include_directories(schewe_statistics PRIVATE ${RANGEV3_DIR}/include ${HOAFP_DIR}/include)


set(SCHEWE10_TESTS
        src/schewe10/tests/test_almost_equivalent_states.cpp
        src/schewe10/tests/test_topological_ordering.cpp
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

set(FRITZWILKE06_INCLUDE ${FRITZWILKE06_HDRS} ${AUTOMATON_INCLUDE} src/equivalence_relation.h)

add_library(delayed_simulation ${FRITZWILKE06_INCLUDE})
target_include_directories(delayed_simulation PRIVATE range-v3/include)

add_executable(fritzwilke_main src/fritzwilke06/fritzwilke_main.cpp ${FRITZWILKE06_INCLUDE} ${MAST_COMMON_HDRS_MAIN})
target_include_directories(fritzwilke_main PRIVATE ${RANGEV3_DIR}/include ${HOAFP_DIR}/include)

add_executable(fritzwilke_statistics src/fritzwilke06/fritzwilke_statistics.cpp ${FRITZWILKE06_INCLUDE} ${MAST_COMMON_HDRS_STAT})
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

set(ITERATEDMOORE_INCLUDE ${ITERATEDMOORE_HDRS} ${AUTOMATON_INCLUDE} src/equivalence_relation.h)

add_library(iterated_moore ${ITERATEDMOORE_INCLUDE})
target_include_directories(iterated_moore PRIVATE range-v3/include)

add_executable(iterated_moore_main src/iterated_moore/iterated_moore_main.cpp ${ITERATEDMOORE_INCLUDE} ${AUTOMATON_HOA_FILES} ${MAST_COMMON_HDRS_MAIN})
target_include_directories(iterated_moore_main PRIVATE ${RANGEV3_DIR}/include ${HOAFP_DIR}/include)

add_executable(iterated_moore_statistics src/iterated_moore/iterated_moore_statistics.cpp ${ITERATEDMOORE_INCLUDE} ${AUTOMATON_HOA_FILES} ${MAST_COMMON_HDRS_STAT})
target_include_directories(iterated_moore_statistics PRIVATE ${RANGEV3_DIR}/include ${HOAFP_DIR}/include)

set(ITERATEDMOORE_TESTS
        src/iterated_moore/tests/test_main.cpp
        src/iterated_moore/tests/test_iterated_moore.cpp)

add_executable(test_iterated_moore ${ITERATEDMOORE_INCLUDE} ${ITERATEDMOORE_TESTS} ${Catch2_HDRS})
target_include_directories(test_iterated_moore PRIVATE ${CATCH2_DIR}/single_include range-v3/include)