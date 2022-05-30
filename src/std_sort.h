
#include <quick_cut_sort.h>


#include <algorithm>

template<class Index, class Counter = index_t, class TRL = TriangleListing::Count<Index>>
void std_sort_timing(TRL*, AdjacencyGraph<Index> *G, void *dummy) {
    (void) dummy;
    // According to sec. 4, the sorting is included in the execution time
    for (Counter u = 0; u < G->n; u++) {
        if (G->adjacency[u].count > 0) {
            quick_cut<Index>(G->adjacency[u].neighbors, 0, G->adjacency[u].count - 1, (Index) u, &G->adjacency[u].count);
            std::sort(G->adjacency[u].neighbors, G->adjacency[u].neighbors + G->adjacency[u].count);
            //quick_sort(G->adjacency[u].neighbors, 0, G->adjacency[u].count - 1);
        }
    }
}
