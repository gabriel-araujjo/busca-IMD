cmake_minimum_required(VERSION 3.3)

project(core)

add_definitions(-std=c++11)

set(HEADER_FILES
        definitions.h
        array.h
        hash_map.h
        iterator.h
        list.h
        )

set(SOURCE_FILES
        list.cpp
        )

add_library(core STATIC ${SOURCE_FILES} ${HEADER_FILES})