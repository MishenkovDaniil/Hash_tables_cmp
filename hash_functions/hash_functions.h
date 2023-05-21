#ifndef HASH_FUNCTIONS_H
#define HASH_FUNCTIONS_H

#include <stdio.h>
#include <assert.h>
#include <string.h>

size_t hash_strlen (const char *string);
size_t hash_ch_sum (const char *string);
size_t hash_crc64 (const char *string);
size_t hash_poly (const char *string);
size_t hash_modulo (const int x);
size_t hash_unsigned (const int x);
size_t hash_multiplicative (const int x);
size_t hash_double_to_int (const double x);
size_t hash_interpret (const double x);

#endif /* HASH_FUNCTIONS_H */