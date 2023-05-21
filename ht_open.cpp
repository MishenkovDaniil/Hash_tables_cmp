#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "ht_open.h"

void ht_open_ctor (Ht_open *ht, size_t capacity)
{
    assert (ht);
    
    ht->buckets = (ht_elem_t *)calloc (capacity, sizeof (ht_elem_t));
    assert (ht->buckets);

    for (int idx = 0; idx < capacity; ++idx)
    {
        *(ht->buckets + idx) = EMPTY_VAL;
    }

    ht->size = 0;
    ht->capacity = capacity;
}

inline double ht_load_factor (Ht_open *ht)
{
    return (double)ht->size / ht->capacity;
}

size_t ht_hash (int x)
{
    size_t result = 0;
    
    memcpy (&result, &x, sizeof (int));

    return result;
}

Key_info ht_find_key_info (Ht_open *ht, const int key)
{
    assert (ht && ht->buckets);

    size_t hash_value = ht_hash (key) % ht->capacity;

    Key_info info = {EMPTY_VAL, POISON_IDX, hash_value};

    ht_elem_t *elem = ht->buckets + hash_value;

    for (size_t idx = hash_value; idx < ht->capacity; ++idx)
    {
        if (*elem == key)
        {
            info.elem = *elem;
            info.idx = idx;
            break;
        }
        if (*elem == EMPTY_VAL)
            break;
        elem++;
    }

    return info;
}

int ht_open_insert (Ht_open *ht, const int key)
{
    assert (ht && ht->buckets);

    if (ht_load_factor (ht) > MAX_LOAD_FACTOR)
    {
        ht_rehash (ht);
    }
    
    Key_info info = ht_find_key_info (ht, key);
    if (info.idx == POISON_IDX)
    {
        *(ht->buckets + info.key_hash) = key;
    }
    else 
    {
        ht_elem_t *cur_val = ht->buckets + info.key_hash;
        while (*cur_val != key && *cur_val != EMPTY_VAL && *cur_val != POISON_VAL)
        {
            cur_val++;
        }
        if (*cur_val == key)
            return EXIT_FAILURE;
        *cur_val = key;
    }
    ++ht->size;

    return EXIT_SUCCESS;
}

int ht_open_remove (Ht_open *ht, const int key)
{
    assert (ht && ht->buckets);

    Key_info info = ht_find_key_info (ht, key);

    if (info.idx != POISON_IDX && info.elem != EMPTY_VAL && info.elem != POISON_VAL)
    {
        *(ht->buckets + info.idx) = POISON_VAL;

        --ht->size;
        
        return EXIT_SUCCESS;
    }   

    return EXIT_FAILURE;
}

int ht_is_contains (Ht_open *ht, const int key)
{
    assert (ht && ht->buckets);
    
    Key_info info = ht_find_key_info (ht, key);
    
    return info.idx != POISON_IDX && info.elem != EMPTY_VAL && info.elem != POISON_VAL;
}

void ht_rehash (Ht_open *ht)
{
    assert (ht && ht->buckets);

    size_t new_capacity = ht->capacity * 2;

    ht_elem_t *new_buckets = (ht_elem_t *)calloc (new_capacity, sizeof (ht_elem_t));
    assert (new_buckets);
    
    for (int idx = 0; idx < new_capacity; ++idx)
        *(new_buckets + idx) = EMPTY_VAL;

    for (size_t idx = 0; idx < ht->capacity; ++idx)
    {
        int key = *(ht->buckets + idx);
        if (key == EMPTY_VAL)
            break;

        if (key == POISON_VAL)
            continue;

        size_t hash_value = ht_hash (key) % new_capacity;
        ht_elem_t *cur_val = new_buckets + hash_value;
        
        while (*cur_val != EMPTY_VAL)
            ++cur_val;

        *cur_val = key;
    }
    
    free (ht->buckets);

    ht->buckets = new_buckets;
    ht->capacity = new_capacity;
}

void ht_open_dtor (Ht_open *ht)
{
    if (!ht)
        return;
    if (!ht->buckets)
    {
        ht = nullptr;
        return;
    }

    free (ht->buckets);
    ht->buckets = nullptr;

    ht = nullptr;
}