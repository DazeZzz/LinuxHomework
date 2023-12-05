add_executable(sort_file_test sort_file_test.cpp)
add_executable(my_test my_test.cpp)

target_link_libraries(
  sort_file_test
  data_producer
  data_sorter
  gtest
  gtest_main
)