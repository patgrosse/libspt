cmake_minimum_required(VERSION 3.0)
project(libspt_tests)

add_subdirectory(lib/googletest/googletest)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(runTests data_fifo_test.cpp)
target_link_libraries(runTests gtest gtest_main)