#include <math.h>
#include "cache.h"

// single block in cache
typedef struct _block
{
    bool valid; // valid bit for block
	bool dirty;
    unsigned b_adr; // block address
    int lru;

} block;

// the cache data structure
struct _cache
{
    int b_size; // block size
    int c_size; // cachse size
    int c_assoc; // cache associativity
    block** chart; // 2 dimentional block's array
};

//given block_ad(excluding the offset) and cache(L) returns the set
int get_set(unsigned block_ad,cache* L){

    return (block_ad)%((int)pow((double)2, L->c_size-(L->c_assoc+L->b_size)));
}
//given an address and a cache returns the address without the offset (shift)
unsigned get_block_address(unsigned address,cache *L){
    return address>>L->b_size;
}
//given a set, way and a cache updates the LRU of the relevant line
void update_LRU(int set, int way,cache* L){
    int w;
    for(w=0;w<(int)pow((double)2, L->c_assoc);++w)
        if(L->chart[set][w].lru < L->chart[set][way].lru && L->chart[set][w].valid)
            L->chart[set][w].lru++;
    L->chart[set][way].lru=0;
}
/*given a cache,set,block_ad(Excludingoffset). if the block is in the cache
 * then return the way, otherwise allocates the block
 * in the first empty way or evicts last recently used way and allocates the block there*/
bool find_block(cache* L, int set, unsigned block_ad, int *way){
    int w;
    int ways_num=(int)pow((double)2, L->c_assoc);
    for(w=0;w<ways_num;++w)
        if(L->chart[set][w].b_adr==block_ad && L->chart[set][w].valid){
            *way=w;
            return true;
        }

    for(w=0;w<ways_num;++w)
        if(!L->chart[set][w].valid){
            *way=w;
            return false;
        }

    for(w=0;w<ways_num;++w)
        if(L->chart[set][w].lru==ways_num-1){
            *way=w;
            return false;
        }
    return false;
}
// lookup for address in l1 and update cache in case it doesn't exist
bool l1_lookup(unsigned address, char* cmd)
{

    int way = -1;
    unsigned block_ad = get_block_address(address, L1);
    int set = get_set(block_ad, L1);
    if (find_block(L1, set, block_ad, &way))
    {
		if (!strcmp(cmd, "w"))
			L1->chart[set][way].dirty = true;
        update_LRU(set, way, L1);
        return true;
    }
    else
        return false;
}
/* lookup for address in l2 and update cache in case it doesn't exist,
in case evict is needed in L2 then evict also in L1 (inclusiveness)*/
bool l2_lookup(unsigned address, char* cmd)
{
    int way = -1;
	int way2 = -1;
	int way3 = -1;
    int block_ad = get_block_address(address, L2); 
    int set = get_set(block_ad, L2); 
    int set2 = get_set(block_ad, L1); 
	int set3;
    if (find_block(L2, set, block_ad, &way)) 
    {
        update_LRU(set, way, L2); 
		find_block(L1, set2, block_ad, &way2);
		if (L1->chart[set2][way2].valid && L1->chart[set2][way2].dirty)
		{
			set3 = get_set(L1->chart[set2][way2].b_adr, L2); 
			find_block(L2, set3, L1->chart[set2][way2].b_adr, &way3);
			update_LRU(set3, way3, L2);
			L1->chart[set2][way2].dirty = false;
		}
		if (!strcmp(cmd, "w"))
			L1->chart[set2][way2].dirty = true;
		L1->chart[set2][way2].valid = true;
		L1->chart[set2][way2].b_adr = block_ad;
		update_LRU(set2, way2, L1);
        return true;
    }
    else
    {
        if (L2->chart[set][way].valid == true) // evict l1
            evict_l1(L2->chart[set][way].b_adr); 

        L2->chart[set][way].valid = true;
        L2->chart[set][way].b_adr = block_ad; // update cache
        update_LRU(set, way, L2); 

		find_block(L1, set2, block_ad, &way2);
		if (L1->chart[set2][way2].valid && L1->chart[set2][way2].dirty)
		{
			set3 = get_set(L1->chart[set2][way2].b_adr, L2); 
			find_block(L2, set3, L1->chart[set2][way2].b_adr, &way3);
			update_LRU(set3, way3, L2);
			L1->chart[set2][way2].dirty = false;
		}
		if (!strcmp(cmd, "w"))
			L1->chart[set2][way2].dirty = true;
		L1->chart[set2][way2].valid = true;
		L1->chart[set2][way2].b_adr = block_ad;
		update_LRU(set2, way2, L1);
        return false;
    }
}

/* evicts if needed block from L1 to maintain inclusiveness with L2 */
void evict_l1(unsigned block_ad)
{
    int way;
    int set = get_set(block_ad, L1);
    if (find_block(L1, set, block_ad, &way))
	{
        L1->chart[set][way].valid = false;
        L1->chart[set][way].dirty = false;
	}
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
        for (int j = 0 ; j < ways ; j++)
        {
            new_cache->chart[i][j].valid = false; // valid false
            new_cache->chart[i][j].b_adr = 0; // init block address
            new_cache->chart[i][j].lru = j; // init LRU priority from left way to right 0,..#ways
			new_cache->chart[i][j].dirty = false;
        }
    }

    return new_cache;
}

/* L1, L2 memory free function */
void free_caches()
{
    // calculate sets number for L1
    int L1_sets_num = (int) pow((double)2, L1->c_size - (L1->b_size + L1->c_assoc));
    for(int i = 0 ; i < L1_sets_num ; i++)
        free(L1->chart[i]);
    free(L1->chart);
    free(L1);

    // calculate sets number for L2
    int L2_sets_num = (int) pow((double)2, L2->c_size - (L2->b_size + L2->c_assoc));
    for(int i = 0 ; i < L2_sets_num ; i++)
        free(L2->chart[i]);
    free(L2->chart);
    free(L2);
}
