#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cache.h"

#define MAX_ARG 2

int cache_line(char* line);
int L1_miss, L1_hit, L2_hit, L2_miss;
cache* L1;
cache* L2;

int main(int argc, char *argv[])
{
    // minimum arguments check
    if (argc < 18)
    {
        fprintf(stderr, "illegal arguments\n");
        exit(1);
    }
    
	FILE *trace = fopen(argv[1], "r");
	if (trace == 0) {
		fprintf(stderr, "cannot open trace file\n");
		exit(2);
	}
    // Initialize Caches L1,L2
    int block_size = atoi(argv[5]);
    int l1_size = atoi(argv[7]);
    int l1_as = atoi(argv[9]);
    int l2_size = atoi(argv[13]);
    int l2_as = atoi(argv[15]);
    L1 = init_cache(block_size, l1_size, l1_as);
    L2 = init_cache(block_size, l2_size, l2_as);

    // variables for using getline()
    char * line = NULL;
    size_t len = 0;

    // read line from .txt file
    while ((getline(&line, &len, trace)) != -1)
    {
        cache_line(line);
    }

    // closing opened file
    fclose(trace);

    return 0;
}

int cache_line(char* line)
{

    char const* delimiters = " \t\n";  
    char* cmd;
    char* args[MAX_ARG];

    // parsing the line into the command and its arguments
    args[0] = strtok(line, delimiters);
    if (args[0] == NULL)
    {
		fprintf(stderr, "Unknown trace command\n");
		exit(3);
    }
    args[1] = strtok(NULL, delimiters); 
    unsigned address = atoi(args[1]);
    
    if (cache_lookup(address, L1))
    // L1 Hit
    {
        printf("L1 Hit for %#010x\n", address);
        L1_hit++;
    }
    else if (cache_lookup(address, L2))
    // L1 Miss, L2 Hit
    {
        printf("L1 Miss, L2 Hit for %#010x\n", address);
        L1_miss++;
        L2_hit++;
    }
    else
    // L1, L2 Missed
    {
        printf("L1 Miss, L2 Miss for %#010x\n", address);
        L2_miss++;
    }
    

    return 0;
}
