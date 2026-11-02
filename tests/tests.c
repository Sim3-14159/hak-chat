/* my_tests.c - your tests for the functions you add to safe_string.h.

   `make test` builds and runs this after ss_test. Write one test_ function per function
   you add, in the style of ss_test.c, and call each one from main. CHECK, check_group and
   check_same_cstr come from ss_check.h. Free every String a test builds, so `make asan`
   reports only real leaks. */

#include <stdio.h>

#include "safe_string.h"
#include "ss_check.h"

/* The number of characters before text's terminator. */
static size_t cstr_length(const wchar_t *text)
{
    size_t n = 0;
    while (text[n] != L'\0') {
        n++;
    }
    return n;
}

/* true if s is a String whose length and bytes are exactly expected's. */
static bool reads(const String *s, const wchar_t *expected)
{
    return s != NULL && ss_length(s) == cstr_length(expected) &&
           check_same_cstr(ss_cstr(s), expected);
}

/* true if s is a String whose C string ends at its length, with nothing cut short. */
// Unused
//static bool terminated(const String *s)
//{
//    return s != NULL && cstr_length(ss_cstr(s)) == ss_length(s);
//}

// TESTS

static void test_ss_to_lower()
{
    String *dune = ss_new(L"Dune");
    String *buFfER = ss_new(L"buFfER");
    String *SOMEthing = ss_new(L"SOMEthing!!!!");
    String *none = ss_new(L"");
    String *single = ss_new(L"l");

    String *lower_dune = ss_to_lower(dune);
    String *lower_buFfER = ss_to_lower(buFfER);
    String *lower_SOMEthing = ss_to_lower(SOMEthing);
    String *lower_none = ss_to_lower(none);
    String *lower_single = ss_to_lower(single);

    check_group("ss_to_lower");
    CHECK(ss_to_lower(NULL) == NULL, "ss_to_lower(NULL) returns NULL");
    CHECK(reads(lower_dune, L"dune"), "ss_to_lower(\"Dune\") is \"dune\"");
    CHECK(reads(lower_single, L"l"), "ss_to_lower(\"l\") is \"l\"");
    CHECK(reads(lower_none, L""), "ss_to_lower(\"\") is \"\"");
    CHECK(reads(lower_buFfER, L"buffer"), "ss_to_lower(\"buFfER\") is \"buffer\"");
    CHECK(reads(lower_SOMEthing, L"something!!!!"),
          "ss_to_lower(\"SOMEthing!!!!\") is \"something!!!!\"");

    ss_free(dune);
    ss_free(single);
    ss_free(buFfER);
    ss_free(SOMEthing);
    ss_free(none);

    ss_free(lower_dune);
    ss_free(lower_single);
    ss_free(lower_buFfER);
    ss_free(lower_SOMEthing);
    ss_free(lower_none);
}

//static void test_ss_trim()
//{
//    String *dune = ss_new(L"Dune");
//    String *multiline = ss_new(L"\t\nthis has weird \nchars");
//    String *SOMEthing = ss_new(L"SOMEthing!!!! \t ");
//    String *pad = ss_new(L" pad ");
//    String *spaces = ss_new(L"   ");
//    String *none = ss_new(L"");
//    String *single = ss_new(L"l");
//
//    check_group("ss_trim");
//
//    CHECK(reads(ss_trim(dune), L"Dune"), "ss_trim(\"Dune\") is \"Dune\"");
//    CHECK(reads(ss_trim(pad), L"pad"), "ss_trim(\" pad \") is \"pad\"");
//    CHECK(ss_trim(NULL) == NULL, "ss_trim(NULL) is NULL");
//    CHECK(reads(ss_trim(spaces), L""), "ss_trim(\"   \") is \"\"");
//    CHECK(reads(ss_trim(single), L"l"), "ss_trim(\"l\") is \"l\"");
//    CHECK(reads(ss_trim(none), L""), "ss_trim(\"\") is \"\"");
//    CHECK(reads(ss_trim(multiline), "\t\nthis has weird \nchars"),
//          "ss_trim(\"\\t\\nthis has weird \\nchars\") is \"this has weird \\nchars\"");
//    CHECK(reads(ss_trim(SOMEthing), "SOMEthing!!!!"),
//          "ss_trim(\"SOMEthing!!!! \\t \") is \"SOMEthing!!!!\"");
//
//    ss_free(dune);
//    ss_free(pad);
//    ss_free(multiline);
//    ss_free(spaces);
//    ss_free(single);
//    ss_free(SOMEthing);
//    ss_free(none);
//}

