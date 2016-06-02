cmake_minimum_required(VERSION 2.8)

project(config)

add_definitions(-std=c++11)

set(HEADER_FILES
        config.h
        )

set(SOURCE_FILES
        config.cpp)


add_library(config STATIC ${HEADER_FILES} ${SOURCE_FILES})