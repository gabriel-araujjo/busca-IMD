project(test)

set(CASES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/cases)

include_directories( ${CMAKE_BINARY_DIR}/generated/)

add_subdirectory(lib/googletest)
add_subdirectory(cases)
add_subdirectory(core_tests)