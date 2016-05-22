cmake_minimum_required(VERSION 3.3)
project(busca_imd)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp search/result.cpp search/result.h search/entry.cpp search/entry.h index/index.cpp index/index.h core/hash_map.cpp core/hash_map.h core/array.cpp core/array.h search/search.cpp search/search.h config/config.cpp config/config.h core/list.cpp core/list.h core/iterator.cpp core/iterator.h test/list_test.cpp test/list_test.h)
add_executable(busca_imd ${SOURCE_FILES})