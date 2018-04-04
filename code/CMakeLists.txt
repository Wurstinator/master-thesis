
cmake_minimum_required (VERSION 2.6)
set (CMAKE_CXX_STANDARD 14)

project (hello_world)

set(NBAUTILS_DIR ./nbautils)

add_subdirectory(nbautils)

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
  ${NBAUTILS_DIR}/src/det.hh
        find_goal_sccs.h find_goal_sccs.cpp)


add_executable(hello_world hello_world.cpp ${nbautils_HDRS} merge_sccs.h)
target_link_libraries(hello_world nbautils-lib)
add_dependencies(hello_world cpphoafparser)
target_include_directories(hello_world PRIVATE ${NBAUTILS_DIR}/src)
