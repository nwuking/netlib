set(net_SRCS 
    Acceptor.cc 
    Buffer.cc 
    Chnnel.cc 
    Connector.cc 
    Epoller.cc 
    EventLoop.cc 
    EventLoopThread.cc 
    EventLoopThreadPool.cc 
    SockAddr.cc 
    Socket.cc 
    SockFunc.cc 
    TcpClient.cc 
    TcpConnection.cc 
    TcpServer.cc 
    Timer.cc 
    TimerQueue.cc)

add_library(netlib_net ${net_SRCS})
target_link_libraries(netlib_net netlib_base)

message(STATUS "PRE=${CMAKE_INSTALL_PREFIX}")

install(TARGETS netlib_net DESTINATION lib)

set(HEADERS 
    Buffer.h 
    Chnnel.h 
    EventLoop.h 
    EventLoopThread.h 
    EventLoopThreadPool.h 
    SockAddr.h 
    TcpClient.h 
    TcpConnection.h 
    TcpServer.h 
    TimerId.h)

install(FILES ${HEADERS} DESTINATION include/netlib/net)

add_subdirectory(tests)

