#include "wtext.h"
#define _XOPEN_SOURCE_EXTENDED

#include <locale.h>
#include <ncursesw/ncurses.h>
#include <wchar.h>
#include <stdio.h>
#include <stdlib.h>

#include "editor.c"

// Control + <key> is actually an overlay of 0x1F on <key>
// Control + 'x' becomes 'x' & Ox1F
#ifndef CTRL
#define CTRL(c) ((c) & 0x1F)
#endif

int main(void)
{
    struct WText *text = malloc(sizeof(struct WText));
    struct WLine *line = malloc(sizeof(struct WLine));
    wl_set(line, L"Type anything...");

    setlocale(LC_ALL, ""); // enable UTF-8

    initscr();

    noecho();
    cbreak(); // no buffering
    keypad(stdscr, TRUE); // enable arrow and f keys
    curs_set(TRUE); // Show terminal cursor.

    if (LINES < 10 || COLS < 30) {
        endwin();

        perror("Terminal is too small.\n");
        perror("Please resize it to at least 30x10.\n");

        return 1;
    }

    int height = LINES - 4;
    int width = COLS - 4;

    WINDOW *editor = newwin(text->line_count + 2, width + 1, height - (text->line_count - 2), 1);
    WINDOW *messages = newwin((int) (height * 2 / 3 - 2), width + 1, 1, 1);

    if (editor == NULL) {
        endwin();
        fprintf(stderr, "Failed to create editor window.\n");
        return 1;
    }
    if (messages == NULL) {
        endwin();
        fprintf(stderr, "Failed to create messages window.\n");
        return 1;
    }

    keypad(editor, TRUE); // enable arrow and f keys

    cursor_y = 0;
    cursor_x = 0;
    int running = TRUE;

    while (running) {
        // TODO: fix ordering of this so cursor doesn't flash and move to other places
        werase(editor);
        wrefresh(editor);
        mvwin(editor, height - (text->line_count - 2), 1);
        wresize(editor, text->line_count + 2, width + 1);

        draw_editor(editor, text);
        //box(messages, 0, 0);

        wint_t ch;
        int result = wget_wch(editor, &ch);

        switch (result) {
            case OK: // Normal character
                if (ch == L'\n' ||
                    ch == L'\r') // newline (can be OK , L'\n' or KEY_CODE_YES , KEY_ENTER)
                    new_line(text);
                else if (ch == 127) // backspace (can be OK - 127 or KEY_CODE_YES - KEY_BACKSPACE)
                    backspace(text);
                else if (ch >= L' ') // Printable Unicode character.
                    insert(ch, text);
                else if (ch == CTRL('x')) // quit
                    running = FALSE;

                break;

            case KEY_CODE_YES: // Special key
                switch (ch) {
                    case KEY_LEFT:
                        move_left(text);
                        break;

                    case KEY_RIGHT:
                        move_right(text);
                        break;

                    case KEY_UP:
                        move_up(text);
                        break;

                    case KEY_DOWN:
                        move_down(text);
                        break;

                    case KEY_BACKSPACE:
                        backspace(text);
                        break;

                    case KEY_DC:
                        delete_char(text);
                        break;

                    case KEY_ENTER:
                        new_line(text);
                        break;

                    case KEY_RESIZE:
                        getmaxyx(stdscr, height, width);
                        // TODO: get rid of dependency on -4
                        // (right now there are a lot of hardcoded numbers)
                        height -= 4;
                        width -= 4;
                        break;
                }
                break;
        }
    }

    delwin(editor);
    delwin(messages);
    endwin();

    fprintf(stdout, "[Exit]\n");
    fflush(stdout);

    return 0;
}
