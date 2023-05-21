#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "hash_table_1.h"

void ht_ctor (Hash_table_1 *ht, size_t capacity)
{
    assert (ht);

    ht->buckets = (Node **)calloc (capacity, sizeof (Node *));
    assert (ht->buckets);

    ht->size = 0;
    ht->capacity = capacity;
}

inline double load_factor (Hash_table_1 *ht)
{
    return (double)ht->size / ht->capacity;
}

size_t hash (int x)
{
    size_t result = 0;
    
    memcpy (&result, &x, sizeof (int));

    return result;
}

Keynode_info find_key_info (Hash_table_1 *ht, const int key)
{
    assert (ht && ht->buckets);

    size_t hash_value = hash (key) % ht->capacity;

    Keynode_info info = {};

    Node *node = ht->buckets[hash_value];
    Node *prev = nullptr;

    while (node)
    {
        if (node->key == key)
        {
            break;
        }
        prev = node;
        node = node->next;
    }

    info.node = node;
    info.prev = prev;
    info.key_hash = hash_value;

    return info;
}

Node *node_ctor (const int key)
{
    Node *node = (Node *)calloc (1, sizeof (Node));
    assert (node);

    node->key = key;

    return node;
}

int ht_insert (Hash_table_1 *ht, const int key)
{
    assert (ht && ht->buckets);

    if (load_factor (ht) > MAX_LOAD_FACTOR_)
    {
        rehash (ht);
    }
    
    Keynode_info info = find_key_info (ht, key);

    if (!(info.node))
    {
        if (!(info.prev))
        {
            ht->buckets[info.key_hash] = node_ctor (key);
        }
        else 
        {
            info.prev->next = node_ctor (key);
        }

        ++ht->size;
        return EXIT_SUCCESS;
    }

    return EXIT_FAILURE;
}

int ht_remove (Hash_table_1 *ht, const int key)
{
    assert (ht && ht->buckets);

    Keynode_info info = find_key_info (ht, key);

    if (info.node && info.node->key == key)
    {
        if (!info.prev)
        {
            ht->buckets[info.key_hash] = info.node->next;
        }
        else
        {
            info.prev->next = info.node->next;
        }

        free (info.node);

        --ht->size;
        
        return EXIT_SUCCESS;
    }   

    return EXIT_FAILURE;
}

int is_contains (Hash_table_1 *ht, const int key)
{
    assert (ht && ht->buckets);
    
    Keynode_info info = find_key_info (ht, key);
    
    return info.node && info.node->key == key;
}

void rehash (Hash_table_1 *ht)
{
    assert (ht && ht->buckets);

    size_t new_capacity = ht->capacity * 2;

    Node **new_buckets = (Node **)calloc (new_capacity, sizeof (Node *));
    assert (new_buckets);

    for (size_t list_idx = 0; list_idx < ht->capacity; ++list_idx)
    {
        Node *node = ht->buckets[list_idx];
        while (node)
        {
            size_t hash_value = hash (node->key) % new_capacity;
            
            Node *new_node = node_ctor (node->key);
            new_node->next = new_buckets[hash_value];
            new_buckets[hash_value] = new_node;

            Node *prev = node;
            node = node->next;
            free (prev);
        }
    }
    
    free (ht->buckets);

    ht->buckets = new_buckets;
    ht->capacity = new_capacity;
}

void ht_dtor (Hash_table_1 *ht)
{
    if (!ht)
        return;
    if (!ht->buckets)
    {
        ht = nullptr;
        return;
    }

    for (size_t list_idx = 0; list_idx < ht->capacity; ++list_idx)
    {
        Node *node = ht->buckets[list_idx];

        while (node)
        {
            Node *prev = node;
            node = node->next;
            free (prev);
        }
    }

    free (ht->buckets);
    ht->buckets = nullptr;

    ht = nullptr;
}