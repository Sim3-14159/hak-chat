CC := gcc
CFLAGS := -std=c11 -Wall -Wextra -Wpedantic -O2
LDFLAGS := -lncursesw

TARGET := build/hak-chat
SOURCE := src/hak-chat.c

TEST_SOURCE := $(wildcard tests/*_test.c)
TEST_TARGET := $(TEST_SOURCE:tests/%.c=build/%)
LIB_SOURCE := $(filter-out $(SOURCE),$(wildcard src/*.c))

.PHONY: all clean test
.DEFAULT_GOAL: all

all: $(TARGET)

$(TARGET): $(SOURCE) $(LIB_SOURCE) | build
	$(CC) $(CPPFLAGS) $(CFLAGS) -Isrc $(SOURCE) $(LIB_SOURCE) -o $(TARGET) $(LDFLAGS)

test: $(TEST_TARGET)
	@set -e; for test in $(TEST_TARGET); do ./$$test; done

valgrind: valgrind_tests valgrind_build

valgrind_tests: $(TEST_TARGET) build/hak-chat
	@set -e; for test in $(TEST_TARGET); do ./$$test; done

valgrind_build:
	valgrind ./build/hak-chat

build/%: tests/%.c $(LIB_SOURCE) | build
	$(CC) $(CPPFLAGS) $(CFLAGS) -Isrc $< $(LIB_SOURCE) -o $@ $(LDFLAGS)

build:
	mkdir -p $@

run: build/hak-chat
	./$^

clean:
	rm -rf build/
