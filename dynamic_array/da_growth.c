/* da_growth.c - checks that da_add stays cheap however big the array gets. Do not edit
   this file.

   Build and run with `make growth`. It adds N values to a new array and counts what the
   array allocated along the way. An array that grows by one element at a time allocates N
   times and, in total, about N * N / 2 values. One that doubles allocates only a handful
   of times, and a few times N values in total. Counting allocations instead of timing
   them gives the same answer on every run and every machine. `make m1` runs it with the
   argument info, which adds only N_INFO values, so it stays quick, and just reports the
   count. */

#include <stdio.h>

#include "alloc_stats.h"
#include "da_check.h"
#include "dynamic_array.h"

#define N 100000
#define N_INFO 10000
#define MAX_ALLOCS 64
#define MAX_BYTES_PER_INT 8

int main(int argc, char **argv)
{
  bool info = argc == 2 && check_same_cstr(argv[1], "info");
  int n = info ? N_INFO : N;
  if (!info) {
    check_section("da_add, 100000 times", "m2 da_add");
  }
  alloc_stats_reset();
  DynamicArray *a = da_new();
  int added = 0;
  for (int i = 0; i < n; i++) {
    added += da_add(a, i) ? 1 : 0;
  }
  bool all = added == n;
  AllocStats s;
  alloc_stats_get(&s);
  da_free(a);

  if (info) {
    if (all) {
      printf("Your da_add: %d adds made %zu allocations. (Information: milestone 2 checks "
             "this.)\n", n, s.allocs);
    } else {
      printf("Your da_add: only %d of %d adds returned true, so there is nothing to "
             "measure yet.\n", added, n);
    }
    return 0;
  }

  size_t final_bytes = (size_t)N * sizeof(int);
  printf("allocations:     %zu\n", s.allocs);
  printf("bytes allocated: %zu (%.1f times the %zu bytes the data needs)\n",
         s.bytes_allocated, (double)s.bytes_allocated / (double)final_bytes, final_bytes);
  printf("peak live bytes: %zu\n", s.peak_live_bytes);

  CHECK(all, "every da_add returned true");
  CHECK(s.allocs <= MAX_ALLOCS, "100000 adds allocate at most 64 times");
  CHECK(s.bytes_allocated <= MAX_BYTES_PER_INT * final_bytes,
        "100000 adds allocate at most 8 times the bytes the data needs, in total");
  return check_summary();
}
