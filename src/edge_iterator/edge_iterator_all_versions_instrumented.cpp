#define INSTRUMENTED
#include "edge_iterator_base.hpp"
#include "edge_iterator_v1.hpp"
#include "edge_iterator_v2.hpp"
#include "edge_iterator_u4.hpp"
#include "edge_iterator_u5.hpp"
#include "edge_iterator_u6.hpp"
#include "edge_iterator_u7.hpp"
#include "edge_iterator_m1.hpp"
#include "edge_iterator_va.hpp"
#include "instrumented_index.h"


namespace ei0 {
    template void edge_iterator(TriangleListing::Count<InstrumentedIndex>* tlr, AdjacencyGraph<InstrumentedIndex> *G, void *dummy = nullptr);
    template void edge_iterator(TriangleListing::SetCollect<InstrumentedIndex>* tlr, AdjacencyGraph<InstrumentedIndex> *G, void *dummy = nullptr);
}

namespace ei1 {
    template void edge_iterator(TriangleListing::Count<InstrumentedIndex>* tlr, AdjacencyGraph<InstrumentedIndex> *G, void *dummy = nullptr);
    template void edge_iterator(TriangleListing::SetCollect<InstrumentedIndex>* tlr, AdjacencyGraph<InstrumentedIndex> *G, void *dummy = nullptr);
}

namespace ei2 {
    template void edge_iterator(TriangleListing::Count<InstrumentedIndex>* tlr, AdjacencyGraph<InstrumentedIndex> *G, void *dummy = nullptr);
    template void edge_iterator(TriangleListing::SetCollect<InstrumentedIndex>* tlr, AdjacencyGraph<InstrumentedIndex> *G, void *dummy = nullptr);
}

namespace eu4  {
    template void edge_iterator(TriangleListing::Count<InstrumentedIndex>* tlr, AdjacencyGraph<InstrumentedIndex> *G, void *dummy = nullptr);
    template void edge_iterator(TriangleListing::SetCollect<InstrumentedIndex>* tlr, AdjacencyGraph<InstrumentedIndex> *G, void *dummy = nullptr);
}

namespace eu5  {
    template void edge_iterator(TriangleListing::Count<InstrumentedIndex>* tlr, AdjacencyGraph<InstrumentedIndex> *G, void *dummy = nullptr);
    template void edge_iterator(TriangleListing::SetCollect<InstrumentedIndex>* tlr, AdjacencyGraph<InstrumentedIndex> *G, void *dummy = nullptr);
}

namespace eu6  {
    template void edge_iterator(TriangleListing::Count<InstrumentedIndex>* tlr, AdjacencyGraph<InstrumentedIndex> *G, void *dummy = nullptr);
    template void edge_iterator(TriangleListing::SetCollect<InstrumentedIndex>* tlr, AdjacencyGraph<InstrumentedIndex> *G, void *dummy = nullptr);
}

namespace eu7  {
    template void edge_iterator(TriangleListing::Count<InstrumentedIndex>* tlr, AdjacencyGraph<InstrumentedIndex> *G, void *dummy = nullptr);
    template void edge_iterator(TriangleListing::SetCollect<InstrumentedIndex>* tlr, AdjacencyGraph<InstrumentedIndex> *G, void *dummy = nullptr);
}

namespace em1  {
    template void edge_iterator(TriangleListing::Count<InstrumentedIndex>* tlr, AdjacencyGraph<InstrumentedIndex> *G, void *dummy = nullptr);
    template void edge_iterator(TriangleListing::SetCollect<InstrumentedIndex>* tlr, AdjacencyGraph<InstrumentedIndex> *G, void *dummy = nullptr);
}

namespace eia {
    template void edge_iterator(TriangleListing::Count<InstrumentedIndex>* tlr, AdjacencyGraph<InstrumentedIndex> *G, void *dummy = nullptr);
    template void edge_iterator(TriangleListing::SetCollect<InstrumentedIndex>* tlr, AdjacencyGraph<InstrumentedIndex> *G, void *dummy = nullptr);
}
