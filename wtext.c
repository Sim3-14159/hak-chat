#include <stdlib.h>
#include <wchar.h>
#include <stdbool.h>
#include "wtext.h"

void wl_delete(struct WLine *line);

/******************************* WText functions **************************************/

/**
  * Adds a wchar_t to a WText object.
  * Automatically allocates memeory for itself when it needs it.
  * returns 0 on success and 1 on error.
  */
int wt_addChar(struct WText *text, wchar_t chr)
{
    // TODO: fix
    struct WText *current = text;
    while (current->next)
        current = current->next;
    if (current->line_count == MAX_LEN - 1) {
        current->next = malloc(sizeof(struct WText));
        if (!current->next)
            return 1;
        current->next->line_count = 1;
        current->next->line->arr[0] = chr;
    } else {
        current->content[current->line_count++] = chr;
        // add null to the end for functions like addwstr or printf to work
        current->content[current->line_count] = L'\0';
    }
    return 0;
}

/**
* Get the line number, of `text`. (0 indexed)
*/
struct WText *wt_getLine(struct WText *text, int lineNo)
{
    struct WText *current = text;
    for (int currentLineNo = 0; currentLineNo < lineNo;) {
        current = current->next;
    }
    return current;
}

void wt_delete(struct WText *text)
{
    struct WText *current = text;
    struct WText *next = text->next;
    while (next) {
        wl_delete(current->content);
        free(current);
        current = next;
        next = next->next;
    }
}

/*************** WLine functions ********************/

void wl_set(struct WLine *line, const wchar_t *content, size_t content_size)
{
    int written_chars = 0;
    while (written_chars < content_size) {
        for (int i = 0; i < MAX_LEN - 1; i++) {
            line->arr[i] = content[i];
        }
        line->arr[MAX_LEN - 1] = L'\0';
        if (content_size) {
        }
    }
}

void wl_addChar(struct WLine *line, wchar_t chr)
{
}

void wl_delete(struct WLine *line)
{
    struct WLine *current = line;
    struct WLine *next = line->next;
    while (next) {
        free(current);
        current = next;
        next = next->next;
    }
}

int wl_setChar(struct WLine *line, int index, wchar_t chr)
{
    // TODO: implement
}
