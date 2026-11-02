#ifndef WSTRING_H
#define WSTRING_H

#include <stdbool.h>
#include <stddef.h>
#include <ncursesw/ncurses.h>

typedef struct WString WString;
typedef struct WS_Stats WS_Stats;

/* Get a word wrapped WW_WString that fits the window */
WS_Stats *ws_fit_window(WString *wstring, WINDOW *win);

/* A new WString holding a copy of text. NULL if text is NULL or memory runs out. */
WString *ws_new(const wchar_t *text);

/* A new WString with the same bytes as s, in memory of its own.
   NULL if s is NULL or memory runs out. */
WString *ws_copy(const WString *s);

/* A new WString: a's bytes followed by b's. NULL if either is NULL or memory runs out. */
WString *ws_concat(const WString *a, const WString *b);

/* A new WString holding len bytes of s, starting at index start.
   NULL if s is NULL, if the range does not fit inside s, or if memory runs out.
   An empty range that fits, such as start == ws_length(s) with len 0, gives "". */
WString *ws_substring(const WString *s, size_t start, size_t len);

/* A new WString with every 'a'..'z' changed to 'A'..'Z' and every other byte kept.
   s itself is unchanged. NULL if s is NULL or memory runs out. */
WString *ws_to_upper(const WString *s);

/* Frees s and its bytes. Does nothing if s is NULL. */
void ws_free(WString *s);

/* ---- Reading ---- */

/* The number of bytes in s, not counting the terminator. 0 if s is NULL. */
size_t ws_length(const WString *s);

/* Stores s's byte at index i in *out and returns true. Returns false, and leaves *out
   alone, if s or out is NULL or i is not less than ws_length(s). */
bool ws_at(const WString *s, size_t i, wchar_t *out);

/* s's bytes as a C string, for printf("%s"). Do not write through it or free it; it stays
   valid until s is freed. "" if s is NULL. */
const wchar_t *ws_cstr(const WString *s);

/* ---- Comparing and searching ---- */

/* true if a and b hold the same bytes. Two NULLs are equal; NULL and a WString are not. */
bool ws_equals(const WString *a, const WString *b);

/* Negative, zero or positive as a sorts before, equal to, or after b, comparing bytes as
   wchar_t, like strcmp. A WString sorts after every WString it begins with ("Dun"
   before "Dune"). NULL sorts before any WString, and two NULLs compare equal. */
int ws_compare(const WString *a, const WString *b);

/* true if s begins with prefix. Every WString begins with "". false if either is NULL. */
bool ws_starts_with(const WString *s, const WString *prefix);

/* Finds the first place needle occurs in haystack: stores its index in *index and returns

   is NULL. An empty needle is found at index 0. */
bool ws_find(const WString *haystack, const WString *needle, size_t *index);

/* ---- Your additions ----

   Declare your extension functions below this line, each with a comment in the style of
   the ones above: what it returns, what it does with NULL, and who owns the result.
   Do not change anything above this line. */

/* A new WString with every 'A'..'Z' changed to 'a'..'z' and every other byte kept.
   s itself is unchanged. NULL if s is NULL or memory runs out. */
WString *ws_to_lower(const WString *s);

/* a new WString without the whitespace at either end; NULL on failure of malloc or if s == NULL */
//WString *ws_trim(const WString *s);

/*  Negative, zero or positive as a sorts before, equal to, or after b, comparing bytes as
   wchar_t, like strcmp. A WString sorts after every WString it begins with ("Dun"
   before "Dune"). NULL sorts before any WString, and two NULLs compare equal. Case
   is entirely ignored, so "a" also sorts equal to "A". /
   */
int ws_compare_ignore_case(const WString *a, const WString *b);

/* reverses a string `s`, returns NULL if s == NULL */
WString *ws_reverse(const WString *s);

/* pop from end of a. return NULL if a is NULL or a->len is 0 (because you can't pop from a 0
                                                               length string)
*/
wchar_t ws_pop(WString *a);

#endif
