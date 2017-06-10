cmake_minimum_required(VERSION 3.0)
project(libspt_tests)

find_package(GTest)
if (GTEST_FOUND)
    include_directories(${GTEST_INCLUDE_DIRS})
else()
    add_subdirectory(lib/googletest/googletest)
    include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
endif()

set(CMAKE_C_FLAGS "-Werror -Wall -Wextra")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} -g -Wfloat-equal -Wundef -Wshadow -Wcast-align \
-Wstrict-prototypes -Wstrict-overflow=5 -Wno-write-strings -Waggregate-return -Wcast-qual -Wswitch-default \
-Wswitch-enum -Wunreachable-code")
use_c99()

# dependencies
include(../cmake/FindLibEvent.cmake)
include_directories(../src/include)
find_package(Threads)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

FILE(GLOB LIBSPT_SOURCES ../src/*.c)

add_executable(runTests data_fifo_test.cpp ${LIBSPT_SOURCES})
target_link_libraries(runTests gtest gtest_main ${LIBEVENT_LIB} ${CMAKE_THREAD_LIBS_INIT})