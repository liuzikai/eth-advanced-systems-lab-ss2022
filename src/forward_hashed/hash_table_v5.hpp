#ifndef TEAM02_HASH_TABLE_V5_H
#define TEAM02_HASH_TABLE_V5_H

#include "common.h"
#include <cstring>
#include "instrumented_immintrin.h"
#include "hash_table_common.h"

namespace fh5 {

//static constexpr size_t HASH_CONTAINER_SIZE = 2U;
//static constexpr size_t HASH_ITEM_SIZE = 8U;
//#define __mi __m256i
//#define _mm_set1_epi32 _mm256_set1_epi32
//#define _mm_cmpeq_epi32 _mm256_cmpeq_epi32
//#define _mm_movemask_epi8 _mm256_movemask_epi8

static constexpr size_t HASH_ITEM_SIZE = 4U;
static constexpr size_t HASH_CONTAINER_SIZE = HASH_TOTAL_SIZE / HASH_ITEM_SIZE;
#define __mi __m128i
#define _mm_set1_epi32 _mm_set1_epi32
#define mm_load _mm_load_si128
#define _mm_cmpgt_epi32 _mm_cmpgt_epi32
#define _mm_cmpeq_epi32 _mm_cmpeq_epi32
#define _mm_movemask_epi8 _mm_movemask_epi8

// separate chaining

// a HashItem in a chain
template<class Index, class Counter = index_t>
struct HashItem {
    // the actual value (node id)
    __attribute__((__aligned__(16)))
    Index number[HASH_ITEM_SIZE];
    struct HashItem<Index> *next;
};

static_assert(sizeof(index_t) * HASH_ITEM_SIZE == sizeof(__mi), "Index number[HASH_ITEM_SIZE]");

template<class Index, class Counter = index_t>
struct HashTable {
    HashItem<Index> head[HASH_CONTAINER_SIZE];
    Counter count;
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
//        table->head[i].tail = 0;
        table->head[i].next = NULL;
    }
}

template<class Index, class Counter = index_t>
void hashtable_insert(HashTable<Index> *table, Index x) {
    table->count++;
    Counter i = hash(x);
    HashItem<Index> *head = &table->head[i];

    static __mi NULL_VEC = _mm_set1_epi32((index_t) HASH_NULL_NUMBER);

    __mi vec = mm_load((const __m128i_u *) &head->number);

    // Signed comparison. Any node is greater than -1, so mask will be some 1's at the lsb and some 0's at msb
    __mi gt = _mm_cmpgt_epi32(vec, NULL_VEC);

    int mask = _mm_movemask_epi8(gt);

//    std::cout << "mask = " << mask << ", cnt = " << cnt << std::endl;

    if (mask != 0xFFFF) {
        Counter cnt = __builtin_clz(mask) / 4 - 4;
        head->number[HASH_ITEM_SIZE - cnt] = x;
        return;
    }

//    for (Counter j = 0; j < HASH_ITEM_SIZE; j++) {
//        if (head->number[j] == HASH_NULL_NUMBER) {
//            head->number[j] = x;
//            return;
//        }
//    }

    HashItem<Index> *new_item = static_cast<HashItem<Index> *>(malloc(sizeof(HashItem<Index>)));
    new_item->number[0] = x;
    for (Counter j = 1; j < HASH_ITEM_SIZE; j++) {
        new_item->number[j] = HASH_NULL_NUMBER;
    }
    new_item->next = head->next;
    head->next = new_item;

}

template<class Index, class Counter = index_t>
bool hashtable_lookup(const HashTable<Index> *table, Index x) {
//    static const __mi zero = _mm256_setzero_si256();
    Counter i = hash(x);
    const HashItem<Index> *head = &table->head[i];
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
