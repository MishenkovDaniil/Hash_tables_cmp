#include <nmmintrin.h>

#include "hash_functions.h"

static const size_t CRC_64_MASK = 0x42F0E1EBA9EA3693; 
static const size_t MODULO = 1000;
static const size_t POWER = 16;
static const size_t MAX = 8503057;
static const size_t MULTIPLIER = 16769023;

size_t hash_strlen (const char *string)
{
    assert (string);
    
    return strlen (string);
}

size_t hash_ch_sum (const char *string)
{
    assert (string);
    
    size_t sum = 0;

    while (*string)
    {
        sum += (size_t)*string;
        ++string;
    }

    return sum;
}

size_t hash_crc64 (const char *string)
{
    size_t crc = 0;
    const size_t BIT_NUM = 8;

    char cur_ch = *string++;

    while (1)
    {
        char temp = cur_ch;

        for (size_t i = 0; i < sizeof (char) * BIT_NUM; ++i)
        {
            size_t first_bit = crc >> (sizeof (size_t) * BIT_NUM - 1);
            size_t first_ch_bit = cur_ch >> (sizeof (char) * BIT_NUM - 1);

            crc = (crc << 1) + first_ch_bit;
            cur_ch <<= 1;

            if (first_bit)
            {
                crc = crc ^ CRC_64_MASK;
            }
        }
        if (!temp)
            break;
        cur_ch = *string++;
    }

    return crc;
}

size_t hash_poly (const char *string)
{
    assert (string);

    if (*(string + 1))
        return (hash_poly (string + 1)*POWER + *string) % MAX;
    
    return *string;
}

size_t hash_modulo (const int x)
{
    return x % MODULO;
}

size_t hash_unsigned (const int x)
{
    unsigned int res = 0;
    memcpy (&res, &x, sizeof (unsigned int));

    return res;    
}

size_t hash_multiplicative (const int x)
{
    return (x * MULTIPLIER) >> (sizeof(size_t) * 4 - sizeof (x) * 4);
}

size_t hash_double_to_int (const double x)
{
    return (size_t)x; 
}

size_t hash_interpret (const double x)
{
    return *((size_t *)&x);
}