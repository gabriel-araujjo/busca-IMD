#utilities tools
project(io)

set(HEADER_FILES
        file_utils.h
        serialization_utils.h)
set(SOURCE_FILES
        file_utils.cpp
        serialization_utils.cpp)

add_library(io STATIC ${HEADER_FILES} ${SOURCE_FILES})