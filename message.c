#ifndef MESSAGE_H
#define MESSAGE_H
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdbool.h>

#include <time.h>

#include "wtext.h"

// 0: success, nonzero: failure
int send_message(struct WText *message)
{
    // ... TODO implement
    return 0;
}

struct WText *render_message(struct WText *message)
{
    // ... TODO implement
    return message;
}

/**
 * Is there a new message in the hak-chat-session?
 * Return true on yes, false on no.
 */
bool message_is_available()
{
    static time_t last_mtime = 0;
    static off_t last_size = 0;
    struct stat st;
    if (stat("/tmp/hak-chat-session/chat", &st) != 0)
        return -1; // error

    int added = (st.st_size > last_size);
    last_size = st.st_size;
    last_mtime = st.st_mtime;
    return added;
};

/**
 * Initialize a hak-chat session by making directory and touching file
  * Return 0 on success, and nonzero on error
  */
static int init_session()
{
    // mkdir /tmp/hak-chat-session/
    const char *dir_name = "/tmp/hak-chat-session/";
    mode_t old_umask;

    old_umask = umask(0);

    if (mkdir(dir_name, 0777) == 0) {
        return 0;
    } else {
        return 1;
    }
    umask(old_umask);

    // touch /tmp/hak-chat-session/chat
    const char *filename = "chat";

    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (fd == -1) {
        return 1;
    }

    close(fd);
    return 0;
}

#endif
