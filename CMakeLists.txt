cmake_minimum_required(VERSION 3.10)
project(ASL-Team02 LANGUAGES CXX)

if (NOT CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_BUILD_TYPE "Release")
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message("${CMAKE_CXX_COMPILER_ID}")

set(CMAKE_CXX_STANDARD 20)

# Enable Warnings
set(WARNING_FLAGS "-Wall -Wextra -Wshadow -Wnon-virtual-dtor -Wpedantic -Werror")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${WARNING_FLAGS} -fconcepts")
set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} ${WARNING_FLAGS}")

# Release Flags
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}  -O3 -march=native")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}  ${WARNING_FLAGS}")

# Debug Flags
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${WARNING_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer")


include_directories(third_party)
include_directories(graphs)
include_directories(src)
include_directories(generated)
add_compile_definitions(INPUT_DIR="${PROJECT_SOURCE_DIR}/input/")

ADD_CUSTOM_COMMAND( OUTPUT ${CMAKE_SOURCE_DIR}/generated/instrumented_immintrin.h
                    COMMAND mkdir -p ${CMAKE_SOURCE_DIR}/generated && python3 ${CMAKE_SOURCE_DIR}/code_generator/code_generator.py ${CMAKE_SOURCE_DIR}/generated/instrumented_immintrin.h
                    DEPENDS code_generator/instrumented_intrinsics.json code_generator/code_generator.py
                    COMMENT "Generating instrumented_intrinsics." )

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
        tests/merge_sort_test.cpp
        tests/instrumented_index_test.cpp)

set(BENCH_SOURCES
        benchmark/main.cpp)

set(TRIANGLE_SOURCES
        src/instrumented_index.cpp
        
        src/edge_iterator/edge_iterator_base.cpp
        src/edge_iterator/edge_iterator_all_versions_instrumented.cpp
        src/edge_iterator/edge_iterator_v1.cpp
        src/edge_iterator/edge_iterator_v2.cpp
        src/edge_iterator/edge_iterator_u4.cpp

        src/forward/forward_base.cpp
        src/forward/forward_v1.cpp
        src/forward/forward_v2.cpp
        src/forward/forward_v3.cpp
        src/forward/forward_v4.cpp
        src/forward/forward_base_instrumented.cpp

        src/forward_hashed/forward_hashed_base.cpp
        src/forward_hashed/forward_hashed_all_versions_instrumented.cpp
        src/forward_hashed/forward_hashed_v1.cpp
        src/forward_hashed/forward_hashed_v2.cpp
        src/forward_hashed/forward_hashed_v3.cpp
        src/forward_hashed/forward_hashed_v4.cpp

        src/merge_sort/merge_sort_all_versions_instrumented.cpp
        src/merge_sort/merge_sort_base.cpp
        src/merge_sort/merge_sort_v2.cpp
        src/merge_sort/merge_sort_v3.cpp
        src/merge_sort/merge_sort_v4.cpp
        
        generated/instrumented_immintrin.h
        )

set(THIRD_PARTY_SOURCES
        third_party/arg_parser.cpp
        third_party/avx2-quicksort.cpp)

add_library(triangle_lib STATIC
        ${TRIANGLE_SOURCES})

add_library(third_party STATIC
        ${THIRD_PARTY_SOURCES})

add_library(graph_lib STATIC
        ${GRAPH_SOURCES})

add_executable(benchmark
        ${BENCH_SOURCES})
target_link_libraries(benchmark triangle_lib)
target_link_libraries(benchmark third_party)
target_link_libraries(benchmark graph_lib)

add_executable(graph_generation
        ${GRAPH_SOURCES})
target_link_libraries(graph_generation triangle_lib)
target_link_libraries(graph_generation third_party)
target_link_libraries(graph_generation graph_lib)

add_executable(tests
        ${TEST_SOURCES})
target_link_libraries(tests triangle_lib)
target_include_directories(tests PRIVATE src)

target_link_libraries(tests graph_lib)
add_dependencies(tests graph_generation)
