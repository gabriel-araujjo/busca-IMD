cmake_minimum_required(VERSION 2.8)

project(core)

add_definitions(-std=c++11)

set(HEADER_FILES
        array.h
        hash_map.h
        iterator.h
        list.h
        short_string.h)


add_library(core STATIC ${HEADER_FILES})

set_target_properties(core PROPERTIES LINKER_LANGUAGE CXX)
