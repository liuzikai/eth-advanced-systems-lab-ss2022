#include <catch.hpp>
extern "C" {
#include "common.h"
}
#include <set>

using Triangle = std::set<index_t>;
using TriangleSet = std::set<Triangle>;
TriangleSet triangles;
extern "C" {
    static void add_triangle(index_t u, index_t v, index_t w) {
        triangles.insert({u, v, w});
    }
}