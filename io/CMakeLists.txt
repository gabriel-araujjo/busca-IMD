#utilities tools
project(io)

set(HEADER_FILES
        serialization_utils.h)
set(SOURCE_FILES
        serialization_utils.cpp)

add_library(io STATIC ${HEADER_FILES} ${SOURCE_FILES})