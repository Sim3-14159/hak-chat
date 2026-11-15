#define _XOPEN_SOURCE_EXTENDED
#include "wstring.h"

#include <locale.h>
#include <ncursesw/ncurses.h>
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

#define INITIAL_SIZE 8
#define X(p)                                                                                       \
    if (!p)                                                                                        \
        return NULL;
#define MIN(x, y) (x > y ? y : x)

// TODO:
bool mws_addWString(mw mwstr, ws wstr)
{
    if (!mwstr || !wstr)
        return false;

    return true;
}

// TODO:
bool mws_addWChar(mw mwstr, wchar_t chr)
{
    if (!mwstr)
        return false;

    return true;
}

// TODO:
WString *mws_new(void)
{
    ws new = malloc(sizeof(MWString));
    X(new);

    new->count = 0;
    new->data = malloc((sizeof(wchar_t) * INITIAL_SIZE));
    new->capacity = INITIAL_SIZE;
    if (!new->data) {
        free(new->data);
        free(new);
        return NULL;
    }
    return new;
}

// TODO:
void mws_free(ws a)
{
    if (!a)
        return;
    free(a->data);
    free(a);
}

// TODO:
size_t mws_count(const WString *a)
{
    if (!a)
        return 0;
    return a->count;
}

// TODO:
bool mws_reserve(WString *a, size_t n)
{
    if (!a)
        return false;
    if (n <= a->capacity)
        return true;
    wchar_t *newmem = malloc(sizeof(wchar_t) * n);
    if (!newmem)
        return false;

    for (size_t i = 0; i < a->count; i++)
        newmem[i] = a->data[i];
    free(a->data);
    a->data = newmem;
    a->capacity = n;
    return true;
}

// TODO:
bool mws_add(WString *a, wchar_t value)
{
    if (!a)
        return false;

    if (a->count >= a->capacity)
        if (!mws_reserve(a, a->capacity * 2))
            return false;

    a->data[a->count] = value;
    a->count++;
    return true;
}

// TODO:
bool mws_insert(WString *a, size_t i, wchar_t value)
{
    if (!a || i > a->count)
        return false;

    if (a->count >= a->capacity)
        if (!mws_reserve(a, a->capacity * 2)) // double size
            return false;

    for (size_t j = a->count; j > i; j--) {
        a->data[j] = a->data[j - 1];
    }
    a->data[i] = value;
    a->count++;
    return true;
}

// TODO:
bool mws_get(const WString *a, size_t i, wchar_t *out)
{
    if (!a || !out || i >= a->count)
        return false;
    *out = a->data[i];
    return true;
}

// TODO:
bool mws_set(WString *a, size_t i, wchar_t value)
{
    if (!a)
        return false;
    if (i >= a->count) // || i < 0
        return false;
    a->data[i] = value;
    return true;
}

// TODO:
bool mws_copy_to(const WString *a, wchar_t *dest, size_t dest_len)
{
    if (!a || !dest || a->count > dest_len)
        return false;

    for (size_t i = 0; i < MIN(dest_len, a->count); i++)
        dest[i] = a->data[i];
    return true;
}

// TODO:
bool mws_index_of(const WString *a, wchar_t value, size_t *index)
{
    if (!a || !index)
        return false;

    for (size_t i = 0; i < a->count; i++) {
        if (a->data[i] == value) {
            *index = i;
            return true;
        }
    }
    return false;
}

// TODO:
bool mws_contains(const WString *a, wchar_t value)
{
    if (!a)
        return false;
    for (size_t i = 0; i < a->count; i++)
        if (a->data[i] == value)
            return true;
    return false;
}

// TODO:
bool mws_remove_at(WString *a, size_t i, wchar_t *out)
{
    if (!a || i >= a->count)
        return false;
    if (out)
        *out = a->data[i];

    for (size_t j = i; j < a->count - 1; j++) {
        a->data[j] = a->data[j + 1];
    }
    a->count--;
    return true;
}

// TODO:
bool mws_remove(WString *a, wchar_t value)
{
    if (!a)
        return false;
    for (size_t i = 0; i < a->count; i++) {
        if (a->data[i] == value) {
            for (size_t j = i; j < a->count - 1; j++)
                a->data[j] = a->data[j + 1];
            a->count--;
            return true;
        }
    }
    return false;
}

// TODO:
void mws_clear(WString *a)
{
    if (!a)
        return;
    a->count = 0;
}

// TODO:
wchar_t mws_pop(ws a)
{
    if (!a)
        return 0;
    wchar_t value = 0;
    mws_remove_at(a, a->count - 1, &value);

    return value;
}

// TODO:
bool mws_reverse(ws a)
{
    if (!a || !a->data)
        return false;
    for (size_t i = 0; i < a->count / 2; i++) {
        wchar_t first = a->data[i];
        wchar_t second = a->data[a->count - i - 1];
        a->data[i] = second;
        a->data[a->count - i - 1] = first;
    }
    return true;
}

// TODO:
bool mws_add_all(ws a, wchar_t *values, size_t n)
{
    if (!a || !values || !mws_reserve(a, a->count + n))
        return false;
    for (size_t i = 0; i < n; i++)
        mws_add(a, values[i]);
    return true;
}
