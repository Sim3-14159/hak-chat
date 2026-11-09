#define _XOPEN_SOURCE_EXTENDED
#include "wstring.h"

#include <locale.h>
#include <ncursesw/ncurses.h>
#include <wchar.h>
#include <stdio.h>
#include <stdlib.h>

void draw_editor(WINDOW *win, WString *text, unsigned int height, unsigned int width)
{
    int editorheight = getmaxy(win);
    //werase(win);

    box(win, 0, 0); // border

    // WS_Stats *stats = ws_fit_window(text, win);

    // Text starts at row 1, column 1 because row 0 & column 0 are used by the border.
    //for (int y = 0; y < (int) stats->height && y < editorheight - 2; y++) {
    //    mvwaddwstr(win, y + 1, 1,
    //               wl_getChar(wt_getLine(text, y),
    //                          0)); // 0 IS A TEST TODO: make sure that this works actually
    //    //mvwaddwstr(win, y + 1, 1, text->arr[y]);
    //}

    // Put cursor inside the border.
    // wmove(win, cursor_y + 1, cursor_x + 1);

    wrefresh(win);
}
