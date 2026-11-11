#define _XOPEN_SOURCE_EXTENDED
// allow wide char stuff
#include "wstring.h"

#include <locale.h>
#include <ncursesw/ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#define CTRL(c) ((c) & 0x1F)

int main()
{
    typedef struct WString *ws;

    setlocale(LC_ALL, ""); // enable UTF-8
    initscr();
    noecho();
    cbreak(); // no buffering
    keypad(stdscr, true); // enable arrow and f keys
    curs_set(TRUE); // Show terminal cursor.

    ws message = ws_new();
    const wchar_t initial_content[] = L"Type something...\n";
    for (size_t i = 0; i < wcslen(initial_content); i++)
        ws_add(message, initial_content[i]);

    if (LINES < 10 || COLS < 30) {
        endwin();
        fprintf(stderr, "Terminal is too small.\nPlease resize it to at least 30x10.\n");
        return 1;
    }

    WINDOW *editor = newwin(5, COLS, LINES - 5, 0);

    if (!editor) {
        endwin();
        fprintf(stderr, "Failed to create window\n");
        return 1;
    }

    refresh();

    keypad(editor, true);
    werase(editor);
    box(editor, 0, 0);
    wmove(editor, 1, 1);
    waddnwstr(editor, message->data, message->count);
    wrefresh(editor);

    // MAIN EVENT LOOP
    bool running = true;
    while (running) {
        wint_t ch;
        int result = wget_wch(stdscr, &ch);

        switch (result) {
            case OK: // Normal character
                // newline (can be OK , L'\n' or KEY_CODE_YES , KEY_ENTER)
                if (ch == L'\n' || ch == L'\r') {
                    ws_add(message, L'\n');
                } // backspace (can be OK - 127 or KEY_CODE_YES - KEY_BACKSPACE)
                else if (ch == 127) {
                    ws_pop(message);
                } // Printable Unicode character.
                else if (ch >= L' ') {
                    ws_add(message, ch);
                } // quit
                else if (ch == CTRL(L'x') || ch == CTRL(L'q') || ch == CTRL(L'c'))
                    running = false;

                break;

            case KEY_CODE_YES: // Special key
                switch (ch) {
                    case KEY_LEFT:
                        // move_left(text); break; case KEY_RIGHT: move_right(text); break; case KEY_UP: move_up(text); break; case KEY_DOWN: move_down(text); break;

                    case KEY_BACKSPACE:
                        // backspace(text);
                        ws_pop(message);
                        break;

                    case KEY_DC:
                        // delete_char(text);
                        break;

                    case KEY_ENTER:
                        // new_line(text);
                        break;

                    case KEY_RESIZE:
                        // getmaxyx(stdscr, height, width);
                        // // TODO: get rid of dependency on -4
                        // // (right now there are a lot of hardcoded numbers)
                        // height -= 4;
                        // width -= 4;
                        break;
                }

                break;
        }

        werase(editor);
        box(editor, 0, 0);
        wmove(editor, 1, 1);
        waddnwstr(editor, message->data, message->count);
        wrefresh(editor);
    }

    // END
    endwin();

    fprintf(stdout, "[Exit]\n");
    fflush(stdout);

    ws_free(message);

    return 0;
}
