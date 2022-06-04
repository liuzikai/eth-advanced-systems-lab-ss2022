#ifndef TEAM02_HASH_TABLE_V1_H
#define TEAM02_HASH_TABLE_V1_H

#include "common.h"
#include <cstring>
#include "hash_table_common.h"

namespace fh1 {

static constexpr size_t HASH_CONTAINER_SIZE = HASH_TOTAL_SIZE;

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
    Counter count;
    HashItem<Index> head[HASH_CONTAINER_SIZE];
};

// super naive hash function
// optimization?: a better hash function with fewer collisions, https://stackoverflow.com/questions/664014/what-integer-hash-function-are-good-that-accepts-an-integer-hash-key
template<class Index, class Counter = index_t>
Counter hash(Index x) {
    x = x % ((Index) HASH_CONTAINER_SIZE);
    return (Counter) x;
}

// use a uniform head size
// optimization?: the hash table of each node will use at most adj->count many slots
template<class Index, class Counter = index_t>
HashTable<Index> *create_hashtable() {
    HashTable<Index> *table = new HashTable<Index>;
    hashtable_init(table);
    return table;
}

template<class Index, class Counter = index_t>
void hashtable_init(HashTable<Index> *table) {
    table->count = 0;
    for (Counter i = 0; i < HASH_CONTAINER_SIZE; i++) {
        table->head[i].number = HASH_NULL_NUMBER;
        table->head[i].next = NULL;
    }
}

template<class Index, class Counter = index_t>
void hashtable_insert(HashTable<Index> *table, Index x) {
    table->count++;
    Counter i = hash(x);
    HashItem<Index> *head = &table->head[i];
    if (head->number == HASH_NULL_NUMBER) {
        head->number = x;
    } else {
        HashItem<Index> *new_item = static_cast<HashItem<Index> *>(malloc(sizeof(HashItem<Index>)));
        new_item->number = x;
        new_item->next = head->next;
        head->next = new_item;
    }
}

template<class Index, class Counter = index_t>
bool hashtable_lookup(HashTable<Index> *table, Index x) {
    Counter i = hash(x);
    HashItem<Index> *head = &table->head[i];
    do {
        if (head->number == x) return true;
        head = head->next;
    } while (head);
    return false;
}

template<class Index, class Counter = index_t>
void hashtable_clear(HashTable<Index> *table) {
    for (Counter i = 0; i < HASH_CONTAINER_SIZE; i++) {
        HashItem<Index> *head = table->head[i].next;
        HashItem<Index> *temp;
        while (head) {
            temp = head;
            head = head->next;
            free(temp);
        }
    }
    hashtable_init(table);
}

template<class Index, class Counter = index_t>
void free_hashtable(HashTable<Index> *table) {
    hashtable_clear(table);
    free(table);
}

}

#endif
