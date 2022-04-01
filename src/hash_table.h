#ifndef TEAM02_HASH_TABLE_H
#define TEAM02_HASH_TABLE_H

#include <stdlib.h>
#include "common.h"

#define HASH_CONTAINER_SIZE 4

// separate chaining

// a hash_item in a chain
typedef struct hash_item {
    // the actual value (node id)
    index_t number;
    struct hash_item *next;
} hash_item_t;

typedef struct {
    index_t size;
    // a linked list of "size"-many ptrs to a chain of hash_items
    hash_item_t **container;
} hash_table_t;

index_t hash(index_t x);
// init the hash table and zero-init the container
void hashtable_init(hash_table_t *table);
// insert at the end of the corresponding chain
void hashtable_insert(hash_table_t *table, index_t i);
// traverse the corresponding chain
bool hashtable_lookup(hash_table_t *table, index_t i);
// free all the chains and then free the table
void free_hashtable(hash_table_t *table);

#endif //TEAM02_HASH_TABLE_H
