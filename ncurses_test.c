#include <ncurses.h>
#include <string.h>

char *repeat(char chr, int num, char buffer[]);

int main()
{
    int running = 1;
    // init ncurses
    initscr();

    // Optional configurations
    cbreak(); // Line buffering disabled
    //noecho(); // Don't echo user input
    keypad(stdscr, TRUE); // Enable arrow keys, F-keys, etc.

    int height, width;

    getmaxyx(stdscr, height, width);

    printw("Testing | Press 'q' to quit.");
    printw("\nThis should be on a new line");
    printw("\nScreen size: %dx%d\n\n", width, height);

    printw(".--------------------------------.\n");
    printw("|                                |\n");
    printw("|                                |\n");
    printw("|                                |\n");
    printw("|                                |\n");
    printw("|                                |\n");
    printw(",________________________________,\n");

    erase();
    getmaxyx(stdscr, height, width);
    char top[200];
    repeat('-', width - 10, top);
    printw(top);

    refresh();
    if (getch() == 'q') {
        running = 0;
    }
}

endwin();

return 0;
}

char *repeat(char chr, int num, char buffer[])
{
    char repeatedStr[200] = "";
    if (num >= 200)
        num = 200;
    for (int i = 0; i < num; i++)
        repeatedStr[i] = chr;
    strcpy(buffer, repeatedStr);
}
