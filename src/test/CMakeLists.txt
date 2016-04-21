add_executable(benchmark_test
  benchmark_test.cc)

add_executable(performance_test
  performance_test.cc)

target_link_libraries(performance_test
utils)