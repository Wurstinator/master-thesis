
cmake_minimum_required (VERSION 2.6)
set (CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -g")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DNDEBUG -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")

project (hello_world)

set(NBAUTILS_DIR ./nbautils)
set(CATCH2_DIR ./Catch2)

add_subdirectory(nbautils)
add_subdirectory(Catch2)
add_subdirectory(range-v3)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(nbautils_HDRS
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

set(Catch2_HDRS
    ${CATCH2_DIR}/single_include/catch.hpp)

set(SCHEWE10_HDRS
        src/schewe10/find_goal_sccs.h
        src/schewe10/find_goal_sccs.cpp
        src/schewe10/product_automaton.h
        src/equivalence_relation.h
        src/schewe10/almost_equivalent_states.h
        src/schewe10/merge_states.h
        src/schewe10/can_reach_state.h
        src/schewe10/merge_sccs.h
        src/schewe10/backwards_automaton.h
        src/schewe10/topological_ordering.h
        src/schewe10/product_automaton.cpp
        )

file(GLOB_RECURSE RANGE_V3_PUBLIC_HEADERS
        RELATIVE range-v3/include
        "range-v3/include/*.hpp")

set(AUTOMATON_HDRS
        src/automaton/finite_automaton.h
        src/automaton/transition_automaton.h
        src/automaton/deterministic_automaton.h
        src/automaton/deterministic_automaton.cpp
        src/automaton/nondeterministic_automaton.h
        src/automaton/nondeterministic_automaton.cpp
        src/automaton/labelled_automaton.h
        src/automaton/util.h
        src/automaton/util.impl.h
        )

ExternalProject_Get_Property(cpphoafparser source_dir)
include_directories(${source_dir}/include)

add_executable(hello_world src/hello_world.cpp ${nbautils_HDRS} ${SCHEWE10_HDRS} ${AUTOMATON_HDRS})
target_link_libraries(hello_world nbautils-lib)
add_dependencies(hello_world cpphoafparser)
target_include_directories(hello_world PRIVATE ${NBAUTILS_DIR}/src)

add_library(automaton ${AUTOMATON_HDRS} ${nbautils_HDRS} ${RANGE_V3_PUBLIC_HEADERS})
target_link_libraries(automaton nbautils-lib)
target_include_directories(automaton PRIVATE ${NBAUTILS_DIR}/src range-v3/include)

add_executable(schewe_main src/schewe10/schewe_main.cpp ${nbautils_HDRS} ${SCHEWE10_HDRS})
target_link_libraries(schewe_main nbautils-lib)
add_dependencies(schewe_main cpphoafparser)
target_include_directories(schewe_main PRIVATE ${NBAUTILS_DIR}/src)

add_executable(schewe_statistics src/schewe10/schewe_statistics.cpp ${nbautils_HDRS} ${SCHEWE10_HDRS})
target_link_libraries(schewe_statistics nbautils-lib)
add_dependencies(schewe_statistics cpphoafparser)
target_include_directories(schewe_statistics PRIVATE ${NBAUTILS_DIR}/src)

add_library(schewe_automaton src/schewe10/schewe_automaton.h ${nbautils_HDRS} ${SCHEWE10_HDRS})
target_link_libraries(schewe_automaton nbautils-lib)
target_include_directories(schewe_automaton PRIVATE ${NBAUTILS_DIR}/src)

set(SCHEWE10_TESTS
        src/schewe10/tests/test_almost_equivalent_states.cpp
        src/schewe10/tests/test_backwards_automaton.cpp
        src/schewe10/tests/test_can_reach_state.cpp
        src/schewe10/tests/test_find_goal_sccs.cpp
        src/schewe10/tests/test_merge_sccs.cpp
        src/schewe10/tests/test_merge_states.cpp
        src/schewe10/tests/test_product_automaton.cpp)

add_executable(test_schewe10 src/schewe10/tests/test_schewe10.cpp ${SCHEWE10_TESTS} ${Catch2_HDRS} ${nbautils_HDRS} ${SCHEWE10_HDRS})
target_link_libraries(test_schewe10 nbautils-lib)
target_include_directories(test_schewe10 PRIVATE ${NBAUTILS_DIR}/src ${CATCH2_DIR}/single_include)
