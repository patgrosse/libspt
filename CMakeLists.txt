cmake_minimum_required(VERSION 3.6)
project(libspt)

# flags
set(CMAKE_C_FLAGS "-Werror -Wall -Wextra -std=gnu99")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} -Wfloat-equal -Wundef -Wshadow -Wcast-align \
-Wstrict-prototypes -Wstrict-overflow=5 -Wno-write-strings -Waggregate-return -Wcast-qual -Wswitch-default \
-Wswitch-enum -Wunreachable-code")
set(CMAKE_C_STANDARD 99)

# dependencies
include(${CMAKE_SOURCE_DIR}/cmake/FindLibEvent.cmake)
include_directories(${CMAKE_SOURCE_DIR}/src/include)
find_package(Threads)

# files
set(SOURCE_FILES
        examples/serialexample.c
        src/data_fifo.c
        src/tty_utils.c
        src/serial_io.c
        src/spt_logger.c
        src/spt.c
        src/base64.c)

# targets
add_executable(serialexample ${SOURCE_FILES})
target_link_libraries(serialexample ${LIBEVENT_LIB})
target_link_libraries(serialexample ${CMAKE_THREAD_LIBS_INIT})

# tests
add_subdirectory(tests)
