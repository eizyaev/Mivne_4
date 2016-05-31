#ifndef CACHE_H
#define CACHE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct _cache cache;
typedef enum { false, true } bool;
cache* init_cache(int block_size,int l1_size,int l1_as);
extern cache* L1;
extern cache* L2;
void free_caches();
bool l1_lookup(unsigned address);
bool l2_lookup(unsigned address);

#endif
