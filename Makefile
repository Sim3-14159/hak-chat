CC := gcc
CFLAGS := -std=c11 -Wall -Wextra -Wpedantic -O2
LDFLAGS := -lncursesw

TARGET := hak-chat
SOURCE := hacker_chatter.c

.PHONY: all clean test-ncurses

all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) $(SOURCE) -o $(TARGET) $(LDFLAGS)

clean:
	$(RM) $(TARGET)
