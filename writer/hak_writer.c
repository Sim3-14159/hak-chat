#include "hak_writer.h"

#include <wctype.h>

static const wchar_t *submit_to_daemon(const wchar_t *line) {
  // TODO: write to the user's sent.chat so the daemon can pick it up
  // only the daemon should touch all.chat
  (void)line; // unused for now
  return L"Not sent: daemon submission is still TODO.";
}

hakWriterResult hak_writer_handle(const wchar_t *line) {
  hakWriterResult result;
  result.kind = HAK_WRITER_EMPTY;
  result.reply = NULL;

  if (line == NULL) {
    return result;
  }

  // ignore whitespace at either end
  size_t start = 0;
  size_t end = wcslen(line);
  while (start < end && iswspace(line[start])) {
    start++;
  }
  while (end > start && iswspace(line[end - 1])) {
    end--;
  }

  if (start == end) {
    return result;
  }

  // super duper secret thingy
  
  if (line[start] == L'/') {
    result.kind = HAK_WRITER_COMMAND;
    // /42 :D
    if (end - start == 3 && line[start + 1] == L'4' &&
        line[start + 2] == L'2') {
      result.reply = L"42: the answer to life, the universe, and everything.";
    } 
    else {
      result.reply = L"Unknown command.";
    }
    return result;
  }

  result.kind = HAK_WRITER_MESSAGE;
  result.reply = submit_to_daemon(line);
  return result;
}
