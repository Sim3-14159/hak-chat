#ifndef WTEXT_H
#define WTEXT_H

#include <stdlib.h>
#include <wchar.h>

#define MAX_LEN 1024

struct WLine {
    size_t len; // same as wcslen(arr), only used because wcslen is inneficient and this is faster
    wchar_t *arr; // must be less than MAX_LEN
    // if the content of the line is greater than MAX_LEN, then link to another line.
    // (NULL means finished)
    struct WLine *next;
};

/**
  * a collection of `WLine`s
  */
struct WText {
    size_t line_count;
    struct WLine *line;
    struct WText *next;
};

#endif
