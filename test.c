#define _XOPEN_SOURCE_EXTENDED 1
#include <curses.h>
#include <wchar.h>

int main(void) {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE); // Enable special key parsing

    wint_t ch;
    printw("Press keys (Backspace to test, q to quit):\n");

    while (1) {
        int status = get_wch(&ch);

        if (status == KEY_CODE_YES) {
            if (ch == KEY_BACKSPACE) {
                printw("[KEY_BACKSPACE detected]\n");
            } else {
                printw("[Other function key: %d]\n", (int)ch);
            }
        } else if (status == OK) {
            if (ch == 8 || ch == 127) {
                printw("[ASCII Backspace/DEL: %d]\n", (int)ch);
            } else if (ch == L'q') {
                break;
            } else {
                printw("[Char: %lc]\n", (wchar_t)ch);
            }
        }
    }

    endwin();
    return 0;
}

