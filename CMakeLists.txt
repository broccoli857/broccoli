cmake_minimum_required(VERSION 2.8)
project(broccoli)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -o0 -ggdb -std=c++11 -Wno-deprecated -Werror -Wno-unused-function")

# 通过find_package引入非官方的库(该方式只对支持cmake编译安装的库有效)
find_package(yaml-cpp REQUIRED)

include_directories(.)
include_directories(${YAML_CPP_INCLUDE_DIR})
include_directories(/usr/lib)

find_library(YAMLCPP yaml-cpp)

set(LIB_SRC
    broccoli/log.cpp
    broccoli/util.cpp
    broccoli/config.cpp
    )

add_library(broccoli SHARED ${LIB_SRC})

add_executable(test_log tests/test.cpp)
add_dependencies(test_log broccoli)
target_link_libraries(test_log broccoli yaml-cpp)

add_executable(test_config tests/test_config.cpp)
add_dependencies(test_config broccoli)
target_link_libraries(test_config broccoli yaml-cpp)

SET(PROPERTIES_OUTPUT_NAME ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_NAME ${PROJECT_SOURCE_DIR}/lib)
