#define _XOPEN_SOURCE_EXTENDED
// allow wide char stuff
#include "wstring.h"

#include <locale.h>
#include <ncursesw/ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#define CTRL(c) ((c) & 0x1F)

int main() {
  typedef struct WString *ws;

  setlocale(LC_ALL, ""); // enable UTF-8
  initscr();
  noecho();
  cbreak();             // no buffering
  keypad(stdscr, TRUE); // enable arrow and f keys
  curs_set(TRUE);       // Show terminal cursor.

  ws message = ws_new(L"Type something...\n");

  WINDOW *editor = newwin(2, LINES, COLS, 1);

  if (LINES < 10 || COLS < 30) {
    endwin();
    fprintf(stderr,
            "Terminal is too small.\nPlease resize it to at least 30x10.\n");
    return 1;
  }
  // MAIN EVENT LOOP
  wint_t ch;
  addwstr(ws_cstr(message));
  wrefresh(stdscr);
  bool running = true;
  while (running) {
    int result = wget_wch(stdscr, &ch);
    wchar_t wstr[2] = {(wchar_t)ch, L'\0'};

    switch (result) {
    case OK: // Normal character
      waddnwstr(stdscr, wstr, 1);

      // newline (can be OK , L'\n' or KEY_CODE_YES , KEY_ENTER)
      if (ch == L'\n' || ch == L'\r') {
        // new_line(text);
        // TODO: add proper functoins to make this work
        ws_
      } // backspace (can be OK - 127 or KEY_CODE_YES - KEY_BACKSPACE)
      else if (ch == 127) {
        ws_pop(message);
      } // Printable Unicode character.
      else if (ch >= L' ') {
        WString *tmp = ws_new((wchar_t[]){ch, L'\0'});
        ws_concat(message, tmp);
        ws_free(tmp);
      } // quit
      else if (ch == CTRL('x'))
        running = false;

      break;

    case KEY_CODE_YES: // Special key
      switch (ch) {
      case KEY_LEFT:
        // move_left(text);
        break;

      case KEY_RIGHT:
        // move_right(text);
        break;

      case KEY_UP:
        // move_up(text);
        break;

      case KEY_DOWN:
        // move_down(text);
        break;

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

    wrefresh(editor);
  }

  // END
  endwin();

  fprintf(stdout, "[Exit]\n");
  fflush(stdout);

  ws_free(message);

  return 0;
}
