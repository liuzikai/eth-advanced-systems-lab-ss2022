cmake_minimum_required(VERSION 3.17)
project(team02)

set(CMAKE_CXX_STANDARD 20)

# Enable Warnings
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic")

#
# RELEASE FLAGS
#
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}  -O3")

#
# DEBUG FLAGS
#
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer")


include_directories(third_party)

set(TEST_SOURCES
        tests/test_main.cpp
        tests/example_test.cpp)

set(BENCH_SOURCES
        benchmark/main.cpp)

ADD_LIBRARY(triangle_lib STATIC
            src/EdgeIterator.cpp )

add_executable(tests
        ${TEST_SOURCES})

add_executable(benchmark
        ${BENCH_SOURCES})

TARGET_LINK_LIBRARIES(tests triangle_lib)
TARGET_LINK_LIBRARIES(benchmark triangle_lib)
