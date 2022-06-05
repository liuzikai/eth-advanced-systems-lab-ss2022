#ifndef TEAM02_HASH_TABLE_STD_H
#define TEAM02_HASH_TABLE_STD_H

#include "common.h"
#include <cstring>
#include <unordered_set>

namespace fhstd {

static constexpr size_t HASH_CONTAINER_SIZE = 4U;

template<class Index, class Counter = index_t>
using HashTable = std::unordered_set<Index>;

// use a uniform container size
// optimization?: the hash table of each node will use at most adj->count many slots
template<class Index, class Counter = index_t>
HashTable<Index> *create_hashtable() {
    HashTable<Index> *table = new HashTable<Index>;
    return table;
}

//template<class Index, class Counter = index_t>
//void hashtable_init(HashTable<Index> *table) {
//
//}

template<class Index, class Counter = index_t>
void hashtable_insert(HashTable<Index> *table, Index x) {
    table->insert(x);
}

template<class Index, class Counter = index_t>
bool hashtable_lookup(HashTable<Index> *table, Index x) {
    return table->contains(x);
}

template<class Index, class Counter = index_t>
void hashtable_clear(HashTable<Index> *table) {
    table->clear();
}

template<class Index, class Counter = index_t>
void free_hashtable(HashTable<Index> *table) {
    free(table);
}

}

#endif
