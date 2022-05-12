#ifndef TEAM02_TRIANGLE_LISTER_H
#define TEAM02_TRIANGLE_LISTER_H

#include "common.h"
#include <iostream>
#include <set>


namespace TriangleListing {

template<class Index, class Counter = index_t>
struct Count {
    Counter count = 0;

    void list_triangle(Index a, Index b, Index c) {
        (void) a;
        (void) b;
        (void) c;
        ++count;
    }
};

template<class Index, class Counter = index_t>
struct Print {
#define SWAP(x, y) { const Index t = x; x = y; y = t; }
    void list_triangle(Index a, Index b, Index c) {
        if (a > c) SWAP(a, c);
        if (a > b) SWAP(a, b);
        if (b > c) SWAP(b, c);
        std::cout << a << " " << b << " " << c << std::endl;
    }

#undef SWAP
};

template<class Index, class Counter = index_t>
struct Collect {
    using Triangle = std::set<Index>;
    using TriangleSet = std::set<Triangle>;

    TriangleSet triangles;

    void list_triangle(Index a, Index b, Index c) {
        triangles.insert({a, b, c});
    }
};

}

#undef SWAP
#endif //TEAM02_TRIANGLE_LISTER_H
