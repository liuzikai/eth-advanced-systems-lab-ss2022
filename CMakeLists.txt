cmake_minimum_required(VERSION 3.17)
project(ASL-Team02)

set(CMAKE_CXX_STANDARD 20)

# Enable Warnings
set(WARNING_FLAGS "-Wall -Wextra -Wshadow -Wnon-virtual-dtor -Wpedantic -Werror")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${WARNING_FLAGS}")
set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} ${WARNING_FLAGS}")

# Release Flags
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}  -O3")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}  ${WARNING_FLAGS}")

set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}  -O3")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}  ${WARNING_FLAGS}")

# Debug Flags
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${WARNING_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer")

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} ${WARNING_FLAGS}")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fno-omit-frame-pointer")

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
        src/common.cpp
        src/adjacency_graph.cpp
        src/forward.cpp
        src/edge_iterator.cpp)

add_executable(tests
        ${TEST_SOURCES})
target_include_directories(tests PRIVATE src)
TARGET_LINK_LIBRARIES(tests triangle_lib)

add_executable(benchmark
        ${BENCH_SOURCES})
TARGET_LINK_LIBRARIES(benchmark triangle_lib)
