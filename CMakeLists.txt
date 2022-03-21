cmake_minimum_required(VERSION 3.17)
project(ASL-Team02)

set(CMAKE_CXX_STANDARD 20)

# Enable Warnings
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic")

# Release Flags
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}  -O3")

# Debug Flags
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer")

include_directories(third_party)
add_compile_definitions(INPUT_DIR="${PROJECT_SOURCE_DIR}/input/")

set(TEST_SOURCES
        tests/test_main.cpp
        tests/example_test.cpp
        tests/adjacent_graph_test.cpp
        tests/forward_test.cpp
        tests/edge_iterator_test.cpp)

set(BENCH_SOURCES
        benchmark/main.cpp)

ADD_LIBRARY(triangle_lib STATIC
        src/common.c
        src/adjacency_graph.c
        src/forward.c
        src/edge_iterator.c)

add_executable(tests
        ${TEST_SOURCES})
target_include_directories(tests PRIVATE src)
TARGET_LINK_LIBRARIES(tests triangle_lib)

add_executable(benchmark
        ${BENCH_SOURCES})
TARGET_LINK_LIBRARIES(benchmark triangle_lib)

