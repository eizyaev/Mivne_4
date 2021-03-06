#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cache.h"

#define MAX_ARG 2

/* The function gets trace input line
 * And performs Cache lookup */
int cache_line(char* line);
/* hit/miss counters*/
int L1_miss, L1_hit, L2_hit, L2_miss;
cache* L1; // L1 cache
cache* L2; // L2 cache

int main(int argc, char *argv[])
{
	// minimum arguments check
	if (argc < 18)
	{
		fprintf(stderr, "illegal arguments\n");
		exit(1);
	}

	// open trace file
	FILE *trace = fopen(argv[1], "r");
	if (trace == 0) {
		fprintf(stderr, "cannot open trace file\n");
		exit(2);
	}
	int count = 0; // commands number counter

	// variables for arguments input
	int block_size = -1;
	int l1_size = -1;
	int l1_as = -1;
	int l2_size = -1;
	int l2_as = -1;
	int L1_time = -1;
	int L2_time = -1;
	int mem_time = -1;

	// arguments parsing
	for (int i = 2 ; i < 18 ; i += 2)
	{
		if(!strcmp(argv[i], "--mem-cyc"))
			mem_time = atoi(argv[i+1]);

		else if(!strcmp(argv[i], "--bsize"))
			block_size = atoi(argv[i+1]);

		else if(!strcmp(argv[i], "--l1-size"))
			l1_size = atoi(argv[i+1]);

		else if(!strcmp(argv[i], "--l1-assoc"))
			l1_as = atoi(argv[i+1]);

		else if(!strcmp(argv[i], "--l1-cyc"))
			L1_time = atoi(argv[i+1]);

		else if(!strcmp(argv[i], "--l2-size"))
			l2_size = atoi(argv[i+1]);

		else if(!strcmp(argv[i], "--l2-assoc"))
			l2_as = atoi(argv[i+1]);

		else if(!strcmp(argv[i], "--l2-cyc"))
			L2_time = atoi(argv[i+1]);
	}

	// input check
	if (block_size == -1 || l1_size == -1 || l1_as == -1 ||
			l2_size == -1 || l2_as == -1 || L1_time == -1 ||
			L2_time == -1 || mem_time == -1)
	{
		fprintf(stderr, "illegal arguments\n");
		exit(1);
	}
	if (l1_size > l2_size)
	{
		fprintf(stderr, "l1 cant be bigger than l2\n");
		exit(2);
	}

	// Initialize Caches L1,L2
	L1 = init_cache(block_size, l1_size, l1_as);
	L2 = init_cache(block_size, l2_size, l2_as);

	// variables for using getline()
	char * line = NULL;
	size_t len = 0;

	// read line from .txt file and perform cache lookup
	while ((getline(&line, &len, trace)) != -1)
	{
		cache_line(line);
		count++;
	}


	// calculating & printing caches stats
	double L1_rate = ((double)(L1_miss))/(L1_miss + L1_hit);
	double L2_rate = ((double)(L2_miss))/(L2_miss + L2_hit);
	double time = L1_hit * L1_time + L2_hit * L2_time + L2_miss * mem_time;
	printf("L1miss=%.3f L2miss=%.3f AccTimeAvg=%.3f\n", L1_rate, L2_rate, time/count);

	free_caches();
	if (line)
		free(line);

	// closing opened file
	fclose(trace);
	return 0;
}

/* The function gets trace input line
 * And performs Cache lookup */
int cache_line(char* line)
{

	char const* delimiters = " \t\n";
	char* args[MAX_ARG];

	// parsing the line into the command and its arguments
	args[0] = strtok(line, delimiters);
	if (args[0] == NULL)
	{
		fprintf(stderr, "Unknown trace command\n");
		exit(3);
	}
	args[1] = strtok(NULL, delimiters);
	unsigned address = strtol(args[1], NULL, 16);

	/* L1 update */
	if (l1_lookup(address, args[0]))
		// L1 Hit
		L1_hit++;
	/* L2 update, L1 evict (if needed) */
	else if (l2_lookup(address, args[0]))
		// L1 Miss, L2 Hit
	{
		L1_miss++;
		L2_hit++;
	}
	else
		// L1, L2 Missed
	{
		L1_miss++;
		L2_miss++;
	}


	return 0;
}
