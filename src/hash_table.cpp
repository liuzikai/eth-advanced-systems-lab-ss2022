#include <stdlib.h>
#include <stdio.h>
#include "common.h"
#include "hash_table.h"

// separate chaining

// super naive hash function
// optimization?: a better hash function with fewer collisions, https://stackoverflow.com/questions/664014/what-integer-hash-function-are-good-that-accepts-an-integer-hash-key
index_t hash(index_t x) {
    x = x % HASH_CONTAINER_SIZE;
    return x;
}

// use a uniform container size
// optimization?: the hash table of each node will use at most adj->count many slots
void hashtable_init(hash_table_t *table) {
    table->container = static_cast<hash_item_t**>(malloc(sizeof(*table->container) * HASH_CONTAINER_SIZE));
    table->size = HASH_CONTAINER_SIZE;
    for (index_t i=0; i < table->size; i++) {
        table->container[i] = nullptr;
    }
}

void hashtable_insert(hash_table_t *table, index_t x) {
    index_t i = hash(x);
    hash_item_t *head = table->container[i];
    hash_item_t *new_item = static_cast<hash_item_t *>(malloc(sizeof(hash_item_t)));
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

bool hashtable_lookup(hash_table_t *table, index_t x) {
    index_t i = hash(x);
    hash_item_t *head = table->container[i];
    while (head) {
        if (head->number == x) return true;
        head = head->next;
    }
    return false;
}

void free_hashtable(hash_table_t *table) {
    for (index_t i=0; i < table->size; i++) {
        hash_item_t *head = table->container[i];
        hash_item_t *temp;
        while (head) {
            temp = head;
            head = head->next;
            free(temp);
        }
    }
    free(table);
}
