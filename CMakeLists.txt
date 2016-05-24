cmake_minimum_required(VERSION 3.3)
project(busca_imd)

add_definitions(-std=c++11)

set(SOURCE_FILES main.cpp)
add_executable(busca_imd ${SOURCE_FILES})

include_directories(core)

add_subdirectory(core)
add_subdirectory(test)

target_link_libraries(busca_imd core)