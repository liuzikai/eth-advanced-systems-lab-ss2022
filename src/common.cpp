#include "common.h"
#include <iostream>

print_triangle_func_t print_triangle_func = print_triangle_printf;

void print_triangle_printf(index_t a, index_t b, index_t c) {
#define SWAP(x, y) { const index_t t = x; x = y; y = t; }
    if (a > c) SWAP(a, c);
    if (a > b) SWAP(a, b);
    if (b > c) SWAP(b, c);
#undef SWAP
    std::cout << a << " " << b << " " << c << std::endl;
}
