#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include "common.h"
#include "hash_table.h"
#include "instrumented_index.h"

// separate chaining

// super naive hash function
// optimization?: a better hash function with fewer collisions, https://stackoverflow.com/questions/664014/what-integer-hash-function-are-good-that-accepts-an-integer-hash-key
template<class Index>
index_t hash(Index x) {
    x = x % HASH_CONTAINER_SIZE;
    return x;
}

// use a uniform container size
// optimization?: the hash table of each node will use at most adj->count many slots
template<class Index>
HashTable<Index> *create_hashtable() {
    HashTable<Index> *table = new HashTable<Index>;
    table->container = static_cast<HashItem<Index> **>(calloc(HASH_CONTAINER_SIZE, sizeof(*table->container)));  // nullptr
    table->size = HASH_CONTAINER_SIZE;
    return table;
}

template<class Index>
void hashtable_insert(HashTable<Index> *table, Index x) {
    Index i = hash(x);
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

template<class Index>
bool hashtable_lookup(HashTable<Index> *table, Index x) {
    Index i = hash(x);
    HashItem<Index> *head = table->container[i];
    while (head) {
        if (head->number == x) return true;
        head = head->next;
    }
    return false;
}

template<class Index>
void hashtable_clear(HashTable<Index> *table) {
    for (Index i = 0; i < table->size; i++) {
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

template<class Index>
void free_hashtable(HashTable<Index> *table) {
    hashtable_clear(table);
    free(table->container);
    free(table);
}



template index_t hash(index_t x);
template HashTable<index_t> *create_hashtable();
template void hashtable_insert(HashTable<index_t>  *table, index_t i);
template bool hashtable_lookup(HashTable<index_t>  *table, index_t i);
template void hashtable_clear(HashTable<index_t> *table);
template void free_hashtable(HashTable<index_t>  *table);

template index_t hash(InstrumentedIndex x);
template HashTable<InstrumentedIndex> *create_hashtable();
template void hashtable_insert(HashTable<InstrumentedIndex>  *table, InstrumentedIndex i);
template bool hashtable_lookup(HashTable<InstrumentedIndex>  *table, InstrumentedIndex i);
template void hashtable_clear(HashTable<InstrumentedIndex> *table);
template void free_hashtable(HashTable<InstrumentedIndex>  *table);
