#include "edge_iterator_u4.hpp"

namespace eu4 {
    template TriangleListing::Count<index_t> edge_iterator(AdjacencyGraph<index_t> *G, void *dummy = nullptr);
    template TriangleListing::Collect<index_t> edge_iterator(AdjacencyGraph<index_t> *G, void *dummy = nullptr);
}
