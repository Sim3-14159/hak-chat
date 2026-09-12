#define _XOPEN_SOURCE_EXTENDED 1

#include <ncurses.h>
#include <locale.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


struct WString {
    size_t len;
    wchar_t *content;
    size_t pos;
};


void repeat(wchar_t chr, int num, wchar_t buffer[]);
void insert(int pos, wchar_t val, wchar_t str[]);

int main()
{
    setlocale(LC_ALL, "");
    int running = 1;
    // init ncurses
    initscr();

    cbreak(); // Line buffering disabled
    noecho();
    keypad(stdscr, TRUE); // Enable arrow keys, F-keys, etc.

    int height, width;
    getmaxyx(stdscr, height, width);
    int y, x;
    getyx(stdscr, y, x);
    // Cursor position, within textarea (|Hello = 0, H|ello = 1, He|llo = 2, etc)
    int textCursorOffset = 0;

    wchar_t *userInput = malloc(10000 * sizeof(wchar_t));
    int where = 0;

    while (running) {
        erase();
        getmaxyx(stdscr, height, width);
        getyx(stdscr, y, x);

        addwstr(L"Testing | Press 'q' to quit.");

        wchar_t *buffer = (wchar_t *)malloc(30 * sizeof(wchar_t));
        swprintf(buffer, 30, L"Screen size: %dx%d\n\n", width, height);
        addwstr(buffer);
        free(buffer);

        for (int tempHeight = height; tempHeight > 5; tempHeight--) {
            addwstr(L"\n");
        }

        wchar_t top[500];
        repeat(L'─', width - 10, top);

        addwstr(L"    ╭");
        addwstr(top);
        addwstr(L"╮  \n");

        repeat(L' ', width - 10 - wcslen(userInput), top);
        addwstr(L"    │");
        addwstr(userInput);
        addwstr(top);
        addwstr(L"│  \n");

        repeat(L'─', width - 10, top);
        addwstr(L"    ╰");
        addwstr(top);
        addwstr(L"╯  \n");

        move(height - 2, 5 + textCursorOffset);

        wint_t key;
        int status = get_wch(&key);
        switch (status) {
            case OK: // Regular keypress
                insert(where, key, userInput);
                where++;
                textCursorOffset++;
                break;

            case KEY_CODE_YES: // Control or arrow keycode
                switch (key) {
                    case KEY_LEFT:
                        if (textCursorOffset > 0) {
                            textCursorOffset--;
                            where--;
                        }
                        break;
                    case KEY_RIGHT:
                        if (textCursorOffset < (int)wcslen(userInput)) {
                            textCursorOffset++;
                            where++;
                        }
                        break;
                    case KEY_DOWN:
                        // TODO: add KEY_DOWN and KEY_UP movement
                        userInput[where] = L'D';
                        where++;
                        break;
                    case KEY_UP:
                        userInput[where] = L'R';
                        where++;
                        break;
                    case 18: // Ctrl+R
                        userInput[where] = L'^';
                        where++;
                        break;
                    case 8: case KEY_BACKSPACE: // Backspace
                        if (where <= 0) {
                            addwstr(L"\a"); // Beep sound
                            break;
                        }
                        where--;
                        textCursorOffset--;
                        userInput[wcslen(userInput) - 2] = '\0'; // remove end of text
                        break;
                    default:
                        addwstr(L"Unknown control key pressed: ");
                        break;
                }
                break;

            default:
                addwstr(L"Error reading key");
                break;
        }
        refresh();
    }
    endwin();
    return 0;
}

void repeat(wchar_t chr, int num, wchar_t buffer[])
{
    wchar_t repeatedStr[500] = L"";
    if (num >= 500)
        num = 500;
    for (int i = 0; i < num; i++)
        repeatedStr[i] = chr;
    wcscpy(buffer, repeatedStr);
}

// NOT OVERFLOW SAFE! TODO: Make WString struct and supporting memory safe funcs
void insert(int pos, wchar_t val, wchar_t str[])
{
    for (int tempPos = wcslen(str); tempPos > pos; tempPos--)
        str[tempPos] = str[tempPos - 1];
    str[pos] = val;
}
