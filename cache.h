#ifndef CACHE_H
#define CACHE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct _cache cache;
typedef enum { false, true } bool;
extern cache* L1;
extern cache* L2;

/* Init function for a cache 
 * arg1 - block size
 * arg2 - cache size
 * arg3 - assoc size aka ways 
 * return - cache* a pointer to the initialized cache */
cache* init_cache(int block_size,int l1_size,int l1_as);
/* L1, L2 memory free function */
void free_caches();
// lookup for address in l1 and update cache in case it doesn't exist
bool l1_lookup(unsigned address, char* cmd);
/* lookup for address in l2 and update cache in case it doesn't exist,
in case evict is needed in L2 then evict also in L1 (inclusiveness)*/
bool l2_lookup(unsigned address, char* cmd); 
//given a set, way and a cache updates the LRU of the relevant line
void update_LRU(int set, int way, cache* L);
/*given a cache,set,block_ad(Excludingoffset). if the block is in the cache
 * then return the way, otherwise allocates the block
 * in the first empty way or evicts last recently used way and allocates the block there*/
bool find_block(cache* L, int set, unsigned block_ad, int *way);
//given an address and a cache returns the address without the offset (shift)
unsigned get_block_address(unsigned address, cache* L);
//given block_ad(excluding the offset) and cache(L) returns the set
int get_set(unsigned block_ad, cache* L);
/* evicts if needed block from L1 to maintain inclusiveness with L2 */
void evict_l1(unsigned block_ad); 

#endif
