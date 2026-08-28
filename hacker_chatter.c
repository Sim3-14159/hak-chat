/** Hacker Chatter: an instant chatting system for multi user POSIX environments.
 *
 * only for the *coolest* of coders XD
 */

#define _XOPEN_SOURCE 700
#define _POSIX_C_SOURCE 200809L

#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <pwd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#ifndef CHAT_FILE
#define CHAT_FILE "/tmp/hak-chat/chat"
#endif

#ifndef CHAT_SESSION
#define CHAT_SESSION "js_chat"
#endif

static int run_command(char *const argv[], int quiet)
{
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        return -1;
    }

    if (pid == 0) {
        if (quiet) {
            int null = open("/dev/null", O_WRONLY);

            if (null != -1) {
                dup2(null, STDOUT_FILENO);
                dup2(null, STDERR_FILENO);
                close(null);
            }
        }

        execvp(argv[0], argv);

        // only gets here if exec explodes
        perror(argv[0]);
        _exit(127);
    }

    int status;

    while (waitpid(pid, &status, 0) == -1) {
        if (errno != EINTR) {
            perror("waitpid");
            return -1;
        }
    }

    if (WIFEXITED(status))
        return WEXITSTATUS(status);

    return -1;
}

static void attach_to_chat(void)
{
    if (getenv("TMUX")) {
        execlp("tmux", "tmux", "switch-client", "-t", CHAT_SESSION, (char *) NULL);
    } else {
        execlp("tmux", "tmux", "attach-session", "-t", CHAT_SESSION, (char *) NULL);
    }

    perror("tmux");
    exit(EXIT_FAILURE);
}

static int find_executable(const char *argv0, char *result, size_t result_size)
{
    // If argv[0] already contains a slash,
    // there's no reason to go digging through PATH.
    if (strchr(argv0, '/')) {
        return realpath(argv0, result) ? 0 : -1;
    }

    const char *path = getenv("PATH");

    if (!path) {
        errno = ENOENT;
        return -1;
    }

    char *copy = strdup(path);

    if (!copy)
        return -1;

    int found = -1;
    char *save = NULL;

    for (char *dir = strtok_r(copy, ":", &save); dir; dir = strtok_r(NULL, ":", &save)) {
        if (*dir == '\0')
            dir = ".";

        int written = snprintf(result, result_size, "%s/%s", dir, argv0);

        if (written < 0 || (size_t) written >= result_size)
            continue;

        if (access(result, X_OK) == 0) {
            char resolved[PATH_MAX];

            if (realpath(result, resolved))
                snprintf(result, result_size, "%s", resolved);

            found = 0;
            break;
        }
    }

    free(copy);

    if (found != 0)
        errno = ENOENT;

    return found;
}

static char *shell_quote(const char *text)
{
    size_t length = 3;

    for (const char *p = text; *p; p++) {
        if (*p == '\'')
            length += 4;
        else
            length++;
    }

    char *quoted = malloc(length);

    if (!quoted)
        return NULL;

    char *out = quoted;

    *out++ = '\'';

    for (const char *p = text; *p; p++) {
        if (*p == '\'') {
            memcpy(out, "'\\''", 4);
            out += 4;
        } else {
            *out++ = *p;
        }
    }

    *out++ = '\'';
    *out = '\0';

    return quoted;
}

static const char *get_username(void)
{
    const char *name = getenv("USER");

    if (name && *name)
        return name;

    struct passwd *pw = getpwuid(getuid());

    if (pw && pw->pw_name)
        return pw->pw_name;

    return "mystery_hacker";
}

static int write_all(int fd, const char *buffer, size_t length)
{
    while (length > 0) {
        ssize_t written = write(fd, buffer, length);

        if (written < 0) {
            if (errno == EINTR)
                continue;

            return -1;
        }

        buffer += written;
        length -= (size_t) written;
    }

    return 0;
}

static int set_file_lock(int fd, short lock_type)
{
    struct flock lock = {.l_type = lock_type, .l_whence = SEEK_SET, .l_start = 0, .l_len = 0};

    int command;

    if (lock_type == F_UNLCK)
        command = F_SETLK;
    else
        command = F_SETLKW;

    while (fcntl(fd, command, &lock) == -1) {
        if (errno != EINTR)
            return -1;
    }

    return 0;
}

