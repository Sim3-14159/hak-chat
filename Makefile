CC := gcc
CFLAGS := -std=c11 -Wall -Wextra -Wpedantic -O2
LDFLAGS := -lncursesw

TARGET := build/hak-chat
SOURCE := src/hak-chat.c

TEST_SOURCE := $(wildcard tests/*.c)
TEST_TARGET := $(TEST_SRC:tests/%.c=build/test_%)

.PHONY: all clean test
.DEFAULT_GOAL: all

all: $(TARGET)

$(TARGET): $(SOURCE) | build
	$(CC) $(CFLAGS) $(SOURCE) -o $(TARGET) $(LDFLAGS)

test: $(TEST_SRC:tests/%.c=build/%) | build/tests

build/%: tests/%.c | build
	@echo $@ : @<
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ $(LDFLAGS) -Isrc

build:
	mkdir -p $@

build/tests:
	mkdir -p $@

clean:
	rm -rf build/
