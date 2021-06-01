include_directories("~/netlib")

add_executable(thread_test thread_test.cc)
target_link_libraries(thread_test netlib)
