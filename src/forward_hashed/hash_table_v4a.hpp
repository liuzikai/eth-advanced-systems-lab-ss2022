#ifndef TEAM02_HASH_TABLE_V4A_H
#define TEAM02_HASH_TABLE_V4A_H

#include "common.h"
#include <cstring>
#include "instrumented_immintrin.h"
#include "hash_table_common.h"

namespace fh4a {

//static constexpr size_t HASH_CONTAINER_SIZE = 2U;
//static constexpr size_t HASH_ITEM_SIZE = 8U;
//#define mi __m256i
//#define mm_set1_epi32 _mm256_set1_epi32
//#define mm_cmpeq_epi32 _mm256_cmpeq_epi32
//#define mm_movemask_epi8 _mm256_movemask_epi8

static constexpr size_t HASH_ITEM_SIZE = 4U;
static constexpr size_t HASH_CONTAINER_SIZE = HASH_TOTAL_SIZE / HASH_ITEM_SIZE;
#define mi __m128i
#define mm_set1_epi32 _mm_set1_epi32
#define mm_load _mm_load_si128
#define mm_cmpeq_epi32 _mm_cmpeq_epi32
#define mm_movemask_epi8 _mm_movemask_epi8

// separate chaining

// a HashItem in a chain
template<class Index, class Counter = index_t>
struct HashItem {
    // the actual value (node id)
    __attribute__((__aligned__(16)))
    Index number[HASH_ITEM_SIZE];
    struct HashItem<Index> *next;
    struct HashItem<Index> *iter;
};

static_assert(sizeof(index_t) * HASH_ITEM_SIZE == sizeof(mi), "Index number[HASH_ITEM_SIZE]");

template<class Index, class Counter = index_t>
struct HashTable {
    Counter count;
    HashItem<Index> head[HASH_CONTAINER_SIZE];
    struct HashItem<Index> *iter_head;
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
        table->head[i].iter = NULL;
    }
    table->iter_head = NULL;
}

template<class Index, class Counter = index_t>
void hashtable_insert(HashTable<Index> *table, Index x) {
    table->count++;
    Counter i = hash(x);
    HashItem<Index> *head = &table->head[i];

    for (Counter j = 0; j < HASH_ITEM_SIZE; j++) {
        if (head->number[j] == HASH_NULL_NUMBER) {
            head->number[j] = x;
            if (j == 0) {
                head->iter = table->iter_head;
                table->iter_head = head;
            }
            return;
        }
    }

    // Move all items to the new item
    HashItem<Index> *new_item = new HashItem<Index>;
    for (Counter j = 0; j < HASH_ITEM_SIZE; j++) {
        new_item->number[j] = head->number[j];
    }
    new_item->next = head->next;
    head->next = new_item;

    new_item->iter = table->iter_head;
    table->iter_head = new_item;

    head->number[0] = x;
    for (Counter j = 1; j < HASH_ITEM_SIZE; j++) {
        head->number[j] = HASH_NULL_NUMBER;
    }
}

template<class Index, class Counter = index_t>
bool hashtable_lookup(HashTable<Index> *table, Index x) {
//    static const mi zero = _mm256_setzero_si256();
    Counter i = hash(x);
    HashItem<Index> *head = &table->head[i];
//    int c = 0;
    do {
//        c++;
        mi v = mm_set1_epi32((index_t) x);
        mi vec = mm_load((__m128i const *) head->number);
        mi cmp = mm_cmpeq_epi32(v, vec);
        int mask = mm_movemask_epi8(cmp);
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

#undef mi
#undef mm_set1_epi32
#undef mm_load
#undef mm_cmpeq_epi32
#undef mm_movemask_epi8

}

#endif
