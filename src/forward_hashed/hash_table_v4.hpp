#ifndef TEAM02_HASH_TABLE_V4_H
#define TEAM02_HASH_TABLE_V4_H

#include "common.h"
#include <cstring>
#include "instrumented_immintrin.h"

namespace fh4 {

// This is very ugly... But I don't want to deal with templates simply for a global variable...
#ifndef HASH_NULL_NUMBER
#define HASH_NULL_NUMBER ((Index) (-1))
#endif

//static constexpr size_t HASH_CONTAINER_SIZE = 2U;
//static constexpr size_t HASH_ITEM_SIZE = 8U;
//#define __mi __m256i
//#define _mm_set1_epi32 _mm256_set1_epi32
//#define _mm_cmpeq_epi32 _mm256_cmpeq_epi32
//#define _mm_movemask_epi8 _mm256_movemask_epi8

static constexpr size_t HASH_CONTAINER_SIZE = 4U;
static constexpr size_t HASH_ITEM_SIZE = 4U;
#define __mi __m128i
#define _mm_set1_epi32 _mm_set1_epi32
#define _mm_cmpeq_epi32 _mm_cmpeq_epi32
#define _mm_movemask_epi8 _mm_movemask_epi8

// separate chaining

// a HashItem in a chain
template<class Index, class Counter = index_t>
struct HashItem {
    // the actual value (node id)
    Index number[HASH_ITEM_SIZE];
    struct HashItem<Index> *next;
};

static_assert(sizeof(index_t) * HASH_ITEM_SIZE == sizeof(__mi), "Index number[HASH_ITEM_SIZE]");

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

    HashItem<Index> *new_item = static_cast<HashItem<Index> *>(malloc(sizeof(HashItem<Index>)));
    new_item->number[0] = x;
    for (Counter j = 1; j < HASH_ITEM_SIZE; j++) {
        new_item->number[j] = HASH_NULL_NUMBER;
    }
    new_item->next = head->next;
    head->next = new_item;

}

template<class Index, class Counter = index_t>
bool hashtable_lookup(HashTable<Index> *table, Index x) {
//    static const __mi zero = _mm256_setzero_si256();
    Counter i = hash(x);
    HashItem<Index> *head = &table->head[i];
//    int c = 0;
    do {
//        c++;
        __mi v = _mm_set1_epi32((index_t) x);
        __mi vec = *((__mi *) head->number);
        __mi cmp = _mm_cmpeq_epi32(v, vec);
        int mask = _mm_movemask_epi8(cmp);
        if (mask != 0) return true;
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

#undef __mi
#undef _mm_set1_epi32
#undef _mm_cmpeq_epi32
#undef _mm_movemask_epi8

}

#endif
