#ifndef HASH_TABLE_1H
#define HASH_TABLE_1H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static const size_t INIT_CAPACITY_ = 100;
static const double MAX_LOAD_FACTOR_ = 0.7;

typedef struct Node
{
    Node *next = nullptr;
    int key = 0;
}Node;

typedef struct
{
    Node **buckets = nullptr;
    size_t size = 0;
    size_t capacity = 0;
}Hash_table_1;

typedef struct 
{
    Node *prev = nullptr;
    Node *node = nullptr;
    size_t key_hash = 0;
}Keynode_info;

size_t hash (int x);
void ht_ctor (Hash_table_1 *ht, size_t capacity);
inline double load_factor (Hash_table_1 *ht);
Keynode_info find_key_info (Hash_table_1 *ht, const int key);
int is_contains (Hash_table_1 *ht, const int key);
Node *node_ctor (const int key);
int ht_insert (Hash_table_1 *ht, const int key);
int ht_remove (Hash_table_1 *ht, const int key);
void ht_dtor (Hash_table_1 *ht);
void rehash (Hash_table_1 *ht);

#endif /* HASH_TABLE_1H */