static int input_mode(void)
{
    int chat_fd = open(CHAT_FILE, O_WRONLY | O_APPEND);

    if (chat_fd == -1) {
        fprintf(stderr, "can't open %s: %s\n", CHAT_FILE, strerror(errno));

        return 1;
    }

    // usernames look better in caps
    const char *raw_name = get_username();

    size_t name_len = strlen(raw_name);

    char *username = malloc(name_len + 1);

    if (!username) {
        perror("malloc");
        close(chat_fd);
        return 1;
    }

    for (size_t i = 0; i < name_len; i++) {
        username[i] = (char) toupper((unsigned char) raw_name[i]);
    }

    username[name_len] = '\0';

    char *line = NULL;
    size_t cap = 0;

    while (getline(&line, &cap, stdin) != -1) {
        // getline keeps the newline, which is mildly annoying
        line[strcspn(line, "\r\n")] = '\0';

        time_t now = time(NULL);
        struct tm tm_now;
        char timestamp[16];

        if (localtime_r(&now, &tm_now) == NULL ||
            strftime(timestamp, sizeof(timestamp), "%I:%M %p", &tm_now) == 0) {
            snprintf(timestamp, sizeof(timestamp), "??:?? ??");
        }

        const char *text = line;

        /*
         * extremely important feature
         */
        if (strcmp(line, "/42") == 0) {
            text =
                "\033[36;1;3;5m4\033[31m2\033[35m \033[33m(\033[35mT\033[36mH\033[34mE\033[33m "
                "\033[35mA\033[32mN\033[32mS\033[32mW\033[33mE\033[34mR\033[32m "
                "\033[31mT\033[32mO\033[35m \033[33mL\033[32mI\033[34mF\033[33mE\033[35m,\033[34m "
                "\033[33mT\033[33mH\033[35mE\033[36m "
                "\033[31mU\033[34mN\033[31mI\033[32mV\033[31mE\033[36mR\033[35mS\033[33mE\033[35m,"
                "\033[33m \033[33mA\033[36mN\033[31mD\033[33m "
                "\033[33mE\033[31mV\033[35mE\033[35mR\033[33mY\033[34mT\033[34mH\033[35mI\033["
                "35mN\033[33mG\033[35m)\033[0m";
        }

        int needed = snprintf(NULL, 0, "[%s @ %s]: %s\n", username, timestamp, text);

        if (needed < 0)
            continue;

        char *message = malloc((size_t) needed + 1);

        if (!message) {
            perror("malloc");
            break;
        }

        snprintf(message, (size_t) needed + 1, "[%s @ %s]: %s\n", username, timestamp, text);

        if (set_file_lock(chat_fd, F_WRLCK) == -1) {
            perror("chat lock");
            free(message);
            break;
        }

        if (write_all(chat_fd, message, (size_t) needed) == -1) {
            fprintf(stderr, "write failed: %s\n", strerror(errno));

            set_file_lock(chat_fd, F_UNLCK);

            free(message);
            break;
        }

        set_file_lock(chat_fd, F_UNLCK);

        free(message);
    }

    free(line);
    free(username);

    close(chat_fd);

    return 0;
}

int main(int argc, char **argv)
{
    // --input is used by the right-side tmux pane
    if (argc == 2 && strcmp(argv[1], "--input") == 0) {
        return input_mode();
    }

    if (argc != 1) {
        fprintf(stderr, "usage: %s\n", argv[0]);

        return 1;
    }

    /*
     * Make sure the chat actually exists before
     * doing a bunch of tmux stuff.
     */
    int chat_fd = open(CHAT_FILE, O_WRONLY | O_APPEND);

    if (chat_fd == -1) {
        fprintf(stderr,
                "can't open %s: %s\n"
                "make sure the chat file exists and is writable\n",
                CHAT_FILE, strerror(errno));

        return 1;
    }

    close(chat_fd);

    /*
     * If a session already exists, just jump into it.
     */
    char *has_session[] = {"tmux", "has-session", "-t", CHAT_SESSION, NULL};

    if (run_command(has_session, 1) == 0)
        attach_to_chat();

    /*
     * tmux needs the real path because it launches
     * another copy of this program for the input pane.
     */
    char executable[PATH_MAX];

    if (find_executable(argv[0], executable, sizeof(executable)) != 0) {
        fprintf(stderr, "can't figure out where this program lives: %s\n", strerror(errno));

        return 1;
    }

    char *quoted_executable = shell_quote(executable);

    if (!quoted_executable) {
        perror("malloc");
        return 1;
    }

    size_t command_size = strlen(quoted_executable) + strlen("exec  --input") + 1;

    char *input_command = malloc(command_size);

    if (!input_command) {
        perror("malloc");

        free(quoted_executable);

        return 1;
    }

    snprintf(input_command, command_size, "exec %s --input", quoted_executable);

    free(quoted_executable);

    /*
     * Left pane:
     * watch the chat file
     */
    char *new_session[] = {"tmux",       "new-session",           "-d", "-s",
                           CHAT_SESSION, "tail -f -- " CHAT_FILE, NULL};

    if (run_command(new_session, 0) != 0) {
        fprintf(stderr, "welp, couldn't start tmux session '%s'\n", CHAT_SESSION);

        free(input_command);

        return 1;
    }

    /*
     * Right pane:
     * run ourselves in input mode
     */
    char *split_window[] = {"tmux", "split-window", "-h", "-t", CHAT_SESSION, input_command, NULL};

    if (run_command(split_window, 0) != 0) {
        fprintf(stderr, "tmux wouldn't make the input pane :(\n");

        free(input_command);

        return 1;
    }

    free(input_command);

    /*
     * Give the chat history a little more room.
     * Not important enough to explode if this fails.
     */
    char *resize_pane[] = {"tmux", "resize-pane", "-t", CHAT_SESSION ":0.0", "-x", "70%", NULL};

    (void) run_command(resize_pane, 1);

    attach_to_chat();

    // attach_to_chat() normally never returns
    return 1;
}
