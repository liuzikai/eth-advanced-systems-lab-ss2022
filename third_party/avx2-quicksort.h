#include <cstdint>

#define FORCE_INLINE __attribute__((always_inline)) inline

// This is from https://github.com/WojciechMula/simd-sort
namespace qs {

    namespace avx2 {

        void quicksort(uint32_t* array, int left, int right);

    } // namespace avx2

} // namespace qs


#include "triangle_lister.h"
#include "adjacency_graph.h"

template<class Index, class Counter = index_t, class TRL = TriangleListing::Count<Index>>
TRL WojciechMula_sort_timing(AdjacencyGraph<Index> *G, void *dummy) {
    (void) dummy;
    // According to sec. 4, the sorting is included in the execution time
    for (Counter u = 0; u < G->n; u++) {
        if (G->adjacency[u].count > 0) {
            qs::avx2::quicksort(G->adjacency[u].neighbors, 0, G->adjacency[u].count);
        }
    }
    TRL res;
    return res;
}