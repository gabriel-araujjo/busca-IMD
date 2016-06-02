cmake_minimum_required(VERSION 2.8)

project(core)

add_definitions(-std=c++11)

set(HEADER_FILES
        array.h
        hash_map.h
        iterator.h
        list.h
        short_string.h)
set(SOURCE_FILES
        short_string.cpp)


add_library(core STATIC ${HEADER_FILES} ${SOURCE_FILES})
