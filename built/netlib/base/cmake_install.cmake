# Install script for directory: /home/nwuking/netlib/netlib/base

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/nwuking/netlib/built/netlib/base/libnetlib_base.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/netlib/base" TYPE FILE FILES
    "/home/nwuking/netlib/netlib/base/AsyncLogging.h"
    "/home/nwuking/netlib/netlib/base/Condition.h"
    "/home/nwuking/netlib/netlib/base/CountDownLatch.h"
    "/home/nwuking/netlib/netlib/base/CurrentThread.h"
    "/home/nwuking/netlib/netlib/base/Date.h"
    "/home/nwuking/netlib/netlib/base/Exception.h"
    "/home/nwuking/netlib/netlib/base/FileUtil.h"
    "/home/nwuking/netlib/netlib/base/LogFile.h"
    "/home/nwuking/netlib/netlib/base/LogStream.h"
    "/home/nwuking/netlib/netlib/base/Logging.h"
    "/home/nwuking/netlib/netlib/base/Mutex.h"
    "/home/nwuking/netlib/netlib/base/ProcessInfo.h"
    "/home/nwuking/netlib/netlib/base/Thread.h"
    "/home/nwuking/netlib/netlib/base/Time.h"
    "/home/nwuking/netlib/netlib/base/TimeZone.h"
    "/home/nwuking/netlib/netlib/base/noncopyable.h"
    )
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/nwuking/netlib/built/netlib/base/tests/cmake_install.cmake")

endif()

