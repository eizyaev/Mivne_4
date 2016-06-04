#ifndef CACHE_H
#define CACHE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct _cache cache;
typedef enum { false, true } bool;
extern cache* L1;
extern cache* L2;

cache* init_cache(int block_size,int l1_size,int l1_as);
void free_caches();
bool l1_lookup(unsigned address);
bool l2_lookup(unsigned address); //TODO
void update_LRU(int set, int way, cache* L);
bool find_block(cache* L, int set, unsigned block_ad, int *way);
unsigned get_block_address(unsigned address, cache* L);
int get_set(unsigned block_ad, cache* L);
void evict_l1(unsigned block_ad); 

#endif
