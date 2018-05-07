
cmake_minimum_required (VERSION 2.6)
set (CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -g")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DNDEBUG -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")

project (hello_world)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# nbautils

set(NBAUTILS_DIR ./nbautils)
add_subdirectory(${NBAUTILS_DIR})

set(NBAUTILS_INCLUDE
        ${NBAUTILS_DIR}/src/dev/memusage.h
        ${NBAUTILS_DIR}/src/dev/bench.hh
        ${NBAUTILS_DIR}/src/common/util.hh
        ${NBAUTILS_DIR}/src/common/relorder.hh
        ${NBAUTILS_DIR}/src/common/triebimap.hh
        ${NBAUTILS_DIR}/src/common/bimap.hh
        ${NBAUTILS_DIR}/src/common/algo.hh
        ${NBAUTILS_DIR}/src/common/graph.hh
        ${NBAUTILS_DIR}/src/common/scc.hh
        ${NBAUTILS_DIR}/src/swa.hh
        ${NBAUTILS_DIR}/src/ba.hh
        ${NBAUTILS_DIR}/src/io.hh
        ${NBAUTILS_DIR}/src/pa.hh
        ${NBAUTILS_DIR}/src/ps.hh
        ${NBAUTILS_DIR}/src/level.hh
        ${NBAUTILS_DIR}/src/det.hh)

ExternalProject_Get_Property(cpphoafparser source_dir)
include_directories(${source_dir}/include)

# Catch2

set(CATCH2_DIR ./Catch2)
add_subdirectory(${CATCH2_DIR})

set(Catch2_HDRS
        ${CATCH2_DIR}/single_include/catch.hpp)

# range v3

set(RANGEV3_DIR ./range-v3)
add_subdirectory(${RANGEV3_DIR})

file(GLOB_RECURSE RANGE_V3_PUBLIC_HEADERS
        RELATIVE ${RANGEV3_DIR}/include
        "${RANGEV3_DIR}/include/*.hpp")

# json

set(JSON_DIR ./json)
add_subdirectory(${JSON_DIR})


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

set(AUTOMATON_INCLUDE ${AUTOMATON_FILES} ${NBAUTILS_INCLUDE} ${RANGE_V3_PUBLIC_HEADERS} src/equivalence_relation.h)

add_library(automaton ${AUTOMATON_INCLUDE})
target_link_libraries(automaton nbautils-lib)
target_include_directories(automaton PRIVATE ${NBAUTILS_DIR}/src ${RANGEV3_DIR}/include)

set(AUTOMATON_TESTS
        src/automaton/tests/test_automata.cpp
        src/automaton/tests/test_util.cpp
        src/automaton/tests/helper_functions.h
        src/automaton/tests/test_nbautils_bridge.cpp)

add_executable(automaton_tests src/automaton/tests/test_main.cpp ${AUTOMATON_FILES} ${AUTOMATON_TESTS} ${Catch2_HDRS} ${NBAUTILS_INCLUDE} ${RANGE_V3_PUBLIC_HEADERS} src/equivalence_relation.h)
target_link_libraries(automaton_tests nbautils-lib)
target_include_directories(automaton_tests PRIVATE ${NBAUTILS_DIR}/src ${RANGEV3_DIR}/include ${CATCH2_DIR}/single_include)



# Schewe10

set(SCHEWE10_HDRS
        src/schewe10/topological_ordering.h
        src/schewe10/topological_ordering.impl.h
        src/schewe10/almost_equivalent_states.h
        src/schewe10/almost_equivalent_states.impl.h
        src/schewe10/schewe_automaton.h src/schewe10/schewe_automaton.cpp)

set(SCHEWE10_INCLUDE ${SCHEWE10_HDRS} ${AUTOMATON_INCLUDE} src/equivalence_relation.h ${NBAUTILS_INCLUDE})

add_library(schewe_automaton ${SCHEWE10_INCLUDE})
target_link_libraries(schewe_automaton nbautils-lib automaton)
target_include_directories(schewe_automaton PRIVATE ${NBAUTILS_DIR}/src ${RANGEV3_DIR}/include)

add_executable(schewe_main src/schewe10/schewe_main.cpp ${SCHEWE10_INCLUDE})
target_link_libraries(schewe_main nbautils-lib)
add_dependencies(schewe_main cpphoafparser)
target_include_directories(schewe_main PRIVATE ${NBAUTILS_DIR}/src ${RANGEV3_DIR}/include)

add_executable(schewe_statistics src/schewe10/schewe_statistics.cpp ${SCHEWE10_INCLUDE})
target_link_libraries(schewe_statistics nbautils-lib)
add_dependencies(schewe_statistics cpphoafparser)
target_include_directories(schewe_statistics PRIVATE ${NBAUTILS_DIR}/src ${RANGEV3_DIR}/include)


set(SCHEWE10_TESTS
        src/schewe10/tests/test_almost_equivalent_states.cpp
        src/schewe10/tests/test_topological_ordering.cpp)

add_executable(test_schewe10 src/schewe10/tests/test_main.cpp ${SCHEWE10_TESTS} ${Catch2_HDRS} ${SCHEWE10_INCLUDE})
target_link_libraries(test_schewe10 nbautils-lib)
target_include_directories(test_schewe10 PRIVATE ${NBAUTILS_DIR}/src ${CATCH2_DIR}/single_include ${RANGEV3_DIR}/include)
