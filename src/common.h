#ifndef TEAM02_COMMON_H
#define TEAM02_COMMON_H

#include <stdint.h>
#include <inttypes.h>

typedef uint64_t index_t;  // XXX: smaller int for vectorization
#define INDEX_FMT "%" PRIu64

// XXX: replace with empty define when benchmarking
#define print_triangle print_triangle_func

typedef void (*print_triangle_func_t)(index_t u, index_t v, index_t w);

// Function pointer to allow easy replacement by tests (use just like function)
extern print_triangle_func_t print_triangle_func;

/**
 * Print a triangle to stdout. Node indices are ordered automatically.
 * @param a
 * @param b
 * @param c
 */
void print_triangle_printf(index_t a, index_t b, index_t c);

#endif //TEAM02_COMMON_H
