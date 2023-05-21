#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "hash_table.h"
#include "ht_open.h"
#include "hash_table_1.h"
#include "hash_functions/hash_functions.h"

static const size_t VALUE_NUM = 1000000; 

static const char *CSV_FILENAME   = "data/histograms/hash.csv";

static const size_t HASH_MMAP_CAPACITY = 1009;
static const size_t INIT_LIST_CAPACITY = 5000;

// #define STRING_TEST 
#define DOUBLE_TEST 
// #define DOUBLE_TEST 

void hash_functions_test ();

#ifdef STRING_TEST
static size_t (*hash_func[])(const char *) = {hash_strlen, hash_ch_sum, 
                                              hash_poly, hash_crc64,  nullptr};
size_t load_hash_maps_string (Hash_table *hash_maps, size_t (**hash_func)(const char *));
void load_strings (Hash_table *hash_maps, const size_t value_num, const size_t hash_functions_num);
#endif 

#ifdef INT_TEST
static size_t (*hash_func[])(const int) = {hash_modulo, hash_unsigned, 
                                           hash_multiplicative,  nullptr};
size_t load_hash_maps_ints (Hash_table *hash_maps, size_t (**hash_func)(const int));
void load_ints (Hash_table *hash_maps, const size_t value_num, const size_t hash_functions_num);
#endif 

#ifdef DOUBLE_TEST
static size_t (*hash_func[])(const double) = {hash_double_to_int, hash_interpret, nullptr};
size_t load_hash_maps_doubles (Hash_table *hash_maps, size_t (**hash_func)(const double));
void load_doubles (Hash_table *hash_maps, const size_t value_num, const size_t hash_functions_num);
#endif 

static const int HASH_FUNCTIONS_NUM = sizeof (hash_func) / sizeof (*hash_func);
void print_elem_idxs (FILE *file, size_t elem_num);
int treat_ht_requests ();

int main ()
{
    // hash_functions_test ();
    treat_ht_requests ();

    return 0;
}

int treat_ht_requests ()
{
    Ht_open ht_open = {};
    Hash_table_1 ht = {};
    
    
    FILE *time_results_file = fopen ("time_results.csv", "w");
    FILE *time_results_txt_file = fopen ("time_results.txt", "w");
    assert (time_results_file && time_results_txt_file);

    double time_1[100] = {0};
    double time_2[100] = {0};

    int key_arr[250000] = {};
    for (int i = 0; i < 250000; ++i)
    {
        key_arr[i] = rand ();
    }
   
    size_t time_idx = 0;

    for (size_t request_num = 10000; request_num < 1000000; request_num += 10000)
    {
        ht_ctor (&ht, request_num);
        ht_open_ctor (&ht_open, request_num);
        
        for (size_t iter = 0; iter < request_num; ++iter)
        {
            char cmd = rand () % 3;
            
            int key = key_arr[rand () % (request_num / 4)];

            switch (cmd)
            {
                case 0:
                {
                    clock_t t1 = clock ();        
                    volatile int res1 = ht_open_insert (&ht_open, key);
                    clock_t t2 = clock ();
                    
                    time_1[time_idx] += (double)(1000 * (t2 - t1)) / CLOCKS_PER_SEC;
                    
                    clock_t t3 = clock ();        
                    volatile int res2 = ht_insert (&ht, key);
                    clock_t t4 = clock ();

                    time_2[time_idx] += (double)(1000 *(t4 - t3)) / CLOCKS_PER_SEC;
                    break;
                }
                case 1:
                {
                    clock_t t1 = clock ();        
                    volatile int res1 = ht_open_remove (&ht_open, key);
                    clock_t t2 = clock ();

                    time_1[time_idx] += (double)(1000 * (t2 - t1)) / CLOCKS_PER_SEC;
                    clock_t t3 = clock ();        
                    volatile int res2 = ht_remove (&ht, key);
                    clock_t t4 = clock ();

                    time_2[time_idx] += (double)(1000 *(t4 - t3)) / CLOCKS_PER_SEC;
                    break;
                }
                case 2:
                {
                    clock_t t1 = clock ();        
                    volatile int res1 = ht_is_contains (&ht_open, key);
                    clock_t t2 = clock ();

                    time_1[time_idx] += (double)(1000 * (t2 - t1)) / CLOCKS_PER_SEC;
                    clock_t t3 = clock ();        
                    volatile int res2 = is_contains (&ht, key);
                    clock_t t4 = clock ();

                    time_2[time_idx] += (double)(1000 *(t4 - t3)) / CLOCKS_PER_SEC;
                    break;
                }
                // case 3:
                // {
                //     clock_t t1 = clock ();        
                //     volatile int res1 = ht_open_insert (&ht_open, key);
                //     clock_t t2 = clock ();
                    
                //     time_1[time_idx] += (double)(1000 * (t2 - t1)) / CLOCKS_PER_SEC;
                    
                //     clock_t t3 = clock ();        
                //     volatile int res2 = ht_insert (&ht, key);
                //     clock_t t4 = clock ();

                //     time_2[time_idx] += (double)(1000 *(t4 - t3)) / CLOCKS_PER_SEC;
                //     break;
                // }
                default:
                {
                    fprintf (stderr, "Error: unknown command.\n");
                    return EXIT_FAILURE;
                }
            }
        }
        fprintf (stderr, "+");
        time_idx++;
        ht_open_dtor (&ht_open);
        ht_dtor (&ht);
    }

    for (int i = 0; i < 100; ++i)
    {
        fprintf (time_results_file, "%.0lf, ", time_1[i]);
        fprintf (time_results_txt_file, "%lf, ", time_1[i]);
        if (!(time_1[i + 1]))
            break;
    }
    fprintf (time_results_file, "\n");
    fprintf (time_results_txt_file, "\n");
    for (int i = 0; i < 100; ++i)
    {
        fprintf (time_results_file, "%.0lf, ", time_2[i]);
        fprintf (time_results_txt_file, "%lf, ", time_2[i]);
        if (!(time_2[i + 1]))
            break;
    }
    fprintf (time_results_file, "\n");
    fprintf (time_results_txt_file, "\n");
    for (int i = 0; i < 100; ++i)
    {
        fprintf (time_results_file, "%d, ", 10000 + 10000 * i);
    }
    fprintf (time_results_file, "\n");

    fclose (time_results_file);
    fclose (time_results_txt_file);


    return EXIT_SUCCESS;
}

