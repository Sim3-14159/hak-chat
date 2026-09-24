#include <stdlib.h>
#include <wchar.h>
#include <stdbool.h>
#include "wtext.h"

void wl_delete(struct WLine *line);
int wl_setChar(struct WLine *line, int index, wchar_t chr);

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
        wl_setChar(current, current->line_count, chr);
        // current->content[current->line_count++] = chr;
        // add null to the end for functions like addwstr or printf to work
        wl_setChar(current, current->line_count, L'\0');
        // current->content[current->line_count] = L'\0';
    }
    return 0;
}

/**
* Get the line number `lineNo`, of `text`. (0 indexed)
*/
struct WLine *wt_getLine(struct WText *text, int lineNo)
{
    // struct WText *current = text;
    // for (int currentLineNo = 0; currentLineNo < lineNo;) {
    //     current = current->next;
    // }
    // return current;
    struct WLine *current = text->line;
    struct WLine *last = text->line;
    for (int currentLineNo = 0; currentLineNo < lineNo; currentLineNo++) {
        while (current->next) // until we reach the end of the line (loop through linked list)
            current = current->next;
        current = last->next;
        last = last->next;
    }

    return current;
}

void wt_delete(struct WText *text)
{
    struct WText *current = text;
    struct WText *next = text->next;
    while (next) {
        wl_delete(current->line);
        free(current);
        current = next;
        next = next->next;
    }
}

/*************** WLine functions ********************/

void wl_setTo(struct WLine *line, const wchar_t *content, size_t content_size)
{
    int written_chars = 0;
    while (written_chars < content_size) {
        for (int i = 0; i < MAX_LEN - 1; i++) {
            line->arr[i] = content[i];
        }
        line->arr[MAX_LEN - 1] = L'\0';
        if (content_size) {
            //   TODO: implement
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

// 1 on error, 0 on OK
int wl_setChar(struct WLine *line, int index, wchar_t chr)
{
    struct WLine *current = line;
    while (index >= MAX_LEN) {
        current = current->next;
        if (!current)
            return 1; // not enough links to get index
        index -= MAX_LEN - 1; // account for NULL terminator
    }
    if (index >= current->len)
        return 1; // string not logn enough to get index
    current->arr[index] = chr;
    return 0;
}
