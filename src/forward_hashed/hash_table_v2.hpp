#ifndef TEAM02_HASH_TABLE_V2_H
#define TEAM02_HASH_TABLE_V2_H

#include "common.h"
#include <cstring>
#include "hash_table_common.h"

namespace fh2 {

static constexpr size_t HASH_ITEM_SIZE = 4U;
static constexpr size_t HASH_CONTAINER_SIZE = HASH_TOTAL_SIZE / HASH_ITEM_SIZE;

// separate chaining

// a HashItem in a chain
template<class Index, class Counter = index_t>
struct HashItem {
    // the actual value (node id)
    Index number[HASH_ITEM_SIZE];
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
        for (Counter j = 0; j < HASH_ITEM_SIZE; j++) {
            table->head[i].number[j] = HASH_NULL_NUMBER;
        }
        table->head[i].next = NULL;
    }
}

template<class Index, class Counter = index_t>
void hashtable_insert(HashTable<Index> *table, Index x) {
    table->count++;
    Counter i = hash(x);
    HashItem<Index> *head = &table->head[i];

    for (Counter j = 0; j < HASH_ITEM_SIZE; j++) {
        if (head->number[j] == HASH_NULL_NUMBER) {
            head->number[j] = x;
            return;
        }
    }

    // Move all items to the new item
    HashItem<Index> *new_item = static_cast<HashItem<Index> *>(malloc(sizeof(HashItem<Index>)));
    for (Counter j = 0; j < HASH_ITEM_SIZE; j++) {
        new_item->number[j] = head->number[j];
    }
    new_item->next = head->next;
    head->next = new_item;

    head->number[0] = x;
    for (Counter j = 1; j < HASH_ITEM_SIZE; j++) {
        head->number[j] = HASH_NULL_NUMBER;
    }
}

template<class Index, class Counter = index_t>
bool hashtable_lookup(HashTable<Index> *table, Index x) {
    Counter i = hash(x);
    HashItem<Index> *head = &table->head[i];
//    int c = 0;
    do {
//        c++;
        for (Counter j = 0; j < HASH_ITEM_SIZE; j++) {
            if (head->number[j] == x) return true;
        }
        head = head->next;
    } while (head);
//    if (c > 1) std::cout << c << std::endl;
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
