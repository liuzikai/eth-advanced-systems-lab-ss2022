#include <catch.hpp>
#include "common.h"

#include <set>

using Triangle = std::set<index_t>;
using TriangleSet = std::set<Triangle>;
extern TriangleSet triangles;
static inline void add_triangle(index_t u, index_t v, index_t w) {
    triangles.insert({u, v, w});
}
