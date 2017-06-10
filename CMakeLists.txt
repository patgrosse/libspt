cmake_minimum_required(VERSION 3.0)
project(libspt)

macro(use_c99)
    if (CMAKE_VERSION VERSION_LESS "3.1")
        if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
            set (CMAKE_C_FLAGS "-std=gnu99 ${CMAKE_C_FLAGS}")
        endif ()
    else ()
        set (CMAKE_C_STANDARD 99)
    endif ()
endmacro(use_c99)

# flags
set(CMAKE_C_FLAGS "-Werror -Wall -Wextra")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} -g -Wfloat-equal -Wundef -Wshadow -Wcast-align \
-Wstrict-prototypes -Wstrict-overflow=5 -Wno-write-strings -Waggregate-return -Wcast-qual -Wswitch-default \
-Wswitch-enum -Wunreachable-code")
use_c99()

# dependencies
include(${CMAKE_CURRENT_LIST_DIR}/cmake/FindLibEvent.cmake)
include_directories(${CMAKE_CURRENT_LIST_DIR}/src/include)
find_package(Threads)

# files
set(SOURCE_FILES
        src/data_fifo.c
        src/tty_utils.c
        src/serial_io.c
        src/spt_logger.c
        src/spt.c
        src/base64.c)

# targets
add_library(libspt ${SOURCE_FILES})
target_link_libraries(libspt ${LIBEVENT_LIB})
target_link_libraries(libspt ${CMAKE_THREAD_LIBS_INIT})

add_executable(libspt_serialexample examples/serialexample.c)
target_link_libraries(libspt_serialexample libspt)

# tests
add_subdirectory(tests)
