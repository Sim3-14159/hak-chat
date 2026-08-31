#define _XOPEN_SOURCE_EXTENDED 1

#include <ncurses.h>
#include <locale.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void repeat(wchar_t chr, int num, wchar_t buffer[]);

int main()
{
    setlocale(LC_ALL, "");
    int running = 1;
    // init ncurses
    initscr();

    // Optional configurations
    cbreak(); // Line buffering disabled
    noecho(); // Don't echo user input
    //keypad(stdscr, TRUE); // Enable arrow keys, F-keys, etc.

    int height, width;
    getmaxyx(stdscr, height, width);

    wchar_t *userInput = malloc(10000 * sizeof(wchar_t));
    int where = 0;

    while (running) { 
        erase();
        getmaxyx(stdscr, height, width);
        addwstr(L"Testing | Press 'q' to quit.");
        addwstr(L"\nThis should be on a new line");

        wchar_t *buffer = (wchar_t *)malloc(30 * sizeof(wchar_t));
        swprintf(buffer, 30, L"Screen size: %dx%d\n\n", width, height);
        addwstr(buffer);
        free(buffer);

        wchar_t top[500];
        repeat(L'─', width - 10, top);

        addwstr(L"    ╭");
        addwstr(top);
        addwstr(L"╮  \n");

        repeat(L' ', width - 10 - wcslen(userInput), top);
        addwstr(L"    |");
        addwstr(userInput);
        addwstr(top);
        addwstr(L"|  \n");

        repeat(L'─', width - 10, top);
        addwstr(L"    ╰");
        addwstr(top);
        addwstr(L"╯  \n");

        wint_t key;
        int status = get_wch(&key);
        switch (status) {
            case OK: // Regular keypress
                switch (key) {
                    case 8: case 127: // Backspace or Delete
                        if (where <= 0) {
                            printf("\a"); // Beep sound
                            break;
                        }
                        where--;
                        userInput[where] = '\0';
                        break;
                    default:
                        userInput[where] = key;
                        where++;
                        break;
                }
                break;

            case KEY_CODE_YES: // Control keycode
                switch (key) {
                    case KEY_LEFT:
                        userInput[where] = L'L';
                        where++;
                        addwstr(L"You pressed LEFT");
                        break;
                    case 18: // Ctrl+R
                        addwstr(L"You pressed Ctrl+R");
                        break;
                    case KEY_UP:
                        addwstr(L"You pressed UP");
                        break;
                    case KEY_DOWN:
                        addwstr(L"You pressed DOWN");
                        break;
                    case KEY_RIGHT:
                        addwstr(L"You pressed RIGHT");
                        break;
                    default:
                        addwstr(L"Unknown control key pressed");
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
