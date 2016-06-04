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
bool l2_lookup(unsigned address);
void update_LRU(int set, int way, unsigned block_ad, cache* L); // TODO
bool find_block(cache* L, int set, unsigned block_ad, int *way); // TODO
unsigned get_block_address(unsigned address, cache* L); // TODO
int get_set(unsigned address, cache* L); // TODO

#endif
