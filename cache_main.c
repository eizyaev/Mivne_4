#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ARG 2

int cache_line(char* line);

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

    printf("%s\n", args[1]);


    return 0;
}
