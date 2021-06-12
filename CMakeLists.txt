#必须 cmake的最低版本号
cmake_minimum_required(VERSION 2.6)

#设置一个project_name， CXX表示只支持C++
project(netlib_project CXX)

#与add_test()一起使用，可以使用make test直接运行所有的被add_test()测试用例
enable_testing()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

set(CXX_FLAGS 
    -g 
    -Wall 
    -std=c++11 
    -rdynamic)

#设置C++编译选项
#set(CMAKE_CXX_FLAGS "${CXX_FLAGS}")
string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")

#if 内部build，PROJECT_BINARY_DIR==PROJECT_SOURCE_DIR
#IF 外部build，即在一个(比喻：一个build目录里)cmake，PROJECT_BINARY_DIR==cmake .. 所在目录
#EXECUTABLE_OUTPUT_PATH:将编译后的二进制执行文件放到指定目录
#LIBRARY_OUTPUT_PATH：将编译后的库放到指定文件里
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

find_package(Boost REQUIRED)
if(NOT Boost_INCLUDE_DIR)
    set(Boost_INCLUDE_DIR /usr/include/)
endif()

message(STATUS "BOOST=${Boost_INCLUDE_DIR}")

find_library(BOOSTTEST_LIBRARY NAMES boost_unit_test_framework)
message(STATUS "OP=${BOOSTTEST_LIBRARY}")

#include_directories(${Boost_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIR})
include_directories(${PROJECT_SOURCE_DIR})

add_subdirectory(netlib/base)
add_subdirectory(netlib/net)