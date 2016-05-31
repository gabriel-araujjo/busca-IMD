#functions to make the config folder visible to the root cmakelists
project(index)

set(HEADER_FILES
        index.h)
set(SOURCE_FILES
        index.cpp)

add_library(index STATIC ${HEADER_FILES} ${SOURCE_FILES})