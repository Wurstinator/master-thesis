
cmake_minimum_required (VERSION 2.6)
set (CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DNDEBUG -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")

project (hello_world)

set(NBAUTILS_DIR ./nbautils)
set(CATCH2_DIR ./Catch2)

add_subdirectory(nbautils)
add_subdirectory(Catch2)

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
        schewe10/find_goal_sccs.h
        schewe10/find_goal_sccs.cpp
        schewe10/product_automaton.h
        schewe10/equivalence_relation.h
        schewe10/almost_equivalent_states.h
        schewe10/merge_states.h
        schewe10/can_reach_state.h
        schewe10/merge_sccs.h
        schewe10/backwards_automaton.h
        )

add_executable(hello_world hello_world.cpp ${nbautils_HDRS} ${SCHEWE10_HDRS})
target_link_libraries(hello_world nbautils-lib)
add_dependencies(hello_world cpphoafparser)
target_include_directories(hello_world PRIVATE ${NBAUTILS_DIR}/src)

add_executable(test_almost_equivalent_states schewe10/test_almost_equivalent_states.cpp ${Catch2_HDRS} ${nbautils_HDRS} ${SCHEWE10_HDRS})
target_link_libraries(test_almost_equivalent_states nbautils-lib)
add_dependencies(test_almost_equivalent_states cpphoafparser)
target_include_directories(test_almost_equivalent_states PRIVATE ${NBAUTILS_DIR}/src ${CATCH2_DIR}/single_include)

add_executable(test_backwards_automaton schewe10/test_backwards_automaton.cpp ${Catch2_HDRS} ${nbautils_HDRS} ${SCHEWE10_HDRS})
target_link_libraries(test_backwards_automaton nbautils-lib)
add_dependencies(test_backwards_automaton cpphoafparser)
target_include_directories(test_backwards_automaton PRIVATE ${NBAUTILS_DIR}/src ${CATCH2_DIR}/single_include)

add_executable(test_can_reach_state schewe10/test_can_reach_state.cpp ${Catch2_HDRS} ${nbautils_HDRS} ${SCHEWE10_HDRS})
target_link_libraries(test_can_reach_state nbautils-lib)
add_dependencies(test_can_reach_state cpphoafparser)
target_include_directories(test_can_reach_state PRIVATE ${NBAUTILS_DIR}/src ${CATCH2_DIR}/single_include)

add_executable(test_find_goal_sccs schewe10/test_find_goal_sccs.cpp ${Catch2_HDRS} ${nbautils_HDRS} ${SCHEWE10_HDRS})
target_link_libraries(test_find_goal_sccs nbautils-lib)
add_dependencies(test_find_goal_sccs cpphoafparser)
target_include_directories(test_find_goal_sccs PRIVATE ${NBAUTILS_DIR}/src ${CATCH2_DIR}/single_include)

add_executable(test_merge_sccs schewe10/test_merge_sccs.cpp ${Catch2_HDRS} ${nbautils_HDRS} ${SCHEWE10_HDRS})
target_link_libraries(test_merge_sccs nbautils-lib)
add_dependencies(test_merge_sccs cpphoafparser)
target_include_directories(test_merge_sccs PRIVATE ${NBAUTILS_DIR}/src ${CATCH2_DIR}/single_include)

add_executable(test_merge_states schewe10/test_merge_states.cpp ${Catch2_HDRS} ${nbautils_HDRS} ${SCHEWE10_HDRS})
target_link_libraries(test_merge_states nbautils-lib)
add_dependencies(test_merge_states cpphoafparser)
target_include_directories(test_merge_states PRIVATE ${NBAUTILS_DIR}/src ${CATCH2_DIR}/single_include)

add_executable(test_product_automaton schewe10/test_product_automaton.cpp ${Catch2_HDRS} ${nbautils_HDRS} ${SCHEWE10_HDRS})
target_link_libraries(test_product_automaton nbautils-lib)
add_dependencies(test_product_automaton cpphoafparser)
target_include_directories(test_product_automaton PRIVATE ${NBAUTILS_DIR}/src ${CATCH2_DIR}/single_include)