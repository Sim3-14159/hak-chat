#ifndef ALLOC_STATS_H
#define ALLOC_STATS_H

/* alloc_stats - counts every malloc, calloc, realloc and free the program makes.

   The Makefile links every program with -Wl,--wrap=malloc (and calloc, realloc, free),
   which sends those calls through alloc_stats.c instead of straight to the C library.
   That lets a test ask "how many times did the array allocate?" and get the same answer
   on every run, on any machine, which a stopwatch can't promise. It can also make an
   allocation fail on purpose, to test what your code does when memory runs out. */

#include <stddef.h>

typedef struct {
  size_t allocs;           /* blocks handed out since the last reset */
  size_t frees;            /* blocks freed since the last reset */
  size_t bytes_allocated;  /* total bytes handed out since the last reset */
  size_t live_bytes;       /* bytes handed out and not yet freed, right now */
  size_t peak_live_bytes;  /* the most live_bytes has been since the last reset */
  size_t new_live_bytes;   /* bytes handed out since the last reset and not yet freed */
} AllocStats;

/* Stores the current counts in *out. */
void alloc_stats_get(AllocStats *out);

/* Zeroes allocs, frees, bytes_allocated and new_live_bytes, and sets peak_live_bytes to
   live_bytes. live_bytes is left alone: memory allocated before a reset is still live
   after it. */
void alloc_stats_reset(void);

/* Lets the next k allocations succeed and makes the one after them return NULL, once.
   alloc_fail_after(0) fails the very next allocation. */
void alloc_fail_after(size_t k);

/* Cancels a pending alloc_fail_after. */
void alloc_fail_never(void);

#endif
