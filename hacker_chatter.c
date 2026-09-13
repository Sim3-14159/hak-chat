#define _XOPEN_SOURCE_EXTENDED

#include <locale.h>
#include <ncursesw/ncurses.h>
#include <wchar.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_LINES 100
#define MAX_COLS 1024

// Control + <key> is actually an overlay of 0x1F on <key>
// Control + 'x' becomes 'x' & Ox1F
#ifndef CTRL
#define CTRL(c) ((c) & 0x1F)
#endif

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

static int cursor_y = 0;
static int cursor_x = 0;

struct WText {
    size_t len;
    wchar_t text[MAX_LINES][MAX_COLS];
    size_t line_count;
};

struct WText text = {
    .len = (size_t) MAX_COLS * MAX_LINES, .text = {L"Type something here."}, .line_count = 1};

// Draw the editor, and surrounding box
static void draw_editor(WINDOW *win)
{
    int editorheight = getmaxy(win);
    //werase(win);

    box(win, 0, 0); // border

    // Text starts at row 1, column 1 because row 0 & column 0 are used by the border.
    for (int y = 0; y < (int) text.line_count && y < editorheight - 2; y++)
        mvwaddwstr(win, y + 1, 1, text.text[y]);

    // Put cursor inside the border.
    wmove(win, cursor_y + 1, cursor_x + 1);

    //wrefresh(win);
}

// Insert into `text` a single wide character, at the text's cursor_y and cursor_x positions
static void insert(wchar_t ch)
{
    if (wcslen(text.text[cursor_y]) >= MAX_COLS - 1)
        return;

    for (size_t i = wcslen(text.text[cursor_y]) + 1; i > (size_t) cursor_x; i--)
        text.text[cursor_y][i] = text.text[cursor_y][i - 1];

    text.text[cursor_y][cursor_x] = ch;
    cursor_x++;
}

static void new_line(void)
{
    if (text.line_count >= MAX_LINES)
        return;

    // Move existing lines down.
    for (int i = text.line_count; i > cursor_y + 1; i--)
        wcscpy(text.text[i], text.text[i - 1]);

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
    wcscpy(text.text[cursor_y + 1], text.text[cursor_y] + cursor_x);

    text.text[cursor_y][cursor_x] = L'\0';

    text.line_count++;

    cursor_y++;
    cursor_x = 0;
}

static void backspace(void)
{
    if (cursor_x > 0) {
        size_t len = wcslen(text.text[cursor_y]);

        for (size_t i = cursor_x; i <= len; i++)
            text.text[cursor_y][i - 1] = text.text[cursor_y][i];

        cursor_x--;

        return;
    }

    if (cursor_y > 0) { // at beginning of line
        size_t previous_len = wcslen(text.text[cursor_y - 1]);
        size_t current_len = wcslen(text.text[cursor_y]);

        if (previous_len + current_len >= MAX_COLS)
            return;

        wcscat(text.text[cursor_y - 1], text.text[cursor_y]);

        // Move following lines up.
        for (int i = cursor_y; i < (int) text.line_count - 1; i++)
            wcscpy(text.text[i], text.text[i + 1]);

        text.text[text.line_count - 1][0] = L'\0';

        text.line_count--;

        cursor_y--;
        cursor_x = previous_len;
        return;
    }

    beep();
}

static void delete_char(void)
{
    size_t len = wcslen(text.text[cursor_y]);

    if (cursor_x < (int) len) {
        for (size_t i = cursor_x; i < len; i++)
            text.text[cursor_y][i] = text.text[cursor_y][i + 1];
        return;
    }

    if (cursor_y + 1 < (int) text.line_count) { // at beginning of line
        size_t current_len = wcslen(text.text[cursor_y]);
        size_t next_len = wcslen(text.text[cursor_y + 1]);

        if (current_len + next_len >= MAX_COLS)
            return;

        wcscat(text.text[cursor_y], text.text[cursor_y + 1]);

        // Move remaining lines up.
        for (int i = cursor_y + 1; i < (int) text.line_count - 1; i++)
            wcscpy(text.text[i], text.text[i + 1]);

        text.text[text.line_count - 1][0] = L'\0';

        text.line_count--;
    }
}

static void move_left(void)
{
    if (cursor_x > 0)
        cursor_x--;

    else if (cursor_y > 0) {
        cursor_y--;
        cursor_x = wcslen(text.text[cursor_y]);
    }
}

static void move_right(void)
{
    int len = wcslen(text.text[cursor_y]);

    if (cursor_x < len)
        cursor_x++;
    else if (cursor_y + 1 < (int) text.line_count) {
        cursor_y++;
        cursor_x = 0;
    }
}

static void move_up(void)
{
    if (cursor_y > 0) {
        cursor_y--;
        int len = wcslen(text.text[cursor_y]);
        if (cursor_x > len)
            cursor_x = len;

    } else if (cursor_y == 0)
        cursor_x = 0;
}

static void move_down(void)
{
    if (cursor_y + 1 < (int) text.line_count) {
        cursor_y++;
        int len = wcslen(text.text[cursor_y]);
        if (cursor_x > len)
            cursor_x = len;

    } else if (cursor_y + 1 == (int) text.line_count)
        cursor_x = wcslen(text.text[cursor_y]);
}

int main(void)
{
    setlocale(LC_ALL, ""); // enable UTF-8

    initscr();

    noecho();
    cbreak(); // no buffering
    keypad(stdscr, TRUE); // enable arrow and f keys
    curs_set(TRUE); // Show terminal cursor.

    if (LINES < 10 || COLS < 30) {
        endwin();

        fprintf(stderr, "Terminal is too small.\n");
        fprintf(stderr, "Please resize it to at least 30x10.\n");

        return 1;
    }

    int height = LINES - 4;
    int width = COLS - 4;

    WINDOW *editor = newwin(text.line_count + 2, width + 1, height - (text.line_count - 2), 1);
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
        mvwin(editor, height - (text.line_count - 2), 1);
        wresize(editor, text.line_count + 2, width + 1);

        draw_editor(editor);
        //box(messages, 0, 0);

        wint_t ch;
        int result = wget_wch(editor, &ch);

        if (result == OK) { // Normal character
            if (ch == L'\n' ||
                ch == L'\r') // newline (can be OK , L'\n' or KEY_CODE_YES , KEY_ENTER)
                new_line();
            else if (ch == 127) // backspace (can be OK - 127 or KEY_CODE_YES - KEY_BACKSPACE)
                backspace();
            else if (ch >= L' ') // Printable Unicode character.
                insert(ch);
            else if (ch == CTRL('x')) // quit
                running = FALSE;

        }

        else if (result == KEY_CODE_YES) { // Special key
            switch (ch) {
                case KEY_LEFT:
                    move_left();
                    break;

                case KEY_RIGHT:
                    move_right();
                    break;

                case KEY_UP:
                    move_up();
                    break;

                case KEY_DOWN:
                    move_down();
                    break;

                case KEY_BACKSPACE:
                    backspace();
                    break;

                case KEY_DC:
                    delete_char();
                    break;

                case KEY_ENTER:
                    new_line();
                    break;

                case KEY_RESIZE: {
                    getmaxyx(stdscr, height, width);
                    // TODO: get rid of dependency on -4
                    // (right now there are a lot of hardcoded numbers)
                    height -= 4;
                    width -= 4;
                    break;
                }
            }
        }
    }

    delwin(editor);
    delwin(messages);
    endwin();

    return 0;
}
