#functions to make the config folder visible to the root cmakelists
project(config)

set(HEADER_FILES
        config.h
        config_file_utils.h)
set(SOURCE_FILES
        config.cpp
        config_file_utils.cpp)

add_library(config STATIC ${HEADER_FILES} ${SOURCE_FILES})