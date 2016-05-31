include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(core_tests
        array_test.cpp
        list_test.cpp
        hash_map_test.cpp
        config_test.cpp)

target_link_libraries(core_tests gtest gtest_main)
target_link_libraries(core_tests core)
target_link_libraries(core_tests index)
target_link_libraries(core_tests config)
