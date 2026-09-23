#ifndef EDITOR_H
#define EDITOR_H

#define _XOPEN_SOURCE_EXTENDED

#include <ncursesw/ncurses.h>
#include <wchar.h>
#include <stdio.h>
#include <stdlib.h>
#include "wtext.c"

static int cursor_y = 0;
static int cursor_x = 0;

// Draw the editor, and surrounding box
void draw_editor(WINDOW *win, struct WText *text)
{
    int editorheight = getmaxy(win);
    //werase(win);

    box(win, 0, 0); // border

    // Text starts at row 1, column 1 because row 0 & column 0 are used by the border.
    for (int y = 0; y < (int) text->line_count && y < editorheight - 2; y++)
        mvwaddwstr(win, y + 1, 1, text->arr[y]);

    // Put cursor inside the border.
    wmove(win, cursor_y + 1, cursor_x + 1);

    //wrefresh(win);
}

// Insert into `text` a single wide character, at the text's cursor_y and cursor_x positions
void insert(wchar_t ch, struct WText *text)
{
    if (wcslen(text->arr[cursor_y]) >= MAX_COLS - 1)
        return;

    for (size_t i = wcslen(text->arr[cursor_y]) + 1; i > (size_t) cursor_x; i--)
        text->arr[cursor_y][i] = text->arr[cursor_y][i - 1];

    text->arr[cursor_y][cursor_x] = ch;
    cursor_x++;
}

void new_line(struct WText *text)
{
    if (text->line_count >= MAX_LINES)
        return;

    // Move existing lines down.
    for (int i = text->line_count; i > cursor_y + 1; i--)
        wcscpy(text->arr[i], text->arr[i - 1]);

    /*
     * Move text after cursor to the new line.
     *
     * Example:
     *
     * Hello| World
     *
     * becomes:
     *
     * Hello
     * | World
     */
    wcscpy(text->arr[cursor_y + 1], text->arr[cursor_y] + cursor_x);

    text->arr[cursor_y][cursor_x] = L'\0';

    text->line_count++;

    cursor_y++;
    cursor_x = 0;
}

void backspace(struct WText *text)
{
    if (cursor_x > 0) {
        size_t len = wcslen(text->arr[cursor_y]);

        for (size_t i = cursor_x; i <= len; i++)
            text->arr[cursor_y][i - 1] = text->arr[cursor_y][i];

        cursor_x--;

        return;
    }

    if (cursor_y > 0) { // at beginning of line
        size_t previous_len = wcslen(text->arr[cursor_y - 1]);
        size_t current_len = wcslen(text->arr[cursor_y]);

        if (previous_len + current_len >= MAX_COLS)
            return;

        wcscat(text->arr[cursor_y - 1], text->arr[cursor_y]);

        // Move following lines up.
        for (int i = cursor_y; i < (int) text->line_count - 1; i++)
            wcscpy(text->arr[i], text->arr[i + 1]);

        text->arr[text->line_count - 1][0] = L'\0';
        text->line_count--;
        cursor_y--;
        cursor_x = previous_len;
        return;
    }

    beep(); // if at beginning of message and trying to press backspace, beep
}

void delete_char(struct WText *text)
{
    size_t len = wcslen(text->arr[cursor_y]);

    if (cursor_x < (int) len) {
        for (size_t i = cursor_x; i < len; i++)
            text->arr[cursor_y][i] = text->arr[cursor_y][i + 1];
        return;
    }

    if (cursor_y + 1 < (int) text->line_count) { // at beginning of line
        size_t current_len = wcslen(text->arr[cursor_y]);
        size_t next_len = wcslen(text->arr[cursor_y + 1]);

        if (current_len + next_len >= MAX_COLS)
            return;

        wcscat(text->arr[cursor_y], text->arr[cursor_y + 1]);

        // Move remaining lines up.
        for (int i = cursor_y + 1; i < (int) text->line_count - 1; i++)
            wcscpy(text->arr[i], text->arr[i + 1]);

        text->arr[text->line_count - 1][0] = L'\0';
        text->line_count--;
        return;
    }

    beep(); // if at end of message and trying to delete, beep
}

void move_left(struct WText *text)
{
    if (cursor_x > 0)
        cursor_x--;

    else if (cursor_y > 0) {
        cursor_y--;
        cursor_x = wcslen(text->arr[cursor_y]);
    }
}

void move_right(struct WText *text)
{
    int len = wcslen(text->arr[cursor_y]);

    if (cursor_x < len)
        cursor_x++;
    else if (cursor_y + 1 < (int) text->line_count) {
        cursor_y++;
        cursor_x = 0;
    }
}

void move_up(struct WText *text)
{
    if (cursor_y > 0) {
        cursor_y--;
        int len = wcslen(text->arr[cursor_y]);
        if (cursor_x > len)
            cursor_x = len;

    } else if (cursor_y == 0)
        cursor_x = 0;
}

void move_down(struct WText *text)
{
    if (cursor_y + 1 < (int) text->line_count) {
        cursor_y++;
        int len = wcslen(text->arr[cursor_y]);
        if (cursor_x > len)
            cursor_x = len;

    } else if (cursor_y + 1 == (int) text->line_count)
        cursor_x = wcslen(text->arr[cursor_y]);
}

#endif
