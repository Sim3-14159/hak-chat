#define _XOPEN_SOURCE_EXTENDED
#include "wstring.h"

#include <locale.h>
#include <ncursesw/ncurses.h>
#include <wchar.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    WString *lines;
    size_t count;
    size_t capacity;
} MWString; // Multiline wide String
