#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <stdbool.h>
#include <stddef.h>

struct WString {
    wchar_t *data;
    size_t count;
    size_t capacity;
};

/* An ordered array of wchar_ts that grows as you add to it. */
typedef struct WString WString;
typedef struct WString *ws; // ptr

/* ---- Making and freeing ---- */

/* A new, empty WString. NULL if memory runs out.
   Whoever gets one must pass it to ws_free exactly once. */
WString *ws_new(void);

/* Frees a and everything it holds. Does nothing if a is NULL. */
void ws_free(WString *a);

/* ---- Size ---- */

/* The number of wchar_ts in a. 0 if a is NULL. */
size_t ws_count(const WString *a);

/* A hint that a will soon hold at least n wchar_ts, so it can make room now. Returns true
   if a can now hold n wchar_ts without allocating again. Returns false, and leaves a
   unchanged, if a is NULL or memory runs out. */
bool ws_reserve(WString *a, size_t n);

/* ---- Adding ---- */

/* Adds value to the end of a and returns true. Returns false, and leaves a unchanged,
   if a is NULL or memory runs out. */
bool ws_add(WString *a, wchar_t value);

/* Inserts value at index i, shifting the wchar_ts at i and after up one index, and returns
   true. i may equal ws_count(a), which adds to the end. Returns false, and leaves a
   unchanged, if a is NULL, i > ws_count(a), or memory runs out. */
bool ws_insert(WString *a, size_t i, wchar_t value);

/* ---- Reading and writing ---- */

/* Stores the wchar_t at index i in *out and returns true. Returns false, and leaves *out
   alone, if a or out is NULL or i is not less than ws_count(a). */
bool ws_get(const WString *a, size_t i, wchar_t *out);

/* Replaces the wchar_t at index i with value and returns true. Returns false, and leaves a
   unchanged, if a is NULL or i is not less than ws_count(a). */
bool ws_set(WString *a, size_t i, wchar_t value);

/* Copies a's wchar_ts, in order, into dest[0 .. ws_count(a) - 1] and returns true.
   Returns false, and writes nothing, if a or dest is NULL or dest_len < ws_count(a). */
bool ws_copy_to(const WString *a, wchar_t *dest, size_t dest_len);

/* ---- Searching ---- */

/* Finds the first index holding value: stores it in *index and returns true. Returns
   false, and leaves *index alone, if value does not occur or a or index is NULL. */
bool ws_index_of(const WString *a, wchar_t value, size_t *index);

/* true if value occurs in a. false if a is NULL. */
bool ws_contains(const WString *a, wchar_t value);

/* ---- Removing ----

   Removing never needs new memory, so these cannot fail for lack of it. */

/* Removes the wchar_t at index i, shifting later wchar_ts down one index, and returns true.
   If out is not NULL, stores the removed wchar_t in *out first. Returns false, and leaves
   a and *out unchanged, if a is NULL or i is not less than ws_count(a). */
bool ws_remove_at(WString *a, size_t i, wchar_t *out);

/* Removes the first occurrence of value from a, shifting later wchar_ts down one index,
   and returns true. Returns false if value does not occur or a is NULL. */
bool ws_remove(WString *a, wchar_t value);

/* Removes every wchar_t from a, so its count is 0. Does nothing if a is NULL. */
void ws_clear(WString *a);

/* ---- Your additions ----

   Declare your extension functions below this line, each with a comment in the style of
   the ones above: what it returns, what it does with NULL, and who owns the result.
   Do not change anything above this line. */

/* Removes the last wchar_t from a and returns it. Returns 0, and leaves a unchanged, if a is
 NULL or empty. */
wchar_t ws_pop(ws a);

/* Reverses the order of the wchar_ts in a and returns true. Returns false, and leaves a
 unchanged, if a is NULL or memory runs out. */
bool ws_reverse(ws a);

/* Adds values[0 .. n - 1] to the end of a, in order, and returns true. Returns false,
 and leaves a unchanged, if a or values is NULL or memory runs out. The caller still
 owns values. */
bool ws_add_all(ws a, wchar_t *values, size_t n);

#endif
