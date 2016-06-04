# Makefile for the Bank program
CC = gcc
CFLAGS = -std=c99 -g -Wall
CCLINK = $(CC)
OBJS = cache_main.o cache.o 
RM = rm -f
# Creating the  executable
cacheSim: $(OBJS)
	$(CCLINK) $(CFLAGS) -o cacheSim $(OBJS) -lm
# Creating the object files
cache_main.o: cache_main.c cache.h
	$(CC) $(CFLAGS) -c cache_main.c
cache.o: cache.c cache.h
	$(CC) $(CFLAGS) -c cache.c
# Cleaning old files before new make
clean:
	$(RM) $(TARGET) *.o *~ "#"* cacheSim.*

