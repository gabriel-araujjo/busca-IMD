
set(SOURCE_FILES
        config_file_utils_test.cpp
        file_item_test.cpp
        config_test.cpp)


add_executable(config_tests ${SOURCE_FILES})

target_link_libraries(config_tests gtest gtest_main)

target_link_libraries(config_tests config)