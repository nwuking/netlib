cmake_minimum_required(VERSION 2.6)

project(Netlib_project CXX)

enable_testing()

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

find_package(Boost REQUIRED)
find_library(BOOSTTEST_LIBRARY NAMES boost_unit_test_framework)

#include_directories(${Boost_INCLUDE_DIRS})
include_directories(/usr/lib)
include_directories(${PROJECT_SOURCE_DIR})

add_subdirectory(netlib/base)
add_subdirectory(netlib/net)