#include "edge_iterator_v2.hpp"

namespace ei2 {
    template TriangleListing::Count<index_t> edge_iterator(AdjacencyGraph<index_t> *G, void *dummy = nullptr);
    template TriangleListing::Collect<index_t> edge_iterator(AdjacencyGraph<index_t> *G, void *dummy = nullptr);
}