void hash_functions_test ()
{
    assert (hash_func);
    
    Hash_table hash_maps [HASH_FUNCTIONS_NUM] = {};

    FILE *csv_file = fopen ("strings.csv", "w");
    FILE *dump_file = fopen ("hash_table_dump.txt", "w");
    assert (csv_file &&dump_file);

    #ifdef STRING_TEST
    size_t hash_functions_num = load_hash_maps_string (hash_maps, hash_func);
    #endif 

    #ifdef INT_TEST 
    size_t hash_functions_num = load_hash_maps_ints (hash_maps, hash_func);
    #endif 

    #ifdef DOUBLE_TEST 
    size_t hash_functions_num = load_hash_maps_doubles (hash_maps, hash_func);
    #endif

    for (int num = 0; num < hash_functions_num; ++num)
    {
        hash_table_dump (hash_maps + num, dump_file, csv_file);
    }

    print_elem_idxs (csv_file, VALUE_NUM);
    fprintf (csv_file, "\n");

    fclose (dump_file);
    fclose (csv_file);
}

#ifdef DOUBLE_TEST 

size_t load_hash_maps_doubles (Hash_table *hash_maps, size_t (**hash_func)(const double))
{
    assert (hash_maps && hash_func);
    
    size_t hash_func_num = 0;

    while (*hash_func)
    {
        hash_table_ctor (hash_maps + hash_func_num, HASH_MMAP_CAPACITY, *hash_func, INIT_LIST_CAPACITY);
        
        hash_func++;
        hash_func_num++;
    }

    load_doubles (hash_maps, VALUE_NUM, hash_func_num);
    
    return hash_func_num;
}

void load_doubles (Hash_table *hash_maps, const size_t value_num, const size_t hash_functions_num)
{
    assert (hash_maps && value_num);
    srand (time (NULL));

    for (size_t i = 0; i < value_num; ++i)
    {
        double key = (double)rand () / (double)rand ();
        
        if (hash_table_exists (hash_maps, key))
        {
            --i;
            continue;
        }
        for (int num = 0; num < hash_functions_num; ++num)
        {
            hash_table_insert (hash_maps + num, key);
        }
    }
}
#endif 

#ifdef INT_TEST 

size_t load_hash_maps_ints (Hash_table *hash_maps, size_t (**hash_func)(const int))
{
    assert (hash_maps && hash_func);
    
    size_t hash_func_num = 0;

    while (*hash_func)
    {
        hash_table_ctor (hash_maps + hash_func_num, HASH_MMAP_CAPACITY, *hash_func, INIT_LIST_CAPACITY);
        
        hash_func++;
        hash_func_num++;
    }

    load_ints (hash_maps, VALUE_NUM, hash_func_num);
    
    return hash_func_num;
}

void load_ints (Hash_table *hash_maps, const size_t value_num, const size_t hash_functions_num)
{
    assert (hash_maps && value_num);
    srand (time (NULL));

    for (size_t i = 0; i < value_num; ++i)
    {
        int key = rand ();
        
        if (hash_table_exists (hash_maps, key))
        {
            --i;
            continue;
        }
        for (int num = 0; num < hash_functions_num; ++num)
        {
            hash_table_insert (hash_maps + num, key);
        }
    }
}
#endif 

#ifdef STRING_TEST 

size_t load_hash_maps_string (Hash_table *hash_maps, size_t (**hash_func)(const char *))
{
    assert (hash_maps && hash_func);

    size_t hash_func_num = 0;

    while (*hash_func)
    {
        hash_table_ctor (hash_maps + hash_func_num, HASH_MMAP_CAPACITY, *hash_func, INIT_LIST_CAPACITY);
        hash_func++;
        hash_func_num++;
    }

    load_strings (hash_maps, VALUE_NUM, hash_func_num);

    return hash_func_num;
}

void load_strings (Hash_table *hash_maps, const size_t value_num, const size_t hash_functions_num)
{
    assert (hash_maps && value_num);
    srand (time (NULL));

    for (size_t i = 0; i < value_num; ++i)
    {

        size_t max_len = ((unsigned int)rand()) % HASH_MMAP_CAPACITY;
        char *string = (char *)calloc (max_len, sizeof (char));
        
        for (size_t j = 0; j < max_len; ++j)
        {
            string[j] = (unsigned char)(rand () % 255);
        }
        if (hash_table_exists (hash_maps, string))
        {
            --i;
            continue;
        }
        for (int num = 0; num < hash_functions_num; ++num)
        {
            hash_table_insert (hash_maps + num, string);
        }
    }
}

#endif 

void print_elem_idxs (FILE *file, size_t elem_num)
{
    assert (file);

    for (int idx = 0; idx < HASH_MMAP_CAPACITY; ++idx)
    {
        fprintf (file, "%d,", idx);
    }
}
