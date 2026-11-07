#ifndef HAK_WRITER_H
#define HAK_WRITER_H

#include <wchar.h>

typedef enum {
  HAK_WRITER_EMPTY,
  HAK_WRITER_MESSAGE,
  HAK_WRITER_COMMAND
} 
hakWriterKind;

typedef struct {
  hakWriterKind kind;
  const wchar_t *reply;
} 
hakWriterResult;

// takes ws_cstr(message), NULL or just whitespace counts as empty
// doesnt change the input. reply is fixed text (or NULL), dont free it
// MESSAGE means normal text, doesnt mean it got sent
hakWriterResult hak_writer_handle(const wchar_t *line);

#endif
