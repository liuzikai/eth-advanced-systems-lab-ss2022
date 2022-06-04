#include "edge_iterator_vec6.hpp"

namespace e_vec6 {
    template void edge_iterator(TriangleListing::Count<index_t>* tlr, AdjacencyGraph<index_t> *G, void *dummy = nullptr);
    template void edge_iterator(TriangleListing::Collect<index_t>* tlr, AdjacencyGraph<index_t> *G, void *dummy = nullptr);
}
