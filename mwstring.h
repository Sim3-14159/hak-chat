#define _XOPEN_SOURCE_EXTENDED
#include "wstring.h"
#include <wchar.h>
#include <stdio.h>
#include <stdlib.h>

// Multiline wide String
typedef struct {
    WString *strings;
    int count;
    int capacity;
} MWString;

typedef MWString *mw;

/**
* Append a WString to a MWString
*/
bool mws_addWString(mw mwstr, ws wstr);

/**
* Add a wchar to a MWString
*/
bool mws_addWChar(mw mwstr, wchar_t chr);

/**
 * Create a new MWString
 */
MWString *mws_new(void);

/**
* Free a MWString
*/
void mws_free(ws a);

/**
* How many chars are in a MWString? (Includes newlines)
*/
size_t mws_count(const MWString *a);

/**
* Reserve space in a MWString
*/
bool mws_reserve(MWString *a, size_t n);

bool mws_add(MWString *a, wchar_t value);

bool mws_insert(MWString *a, size_t i, wchar_t value);

bool mws_get(const MWString *a, size_t i, wchar_t *out);

bool mws_set(MWString *a, size_t i, wchar_t value);

bool mws_copy_to(const MWString *a, wchar_t *dest, size_t dest_len);

bool mws_index_of(const MWString *a, wchar_t value, size_t *index);

bool mws_contains(const MWString *a, wchar_t value);

bool mws_remove_at(MWString *a, size_t i, wchar_t *out);

bool mws_remove(MWString *a, wchar_t value);

void mws_clear(MWString *a);

wchar_t mws_pop(ws a);

bool mws_reverse(ws a);

bool mws_add_all(ws a, wchar_t *values, size_t n);
