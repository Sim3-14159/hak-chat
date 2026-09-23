#ifndef WTEXT_H
#define WTEXT_H

#include <stdlib.h>
#include <wchar.h>

#define MAX_LEN 1024

struct WText {
    size_t len; // same as wcslen(arr), only used because wcslen is inneficient and this is faster
    wchar_t *arr;
    struct WText *next;
};

#endif
