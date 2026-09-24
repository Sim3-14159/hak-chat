#include "wtext.h"
#include <time.h>

struct Message {
    struct WText *content;
    struct tm *timesent;
    // 256 chars is max POSIX filename length (for the most part), not including null terminator
    char username[256];
};
