project(test)

set(CASES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/cases)

include_directories(${CMAKE_BINARY_DIR}/generated/)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_subdirectory(lib/googletest)
add_subdirectory(cases)
add_subdirectory(core_tests)
add_subdirectory(index_tests)
add_subdirectory(config_tests)