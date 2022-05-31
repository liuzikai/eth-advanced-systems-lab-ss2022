#include "edge_iterator_u5.hpp"

namespace eu5 {
    template void edge_iterator(TriangleListing::Count<index_t>* tlr, AdjacencyGraph<index_t> *G, void *dummy = nullptr);
    template void edge_iterator(TriangleListing::Collect<index_t>* tlr, AdjacencyGraph<index_t> *G, void *dummy = nullptr);
}
