
#if(BOOSTTEST_LIBRARY)
#link_directories(/usr/lib/)
add_executable(buffer_test Buffer_test.cc)
target_link_libraries(buffer_test netlib_net boost_unit_test_framework)
add_test(NAME buffer_test COMMAND buffer_test)

#endif()
