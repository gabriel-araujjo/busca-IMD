set(SOURCE_FILES
        index_test.cpp)


add_executable(index_tests ${SOURCE_FILES})

target_link_libraries(index_tests gtest gtest_main)

target_link_libraries(index_tests index)