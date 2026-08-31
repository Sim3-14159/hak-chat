#include <ncurses.h>
#include <string.h>

void repeat(char chr, int num, char buffer[]);

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

    while (running) { 
        erase();
        getmaxyx(stdscr, height, width);
        printw("Testing | Press 'q' to quit.");
        printw("\nThis should be on a new line");
        printw("\nScreen size: %dx%d\n\n", width, height);

        char top[200];
        repeat('-', width - 10, top);

	printw("    .");
        printw(top);
	printw(".  \n");
        repeat(' ', width - 10, top);
        printw("    |");
	printw(top);
	printw("|  \n");

        repeat('_', width - 10, top);
        printw("    ,");
	printw(top);
	printw(",  \n");

        refresh();
        char key = getch();

    }
    endwin();
    return 0;
}

void repeat(char chr, int num, char buffer[])
{
    char repeatedStr[200] = "";
    if (num >= 200)
        num = 200;
    for (int i = 0; i < num; i++)
        repeatedStr[i] = chr;
    strcpy(buffer, repeatedStr);
}
