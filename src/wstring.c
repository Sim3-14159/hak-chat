#include <stdlib.h>
#include <stdio.h>

#include <ncursesw/ncurses.h>
#include "wstring.h"

#define X(s)                                                                                       \
    {                                                                                              \
        if (s == NULL)                                                                             \
            return NULL;                                                                           \
    }

struct WString {
    wchar_t *data;
    size_t len;
};

struct WS_Stats {
    unsigned int height;
    unsigned int width;
};

WS_Stats *ws_fit_window(WString *wstring, WINDOW *win)
{
    WS_Stats *new = malloc(sizeof(*new));
    if (!new)
        return NULL;

    new->height = 1;
    new->width = 0;

    return new;
}

WString *ws_new(const wchar_t *text)
{
    WString *s;
    size_t len = 0;
    size_t i;

    X(text);

    while (text[len] != L'\0') {
        len++;
    }

    s = malloc(sizeof *s);
    X(s)
    s->data = malloc(sizeof(wchar_t) * (len + 1));
    if (s->data == NULL) {
        free(s);
        return NULL;
    }

    for (i = 0; i < len; i++) {
        s->data[i] = text[i];
    }
    s->data[len] = L'\0';
    s->len = len;
    return s;
}

WString *ws_copy(const WString *s)
{
    X(s);
    return ws_new(s->data);
}

WString *ws_concat(const WString *a, const WString *b)
{
    if (a == NULL || b == NULL)
        return NULL;

    wchar_t *data = malloc(sizeof *data * (a->len + b->len + 1));
    X(data);

    size_t i;
    for (i = 0; i < a->len; i++) {
        data[i] = a->data[i];
    }
    for (size_t j = 0; j < b->len; j++) {
        data[i + j] = b->data[j];
    }
    data[a->len + b->len] = L'\0';

    WString *new = ws_new(data);
    free(data);
    return new;
}

WString *ws_substring(const WString *s, size_t start, size_t len)
{
    X(s);
    if (start > s->len || len > s->len || start + len > s->len)
        return NULL;
    if (len == 0)
        return ws_new(L"");
    WString *new = ws_new(&(s->data[start])); // get from start index
    new->len = len; // set correct length
    new->data[len] = L'\0'; // and insert null
    return new;
}

WString *ws_to_upper(const WString *s)
{
    X(s);
    WString *new = ws_copy(s);
    X(new);
    for (size_t i = 0; i < new->len; i++)
        if (new->data[i] >= L'a' && new->data[i] <= L'z')
            new->data[i] = new->data[i] - 32;
    return new;
}

void ws_free(WString *s)
{
    if (!s)
        return;
    free(s->data);
    free(s);
}

size_t ws_length(const WString *s)
{
    if (s == NULL) {
        return 0;
    }
    return s->len;
}

bool ws_at(const WString *s, size_t i, wchar_t *out)
{
    if (out == NULL)
        return false;
    if (s == NULL)
        return false;
    if (i >= s->len)
        return false;
    *out = (s->data[i]);
    return true;
}

const wchar_t *ws_cstr(const WString *s)
{
    if (s == NULL)
        return L"";
    return s->data;
}

bool ws_equals(const WString *a, const WString *b)
{
    if (a == b)
        return true;
    if (a == NULL || b == NULL || a->len != b->len)
        return false;
    for (size_t i = 0; i < a->len; i++) {
        if (a->data[i] != b->data[i])
            return false;
    }
    return true;
}

int ws_compare(const WString *a, const WString *b)
{
    if (a == NULL) {
        if (b == NULL)
            return 0;
        else
            return -1;
    } else if (b == NULL) {
        return 1;
    }
    if (ws_equals(a, b))
        return 0;

    int i = 0;
    while (true) {
        if ((wchar_t) a->data[i] > (wchar_t) b->data[i])
            return 1;
        if ((wchar_t) b->data[i] > (wchar_t) a->data[i])
            return -1;
        i++;
    }
}

bool ws_starts_with(const WString *s, const WString *prefix)
{
    if (s == NULL || prefix == NULL)
        return false;
    if (ws_length(prefix) > ws_length(s))
        return false;
    for (size_t i = 0; i < prefix->len; i++) {
        if (prefix->data[i] != s->data[i])
            return false;
    }
    return true;
}

bool ws_find(const WString *haystack, const WString *needle, size_t *index)
{
    if (haystack == NULL || needle == NULL || index == NULL)
        return false;
    if (ws_length(needle) > ws_length(haystack))
        return false;
    if (ws_equals(needle, haystack) || *(needle->data) == L'\0') {
        *index = 0;
        return true;
    }

    //size_t found_index = -1;
    for (size_t i = 0; i <= haystack->len - needle->len; i++) {
        for (size_t j = 0; j < needle->len; j++) {
            if (haystack->data[j + i] != needle->data[j]) {
                break;
            }
            if (j == needle->len - 1) {
                *index = i;
                return true;
            }
        }
    }
    return false;
}

/* ---- Your additions ---- */

WString *ws_to_lower(const WString *s)
{
    X(s);
    WString *new = ws_copy(s);
    X(new);

    for (size_t i = 0; i < new->len; i++)
        if (new->data[i] >= L'A' && new->data[i] <= L'Z')
            new->data[i] = new->data[i] + 32;
    return new;
}

//WString *ws_trim(const WString *s)
//{
//    X(s);
//
//    printf("INPUT: %sEOF\n", s->data);
//    size_t si = 0; // starting index
//    size_t len;
//
//    // si = index of first non whitespace wchar_t
//    while (si < s->len && (s->data[si] == ' ' || s->data[si] == '\t' || s->data[si] == '\n' ||
//                           s->data[si] == '\r')) {
//        si++;
//    }
//    len = s->len - si;
//
//    for (size_t i = s->len; i > 0; i--) {
//        wchar_t c = s->data[i];
//        if (!c)
//            printf("broke\n");
//        break;
//        printf("%zu,", i);
//        if (c == ' ' || c == '\t' || c == '\n' || c == '\r') {
//            printf("len--\n");
//            len--;
//        }
//    }
//    printf("finished that\n");
//
//    printf("si: %zu, len: %zu\n", si, len);
//    WString *new = ws_substring(s, si, len);
//    printf("OUTPUT: %sEOF\n", new->data);
//
//    return new;
//}

WString *ws_reverse(const WString *s)
{
    WString *new = ws_copy(s);
    X(new);

    size_t j = s->len - 1;
    size_t i = 0;
    while (i < s->len) { // && j >= 0
        new->data[j] = s->data[i];
        j--;
        i++;
    }
    return new;
}

int ws_compare_ignore_case(const WString *a, const WString *b)
{
    WString *upper_a = ws_to_upper(a);
    WString *upper_b = ws_to_upper(b);
    int result = ws_compare(upper_a, upper_b);
    ws_free(upper_a);
    ws_free(upper_b);
    return result;
}

wchar_t ws_pop(WString *a)
{
    if (!a)
        return L'\0';
    if (!a->len)
        return L'\0';
    wchar_t c = a->data[--a->len];
    a->data[a->len] = L'\0';
    return c;
}

bool ws_append(WString *a, wchar_t c)
{
    if (!a)
        return false;

    return true;
}
