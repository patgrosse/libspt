cmake_minimum_required(VERSION 3.0)
project(libspt_tests)

macro(use_c99)
    if (CMAKE_VERSION VERSION_LESS "3.1")
        if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
            set (CMAKE_C_FLAGS "-std=gnu99 ${CMAKE_C_FLAGS}")
        endif ()
    else ()
        set (CMAKE_C_STANDARD 99)
    endif ()
endmacro(use_c99)

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