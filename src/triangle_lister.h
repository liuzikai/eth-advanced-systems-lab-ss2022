#ifndef TEAM02_TRIANGLE_LISTER_H
#define TEAM02_TRIANGLE_LISTER_H

#include "common.h"
#include <iostream>
#include <set>

#define SWAP(x, y) { const index_t t = x; x = y; y = t; }

using Triangle = std::set<index_t>;
using TriangleSet = std::set<Triangle>;


namespace TriangleListing {
struct Count {
    index_t count = 0;

    void list_triangle(index_t a, index_t b, index_t c) {
        (void) a;
        (void) b;
        (void) c;
        ++count;
    }
};

struct Print {
    void list_triangle(index_t a, index_t b, index_t c) {
        if (a > c) SWAP(a, c);
        if (a > b) SWAP(a, b);
        if (b > c) SWAP(b, c);
        std::cout << a << " " << b << " " << c << std::endl;
    }
};


struct Collect {
    TriangleSet triangles;

    void list_triangle(index_t a, index_t b, index_t c) {
        triangles.insert({a, b, c});
    }
};

}

#undef SWAP
#endif //TEAM02_TRIANGLE_LISTER_H
