#include "wtext.h"
#include <stdlib.h>
#include <wchar.h>

/**
  * Adds a wchar_t to a WText object.
  * Automatically allocates memeory for itself when it needs it.
  * returns 0 on success and 1 on error.
  */
int wt_addChar(struct WText *text, wchar_t chr)
{
    struct WText *current = text;
    while (current->next)
        current = current->next;
    if (current->len == MAX_LEN - 1) {
        current->next = malloc(sizeof(struct WText));
        if (!current->next)
            return 1;
        current->next->len = 1;
        current->next->arr[0] = chr;
    } else {
        current->arr[current->len++] = chr;
        // add null to the end for functions like addwstr or printf to work
        current->arr[current->len] = L'\0';
    }
    return 0;
}

void wt_delete(struct WText *text)
{
    struct WText *current = text;
    struct WText *next = text->next;
    while (next) {
        free(current);
        current = next;
        next = next->next;
    }
}
