add_executable(thread_test thread_test.cc)
target_link_libraries(thread_test netlib_base)

add_executable(mutex_test mutex_test.cc)
target_link_libraries(mutex_test netlib_base)

add_executable(time_test time_test.cc)
target_link_libraries(time_test netlib_base)
add_test(NAME time_test COMMAND time_test)

add_executable(timeZone_test timeZone_test.cc)
target_link_libraries(timeZone_test netlib_base)

add_executable(exception_test exception_test.cc)
target_link_libraries(exception_test netlib_base)

add_executable(logger_test logger_test.cc)
target_link_libraries(logger_test netlib_base)
