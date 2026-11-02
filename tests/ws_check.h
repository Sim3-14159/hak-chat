#ifndef SS_CHECK_H
#define SS_CHECK_H

/* A tiny test harness, shared by ss_test.c and my_tests.c.

   CHECK(condition, L"description") prints PASS or FAIL with the description, and counts
   it. check_group("name") prints a heading. check_summary() prints the totals and
   returns the program's exit status: 0 only when every check passed.

   Include this from one .c file per program. Each program gets its own counters. */

#include <stdbool.h>
#include <stdio.h>

static int check_passed = 0;
static int check_failed = 0;

static inline void check_report(bool ok, const char *description, const char *file, int line)
{
    if (ok) {
        check_passed++;
        printf("\033[32mPASS\033[0m  %s\n", description);
    } else {
        check_failed++;
        printf("\033[31mFAIL\033[0m  %s (%s:%d)\n", description, file, line);
    }
    fflush(stdout);
}

#define CHECK(condition, description)                                                              \
    check_report((condition) ? true : false, (description), __FILE__, __LINE__)

static inline void check_group(const char *name)
{
    printf("\n%s\n", name);
}

static inline int check_summary(void)
{
    printf("\n%d of %d tests passed\n", check_passed, check_passed + check_failed);
    fflush(stdout);
    return check_failed == 0 ? 0 : 1;
}

/* true if a and b hold the same characters. A NULL equals only another NULL.
   This is the harness's own strcmp, so the tests need no <string.h> either. */
static inline bool check_same_cstr(const wchar_t *a, const wchar_t *b)
{
    size_t i = 0;
    if (a == NULL || b == NULL) {
        return a == b;
    }
    while (a[i] != '\0' && a[i] == b[i]) {
        i++;
    }
    return a[i] == b[i];
}

#endif
