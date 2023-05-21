#ifndef HT_OPEN_H
#define HT_OPEN_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef int ht_elem_t;
static const size_t INIT_CAPACITY = 100;
static const double MAX_LOAD_FACTOR = 0.7;

static const int POISON_IDX = -1;
static const int POISON_VAL = 0xDEADBEEF;
static const int EMPTY_VAL = 0xCCCCCCCC;


typedef struct
{
    ht_elem_t *buckets = nullptr;
    size_t size = 0;
    size_t capacity = 0;
}Ht_open;

typedef struct 
{
    ht_elem_t elem = EMPTY_VAL;
    int idx = POISON_IDX;
    size_t key_hash = 0;
}Key_info;

void ht_open_ctor (Ht_open *ht, size_t capacity = INIT_CAPACITY);
int ht_open_insert (Ht_open *ht, const int key);
Key_info ht_find_key_info (Ht_open *ht, const int key);
int ht_is_contains (Ht_open *ht, const int key);
int ht_open_remove (Ht_open *ht, const int key);
void ht_open_dtor (Ht_open *ht);
void ht_rehash (Ht_open *ht);
inline double ht_load_factor (Ht_open *ht);
size_t ht_hash (int x);

#endif /* HT_OPEN_H */