static void test_ss_compare_ignore_case()
{
    //String *one = ss_new(L"one");
    //String *upper = ss_new(L"UPPER");
    //String *lower = ss_new(L"lower");
    //String *none = ss_new(L"");

    //check_group("ss_compare_ignore_case");

    //CHECK(ss_compare_ignore_case(one, one) == 0, "ss_compare_ignore_case(\"one\") is \"one\"");
    //CHECK(ss_compare_ignore_case(one, one) == 0, "ss_compare_ignore_case(\"one\") is \"one\"");

    //ss_free(one);
    //ss_free(none);
    //ss_free(upper);
    //ss_free(lower);

    String *dune = ss_new(L"Dune");
    String *dune2 = ss_new(L"Dune");
    String *dun = ss_new(L"Dun");
    String *dunk = ss_new(L"Dunk");
    String *a = ss_new(L"a");
    String *b = ss_new(L"b");
    String *aa = ss_new(L"aa");
    String *AA = ss_new(L"AA");
    String *empty = ss_new(L"");
    String *high = ss_new(L"\xe9"); /* a byte above 127, as in the UTF-8 for an accent */

    check_group("ss_compare_ignore_case");
    CHECK(ss_compare_ignore_case(dune, dune2) == 0, "\"Dune\" compares equal to \"Dune\"");
    CHECK(ss_compare_ignore_case(AA, aa) == 0, "\"AA\" compares equal to \"aa\"");
    CHECK(ss_compare_ignore_case(a, b) < 0, "\"a\" sorts before \"b\"");
    CHECK(ss_compare_ignore_case(b, a) > 0, "\"b\" sorts after \"a\"");
    CHECK(ss_compare_ignore_case(dun, dune) < 0,
          "\"Dun\" sorts before \"Dune\" (a prefix comes first)");
    CHECK(ss_compare_ignore_case(dune, dun) > 0, "\"Dune\" sorts after \"Dun\"");
    CHECK(ss_compare_ignore_case(dune, dunk) < 0,
          "\"Dune\" sorts before \"Dunk\" (the last byte decides)");
    CHECK(ss_compare_ignore_case(dunk, dune) > 0, "\"Dunk\" sorts after \"Dune\"");
    CHECK(ss_compare_ignore_case(b, aa) > 0,
          "\"b\" sorts after the longer \"aa\" (bytes decide before length does)");
    CHECK(ss_compare_ignore_case(empty, a) < 0, "\"\" sorts before \"a\"");
    CHECK(ss_compare_ignore_case(high, a) > 0,
          "a byte above 127 sorts after 'a' (bytes compare as unsigned char)");
    CHECK(ss_compare_ignore_case(NULL, empty) < 0, "NULL sorts before \"\"");
    CHECK(ss_compare_ignore_case(empty, NULL) > 0, "\"\" sorts after NULL");
    CHECK(ss_compare_ignore_case(NULL, NULL) == 0, "NULL compares equal to NULL");

    ss_free(dune);
    ss_free(dune2);
    ss_free(dun);
    ss_free(dunk);
    ss_free(a);
    ss_free(b);
    ss_free(aa);
    ss_free(AA);
    ss_free(empty);
    ss_free(high);
}

static void test_ss_reverse()
{
    String *aa = ss_new(L"aa");
    String *string = ss_new(L"string");
    String *racecar = ss_new(L"Racecar");
    String *empty = ss_new(L"");

    check_group("test_ss_reverse");

    String *result_aa = ss_reverse(aa);
    String *result_string = ss_reverse(string);
    String *result_racecar = ss_reverse(racecar);
    String *result_empty = ss_reverse(empty);

    CHECK(ss_reverse(NULL) == NULL, "ss_reverse(NULL) is NULL");
    CHECK(reads(result_aa, L"aa"), "ss_reverse(\"aa\") is the same as \"aa\"");
    CHECK(reads(result_string, L"gnirts"), "ss_reverse(\"string\") is \"gnirts\"");
    CHECK(reads(result_racecar, L"racecaR"), "ss_reverse(\"Racecar\") is \"racecaR\"");
    CHECK(reads(result_empty, L""), "ss_reverse(\"\") is \"\"");

    ss_free(aa);
    ss_free(string);
    ss_free(racecar);
    ss_free(empty);
    ss_free(result_aa);
    ss_free(result_string);
    ss_free(result_racecar);
    ss_free(result_empty);
}

int main(void)
{
    test_ss_to_lower();
    //test_ss_trim();
    test_ss_reverse();
    test_ss_compare_ignore_case();
    return check_summary();
}
