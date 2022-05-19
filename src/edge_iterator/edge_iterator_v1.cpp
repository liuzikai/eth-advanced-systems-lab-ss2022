#include "edge_iterator_v1.hpp"

namespace ei1 {
    template TriangleListing::Count<index_t> edge_iterator(AdjacencyGraph<index_t> *G, void *dummy = nullptr);
    template TriangleListing::Collect<index_t> edge_iterator(AdjacencyGraph<index_t> *G, void *dummy = nullptr);
}
