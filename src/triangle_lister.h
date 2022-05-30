#ifndef TEAM02_TRIANGLE_LISTER_H
#define TEAM02_TRIANGLE_LISTER_H

#include "common.h"
#include <iostream>
#include <set>
#include "instrumented_index.h"

namespace TriangleListing {


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
struct SetCollect {
    using Triangle = std::set<index_t>;
    using TriangleSet = std::set<Triangle>;

    TriangleSet triangles;

    void list_triangle(Index a, Index b, Index c) {
        triangles.insert({(index_t)a, (index_t)b, (index_t)c});
    }
};

template<class Index, class Counter = index_t>
struct Count {
    Count(size_t dummy) {
        count = 0;
    }
    Counter count;

    __attribute__((always_inline)) void list_triangle(Index a, Index b, Index c) {
        (void) a;
        (void) b;
        (void) c;
        ++count;
    }
};


template<class Index, class Counter = index_t>
struct Collect {

    struct Triangle {
        uint32_t a, b, c;
    };


    Collect(size_t max_triangles) {
        pos = 0;
        triangles = mallocl(sizeof(Triangle) * max_triangles);
    }
    
    size_t pos;
    // Yes this is a static array. If you want to keep the result call to set.
    Triangle* triangles;


    void list_triangle(Index a, Index b, Index c) {
        triangles[pos] = Triangle{(index_t)a, (index_t)b, (index_t)c};
        ++pos;
        
    }

    using SetTriangle = std::set<index_t>;
    using TriangleSet = std::set<SetTriangle>;

    TriangleSet to_set() {
        TriangleSet set;
        for (auto &triangle : triangles) {
            set.insert({triangle.a, triangle.b, triangle.c});
        }
        return set;
    }
};

}

#undef SWAP
#endif //TEAM02_TRIANGLE_LISTER_H
