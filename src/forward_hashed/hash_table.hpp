#ifndef TEAM02_HASH_TABLE_H
#define TEAM02_HASH_TABLE_H

#include "common.h"
#include <cstring>

namespace forward_hashed_base {

static constexpr size_t HASH_CONTAINER_SIZE = 4U;

// separate chaining

// a HashItem in a chain
template<class Index, class Counter = index_t>
struct HashItem {
    // the actual value (node id)
    Index number;
    struct HashItem<Index> *next;
};

template<class Index, class Counter = index_t>
struct HashTable {
    Counter size;
    // a linked list of "size"-many ptrs to a chain of HashItems
    HashItem<Index> **container;
};

// super naive hash function
// optimization?: a better hash function with fewer collisions, https://stackoverflow.com/questions/664014/what-integer-hash-function-are-good-that-accepts-an-integer-hash-key
template<class Index, class Counter = index_t>
Counter hash(Index x) {
    x = x % ((Index) HASH_CONTAINER_SIZE);
    return (Counter) x;
}

// use a uniform container size
// optimization?: the hash table of each node will use at most adj->count many slots
template<class Index, class Counter = index_t>
HashTable<Index> *create_hashtable() {
    HashTable<Index> *table = new HashTable<Index>;
    table->container = static_cast<HashItem<Index> **>(calloc(HASH_CONTAINER_SIZE,
                                                              sizeof(*table->container)));  // nullptr
    table->size = HASH_CONTAINER_SIZE;
    return table;
}

template<class Index, class Counter = index_t>
void hashtable_insert(HashTable<Index> *table, Index x) {
    Counter i = hash(x);
    HashItem<Index> *head = table->container[i];
    HashItem<Index> *new_item = static_cast< HashItem<Index> *>(malloc(sizeof(HashItem<Index>)));
    new_item->number = x;
    new_item->next = NULL;
    if (head == nullptr) {
        table->container[i] = new_item;
    } else {
        while (head->next) {
            head = head->next;
        }
        head->next = new_item;
    }
}

template<class Index, class Counter = index_t>
bool hashtable_lookup(HashTable<Index> *table, Index x) {
    Counter i = hash(x);
    HashItem<Index> *head = table->container[i];
    while (head) {
        if (head->number == x) return true;
        head = head->next;
    }
    return false;
}

template<class Index, class Counter = index_t>
void hashtable_clear(HashTable<Index> *table) {
    for (Counter i = 0; i < table->size; i++) {
        HashItem<Index> *head = table->container[i];
        HashItem<Index> *temp;
        while (head) {
            temp = head;
            head = head->next;
            free(temp);
        }
    }
    memset(table->container, 0, sizeof(*table->container) * HASH_CONTAINER_SIZE);
}

template<class Index, class Counter = index_t>
void free_hashtable(HashTable<Index> *table) {
    hashtable_clear(table);
    free(table->container);
    free(table);
}

}

#endif //TEAM02_HASH_TABLE_H
