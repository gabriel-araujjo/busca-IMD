include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(runCoreTests
        array_test.cpp)

target_link_libraries(runCoreTests gtest gtest_main)
target_link_libraries(runCoreTests core)
