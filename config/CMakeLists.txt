#functions to make the config folder visible to the root cmakelists
project(config)

set(HEADER_FILES
        config.h
        config_file_utils.h
        file_info.h)
set(SOURCE_FILES
        config.cpp
        config_file_utils.cpp
        file_info.cpp)

add_library(config STATIC ${HEADER_FILES} ${SOURCE_FILES})

target_link_libraries(config core index)