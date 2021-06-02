set(base_SRCS
    AsyncLogging.cc 
    CountDownLatch.cc 
    CurrentThread.cc 
    Date.cc 
    Exception.cc 
    FileUtil.cc 
    LogFile.cc 
    Logging.cc 
    LogStream.cc 
    ProcessInfo.cc 
    Thread.cc 
    Time.cc 
    TimeZone.cc)

add_library(netlib_base ${base_SRCS})
target_link_libraries(netlib_base pthread)

install(TARGETS netlib_base DESTINATION lib)

file(GLOB HEADERS "*.h")
install(FILES ${HEADERS} DESTINATION include/netlib/base)

add_subdirectory(tests)