#ifndef TEAM02_HASH_TABLE_H
#define TEAM02_HASH_TABLE_H

#include <stdlib.h>
#include "common.h"

#define HASH_CONTAINER_SIZE 4

// separate chaining

// a HashItem in a chain
template<class Index>
struct HashItem {
    // the actual value (node id)
    Index number;
    struct HashItem<Index> *next;
};

template<class Index>
struct HashTable{
    Index size;
    // a linked list of "size"-many ptrs to a chain of HashItems
    HashItem<Index> **container;
};

template<class Index>
index_t hash(Index x);
// init the hash table and zero-init the container

template<class Index>
HashTable<Index> *create_hashtable();
// insert at the end of the corresponding chain

template<class Index>
void hashtable_insert(HashTable<Index>  *table, Index i);

// traverse the corresponding chain
template<class Index>
bool hashtable_lookup(HashTable<Index>  *table, Index i);

// clear all the chains
template<class Index>
void hashtable_clear(HashTable<Index> *table);

// free all the chains and then free the table
template<class Index>
void free_hashtable(HashTable<Index>  *table);

#endif //TEAM02_HASH_TABLE_H
