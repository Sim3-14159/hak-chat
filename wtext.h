#ifndef WTEXT_H
#define WTEXT_H

#include <stdlib.h>

#define MAX_LINES 100
#define MAX_COLS 1024

struct WText {
    size_t len;
    wchar_t text[MAX_LINES][MAX_COLS];
    size_t line_count;
};
#endif
