#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <stdbool.h>
#include <stddef.h>

struct DynamicArray {
    int *data;
    size_t count;
    size_t capacity;
};

/* An ordered array of ints that grows as you add to it. */
typedef struct DynamicArray DynamicArray;
typedef struct DynamicArray *da; // ptr

/* ---- Making and freeing ---- */

/* A new, empty DynamicArray. NULL if memory runs out.
   Whoever gets one must pass it to da_free exactly once. */
DynamicArray *da_new(void);

/* Frees a and everything it holds. Does nothing if a is NULL. */
void da_free(DynamicArray *a);

/* ---- Size ---- */

/* The number of ints in a. 0 if a is NULL. */
size_t da_count(const DynamicArray *a);

/* A hint that a will soon hold at least n ints, so it can make room now. Returns true
   if a can now hold n ints without allocating again. Returns false, and leaves a
   unchanged, if a is NULL or memory runs out. */
bool da_reserve(DynamicArray *a, size_t n);

/* ---- Adding ---- */

/* Adds value to the end of a and returns true. Returns false, and leaves a unchanged,
   if a is NULL or memory runs out. */
bool da_add(DynamicArray *a, int value);

/* Inserts value at index i, shifting the ints at i and after up one index, and returns
   true. i may equal da_count(a), which adds to the end. Returns false, and leaves a
   unchanged, if a is NULL, i > da_count(a), or memory runs out. */
bool da_insert(DynamicArray *a, size_t i, int value);

/* ---- Reading and writing ---- */

/* Stores the int at index i in *out and returns true. Returns false, and leaves *out
   alone, if a or out is NULL or i is not less than da_count(a). */
bool da_get(const DynamicArray *a, size_t i, int *out);

/* Replaces the int at index i with value and returns true. Returns false, and leaves a
   unchanged, if a is NULL or i is not less than da_count(a). */
bool da_set(DynamicArray *a, size_t i, int value);

/* Copies a's ints, in order, into dest[0 .. da_count(a) - 1] and returns true.
   Returns false, and writes nothing, if a or dest is NULL or dest_len < da_count(a). */
bool da_copy_to(const DynamicArray *a, int *dest, size_t dest_len);

/* ---- Searching ---- */

/* Finds the first index holding value: stores it in *index and returns true. Returns
   false, and leaves *index alone, if value does not occur or a or index is NULL. */
bool da_index_of(const DynamicArray *a, int value, size_t *index);

/* true if value occurs in a. false if a is NULL. */
bool da_contains(const DynamicArray *a, int value);

/* ---- Removing ----

   Removing never needs new memory, so these cannot fail for lack of it. */

/* Removes the int at index i, shifting later ints down one index, and returns true.
   If out is not NULL, stores the removed int in *out first. Returns false, and leaves
   a and *out unchanged, if a is NULL or i is not less than da_count(a). */
bool da_remove_at(DynamicArray *a, size_t i, int *out);

/* Removes the first occurrence of value from a, shifting later ints down one index,
   and returns true. Returns false if value does not occur or a is NULL. */
bool da_remove(DynamicArray *a, int value);

/* Removes every int from a, so its count is 0. Does nothing if a is NULL. */
void da_clear(DynamicArray *a);

/* ---- Your additions ----

   Declare your extension functions below this line, each with a comment in the style of
   the ones above: what it returns, what it does with NULL, and who owns the result.
   Do not change anything above this line. */

/* Removes the last int from a and returns it. Returns 0, and leaves a unchanged, if a is
 NULL or empty. */
int da_pop(da a);

/* Reverses the order of the ints in a and returns true. Returns false, and leaves a
 unchanged, if a is NULL or memory runs out. */
bool da_reverse(da a);

/* Adds values[0 .. n - 1] to the end of a, in order, and returns true. Returns false,
 and leaves a unchanged, if a or values is NULL or memory runs out. The caller still
 owns values. */
bool da_add_all(da a, int *values, size_t n);

#endif
