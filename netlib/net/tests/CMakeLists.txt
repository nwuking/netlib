
#if(BOOSTTEST_LIBRARY)
#link_directories(/usr/lib/)
add_executable(buffer_test Buffer_test.cc)
target_link_libraries(buffer_test netlib_net -lboost_unit_test_framework)
add_test(NAME buffer_test COMMAND buffer_test)

add_executable(eventloop_test EventLoop_test.cc)
target_link_libraries(eventloop_test netlib_net)

add_executable(eventloopthread_test EventLoopThread_test.cc)
target_link_libraries(eventloopthread_test netlib_net)

add_executable(eventloopthreadpool_test EventLoopThreadPool_test.cc)
target_link_libraries(eventloopthreadpool_test netlib_net)

add_executable(timerqueue_test TimerQueue_test.cc)
target_link_libraries(timerqueue_test netlib_net)

add_executable(sockaddr_test SockAddr_test.cc)
target_link_libraries(sockaddr_test netlib_net -lboost_unit_test_framework)
add_test(NAME sockaddr_test COMMAND sockaddr_test)

#endif()
