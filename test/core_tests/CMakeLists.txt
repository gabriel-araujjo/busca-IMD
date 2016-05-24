include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(core_tests
        array_test.cpp
        list_test.cpp)

target_link_libraries(core_tests gtest gtest_main)
target_link_libraries(core_tests core)
