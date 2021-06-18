# Install script for directory: /home/nwuking/netlib/netlib/net

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/home/nwuking/netlib/build/release-cpp11/release-install-cpp11")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "release")
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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/nwuking/netlib/build/release-cpp11/lib/libnetlib_net.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/netlib/net" TYPE FILE FILES
    "/home/nwuking/netlib/netlib/net/Buffer.h"
    "/home/nwuking/netlib/netlib/net/CallBack.h"
    "/home/nwuking/netlib/netlib/net/Chnnel.h"
    "/home/nwuking/netlib/netlib/net/EventLoop.h"
    "/home/nwuking/netlib/netlib/net/EventLoopThread.h"
    "/home/nwuking/netlib/netlib/net/EventLoopThreadPool.h"
    "/home/nwuking/netlib/netlib/net/SockAddr.h"
    "/home/nwuking/netlib/netlib/net/TcpClient.h"
    "/home/nwuking/netlib/netlib/net/TcpConnection.h"
    "/home/nwuking/netlib/netlib/net/TcpServer.h"
    "/home/nwuking/netlib/netlib/net/TimerId.h"
    )
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/nwuking/netlib/build/release-cpp11/netlib/net/tests/cmake_install.cmake")

endif()

