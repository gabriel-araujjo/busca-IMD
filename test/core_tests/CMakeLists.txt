set(SOURCE_FILES
        array_test.cpp
        list_test.cpp
        hash_map_test.cpp
        short_string_test.cpp)


add_executable(core_tests ${SOURCE_FILES})

target_link_libraries(core_tests gtest gtest_main)
target_link_libraries(core_tests core)
