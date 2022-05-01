cmake_minimum_required(VERSION 3.10)
project(ASL-Team02)

MESSAGE("${CMAKE_CXX_COMPILER_ID}")

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
include_directories(graphs)
add_compile_definitions(INPUT_DIR="${PROJECT_SOURCE_DIR}/input/")

set(GRAPH_SOURCES
        graphs/graph_generation.cpp
        graphs/graph_generation.h
        graphs/graph_generation_util.cpp
        graphs/adjacency_graph.cpp)

set(TEST_SOURCES
        tests/test_main.cpp
        tests/example_test.cpp
        tests/adjacent_graph_test.cpp
        tests/forward_test.cpp
        tests/edge_iterator_test.cpp
        tests/forward_hashed_test.cpp
        tests/instrumented_index_test.cpp)

set(BENCH_SOURCES
        benchmark/main.cpp)

set(TRIANGLE_SOURCES
        src/instrumented_index.cpp
        src/hash_table.cpp
        src/forward.cpp
        src/edge_iterator.cpp
        src/forward_hashed.cpp)

set(THIRD_PARTY_SOURCES
        third_party/ArgParser.cpp)

ADD_LIBRARY(triangle_lib STATIC
        ${TRIANGLE_SOURCES})

ADD_LIBRARY(third_party STATIC
        ${THIRD_PARTY_SOURCES})

ADD_LIBRARY(graph_lib STATIC
        ${GRAPH_SOURCES})

add_executable(benchmark
        ${BENCH_SOURCES})
TARGET_LINK_LIBRARIES(benchmark triangle_lib)
TARGET_LINK_LIBRARIES(benchmark third_party)
TARGET_LINK_LIBRARIES(benchmark graph_lib)

add_executable(graph_generation
        ${GRAPH_SOURCES})
TARGET_LINK_LIBRARIES(graph_generation triangle_lib)
TARGET_LINK_LIBRARIES(graph_generation third_party)
TARGET_LINK_LIBRARIES(graph_generation graph_lib)

add_executable(tests
        ${TEST_SOURCES})
target_include_directories(tests PRIVATE src)
TARGET_LINK_LIBRARIES(tests triangle_lib)
TARGET_LINK_LIBRARIES(tests graph_lib)
