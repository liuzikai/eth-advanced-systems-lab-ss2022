#include <edge_iterator/edge_iterator_va.hpp>

namespace eia {
    template TriangleListing::Count<index_t> edge_iterator(AdjacencyGraph<index_t> *G, void *dummy = nullptr);
    template TriangleListing::Collect<index_t> edge_iterator(AdjacencyGraph<index_t> *G, void *dummy = nullptr);
}
