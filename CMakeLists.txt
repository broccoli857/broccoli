cmake_minimum_required(VERSION 2.8)
project(broccoli)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -o0 -ggdb -std=c++11 -Wno-deprecated -Werror -Wno-unused-function")

include_directories(.)

set(LIB_SRC
    broccoli/log.cpp
    broccoli/util.cpp
    #broccoli/config.cpp
    )

add_library(broccoli SHARED ${LIB_SRC})
#add_library(broccoli_static STATIC $(LIB_SRC))
#set_TARGET_PROPERTIES (broccoli_static PROPERTIES OUTPUT_NAME "broccoli")

add_executable(test_log tests/test.cpp)
add_dependencies(test_log broccoli)
target_link_libraries(test_log broccoli)

#add_executable(test_config tests/test_config.cpp)
#add_dependencies(test_config broccoli)
#target_link_libraries(test_config broccoli)

SET(PROPERTIES_OUTPUT_NAME ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_NAME ${PROJECT_SOURCE_DIR}/lib)
