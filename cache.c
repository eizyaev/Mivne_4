#include <math.h>
#include "cache.h"

// single block in cache
typedef struct _block
{
    bool valid; // valid bit for block
    unsigned b_adr; // block address

} block;

// the cache data structure
struct _cache
{
    int b_size; // block size
    int c_size; // cachse size
    int c_assoc; // cache associativity
    block** chart; // 2 dimentional block's array
    int** LRU; // LRU 2 dimentional array
};

bool l1_lookup(unsigned address)
{



    return true;
}

/* Init function for a cache 
 * arg1 - block size
 * arg2 - cache size
 * arg3 - assoc size aka ways 
 * return - cache* a pointer to the initialized cache */
cache* init_cache(int block_size,int size,int assoc)
{
    // allocate cahce template
    cache* new_cache = (cache*)malloc(sizeof(cache));
    if (new_cache == NULL)
    {
        fprintf(stderr, "Error not enough space\n");
        exit(4);
    }
    new_cache->b_size = block_size;
    new_cache->c_size = size;
    new_cache->c_assoc = assoc;

    int sets_num = (int) pow((double)2, size - (block_size + assoc));
    // allocate cache raws
    new_cache->chart = (block**)malloc(sizeof(block*)*sets_num);
    if (new_cache->chart == NULL)
    {
        fprintf(stderr, "Error not enough space\n");
        free(new_cache);
        exit(4);
    }
    // allocate LRU raws
    new_cache->LRU = (int**)malloc(sizeof(int*)*sets_num);
    if (new_cache->LRU == NULL)
    {
        fprintf(stderr, "Error not enough space\n");
        free(new_cache->chart);
        free(new_cache);
        exit(4);
    }
    
    int ways = (int) pow((double)2, assoc); 
    for (int i = 0 ; i < sets_num ; i++ )
    {
        // allocate cache raw for all ways
        int k = 0;
        new_cache->chart[i] = (block*) malloc(sizeof(block)*ways);
        if (new_cache->chart[i] == NULL)
        {
            fprintf(stderr, "Error not enough space\n");
            for (k = 0 ; k < i ; k++)
                free(new_cache->chart[k]);
            free(new_cache->chart);
            free(new_cache);
            exit(4);
        }
        // allocate LRU raw
        new_cache->LRU[i] = (int*) malloc(sizeof(int)*ways);
        if (new_cache->LRU[i] == NULL)
        {
            fprintf(stderr, "Error not enough space\n");
            for (k = 0 ; k < i ; k++)
            {
                free(new_cache->chart[k]);
                free(new_cache->LRU[k]);
            }
            free(new_cache->chart[k]);
            free(new_cache->chart);
            free(new_cache);
            exit(4);
        }
        for (int j = 0 ; j < ways ; j++)
        {
            new_cache->chart[i][j].valid = false; // valid false
            new_cache->chart[i][j].b_adr = 0; // init block address
            new_cache->LRU[i][j] = j; // init LRU priority from left way to right 0,..#ways
        }
    }

    return new_cache;
}

/* L1, L2 memory free function */
void free_cache()
{
    // calculate sets number for L1
    int L1_sets_num = (int) pow((double)2, L1->c_size - (L1->b_size + L1->c_assoc));
    for(int i = 0 ; i < L1_sets_num ; i++)
    {
        free(L1->chart[i]);
        free(L1->LRU[i]); 
    }
    free(L1->chart);
    free(L1->LRU);
    free(L1);

    // calculate sets number for L2
    int L2_sets_num = (int) pow((double)2, L2->c_size - (L2->b_size + L2->c_assoc));
    for(int i = 0 ; i < L2_sets_num ; i++)
    {
        free(L2->chart[i]);
        free(L2->LRU[i]); 
    }
    free(L2->chart);
    free(L2->LRU);
    free(L2);

}
