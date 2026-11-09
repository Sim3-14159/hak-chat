/* my_tests.c - your own tests for your array.

   Every `make m1` .. `make m4` and `make test` builds and runs this after da_test, and
   it must not crash. Write each test as printf lines that show what you got and what you
   expected, and read them to decide whether your array is right. alloc_stats.h counts
   allocations, and can make one fail on purpose. Free every array a test builds, so
   `make asan` reports only real leaks. */

#include <stdio.h>

#include "alloc_stats.h"
#include "da_check.h"
#include "dynamic_array.h"

bool arrayeq(int *a1, int *a2, size_t len)
{
    for (size_t i = 0; i < len; i++)
        if (a1[i] != a2[i])
            return false;
    return true;
}

static void test_da_get()
{
    da a = da_new();
    da b = da_new();

    check_section("da_get", "m2 da_get");

    int val = 0;
    CHECK(da_get(a, 0, &val) == false, "da_get at an index that isn't initialized returns false");
    CHECK(val == 0, "da_get at an index that isn't initialized doesn't change out");

    int val2 = 0;
    da_add(b, 1);
    CHECK(da_get(b, 0, &val2) == true, "da_get on an initialized value returns true");
    CHECK(val2 == 1, "da_get on an initialized value sets out to the value");

    da_free(a);
    da_free(b);
}

static void test_da_reserve()
{
    da a = da_new();
    size_t old_capacity_a = a->capacity;

    da b = da_new();
    size_t old_capacity_b = b->capacity;

    da c = da_new();
    size_t old_count_c = c->count;

    da d = da_new();
    int old_data_d[] = {1, 2, 3};
    for (size_t i = 0; i < 3; i++)
        da_add(d, old_data_d[i]);

    check_section("da_reserve", "m2 da_reserve");

    CHECK(da_reserve(a, a->capacity * 2) && a->capacity == 2 * old_capacity_a,
          "Capacity doubles when da_reserve is called with a->capacity * 2.");
    CHECK(da_reserve(NULL, 0) == false, "da_reserve-ing on NULL is false");
    CHECK(da_reserve(b, b->capacity) && b->capacity == old_capacity_b,
          "da_reserve-ing a da's capacity does not change a da's capacity");
    CHECK(da_reserve(c, c->capacity) && c->count == old_count_c,
          "da_reserve-ing a da's capacity does not change a da's count");
    CHECK(da_reserve(d, d->capacity) && arrayeq(d->data, old_data_d, d->count),
          "da_reserve-ing a da's capacity does not change a da's contents");

    da_free(a);
    da_free(b);
    da_free(c);
    da_free(d);
}

static void test_da_set()
{
    da a = da_new();

    da b = da_new();

    da c = da_new();

    da d = da_new();
    //size_t old_count_d = d->count;
    //int old_data_d[a->count != 0 ? a->count : 1]; // c does not allow arrays of length 0
    //for (size_t i = 0; i < a->count; i++)
    //    old_data_d[i] = a->data[i];
    int old_data_d[] = {1, 2, 3};
    for (size_t i = 0; i < 3; i++)
        da_add(d, old_data_d[i]);
    size_t old_count_d = d->count;

    da e = da_new();

    check_section("da_set", "m2 da_set");

    CHECK(da_set(a, 0, 1) == false,
          "Setting an uninitialized value (index 0) of a new da returns false");
    da_add(b, 1);
    CHECK(da_set(b, 0, 1) == true, "Setting an initialized value (index 0) of a da returns true");
    CHECK(da_set(NULL, 0, 1) == false, "Setting a value on NULL returns false");
    CHECK(da_set(c, -1, 1) == false, "Setting a negative index returns false.");
    da_set(d, -1, 1);
    CHECK(arrayeq(d->data, old_data_d, old_count_d),
          "Setting a negative index does not change the array's data.");
    da_set(d, -100, 1);
    CHECK(true, "Setting a large negative index does not segfault.");
    da_set(d, -1000, 1);
    CHECK(true, "Setting a larger negative index does not segfault.");
    da_set(d, -10000, 1);
    CHECK(true, "Setting a very large negative index does not segfault.");

    da_free(a);
    da_free(b);
    da_free(c);
    da_free(d);
    da_free(e);
}

static void test_da_insert()
{
    da a = da_new();

    da b = da_new();

    da c = da_new();

    da d = da_new();
    size_t old_count_d = d->count;

    check_section("da_insert", "m3 da_insert");

    CHECK(da_insert(a, 5, 1) == false,
          "inserting at an index greater than the da's `count` returns false");
    CHECK(da_insert(b, 0, 1) == true,
          "inserting at an index equal to the da's `count` returns true");
    da_add(c, 2);
    CHECK(da_insert(c, 0, 1) && c->data[0] == 1 && c->data[1] == 2,
          "da_insert inserts numbers AT the posision specified, in order");
    CHECK(da_insert(d, 0, 1) && d->count == old_count_d + 1, "da_insert increments the count");

    da_free(a);
    da_free(b);
    da_free(c);
    da_free(d);
}

static void test_da_clear()
{
    da a = da_new();
    da_add(a, 6);
    da_add(a, 7);
    da_add(a, 8);
    da_add(a, 9);

    check_section("da_clear", "m3 da_clear");
    da_clear(a);
    CHECK(a->count == 0, "da_clear clears a da even if it has stuff in it");

    da_free(a);
}

static void test_da_copy_to()
{
    da a = da_new();
    da b = da_new();
    da_add(a, 6);
    da_add(a, 7);
    da_add(a, 8);
    da_add(a, 9);

    da_add(b, 6);
    da_add(b, 7);
    da_add(b, 8);
    da_add(b, 9);

    int a_data[a->count];
    int b_data[b->count];
    size_t old_b_count = b->count;
    size_t old_b_capacity = b->capacity;

    int old_b_data[b->count != 0 ? b->count : 1]; // c does not allow arrays of length 0
    for (size_t i = 0; i < a->count; i++)
        old_b_data[i] = b->data[i];

    check_section("da_copy_to", "m3 da_copy_to");

    da_copy_to(a, a_data, a->count);
    da_copy_to(b, b_data, b->count);

    CHECK(arrayeq(a->data, a_data, a->count), "da_copy_to copies a da's ints into dest");
    CHECK(b->count == old_b_count && b->capacity == old_b_capacity &&
              arrayeq(b->data, old_b_data, b->count),
          "da_copy_to does not modify the original da");

    da_free(a);
    da_free(b);
}

static void test_da_index_of()
{
    da a = da_new();
    da b = da_new();
    da_add(a, 6);
    da_add(a, 7);
    da_add(a, 8);
    da_add(a, 9);

    da_add(b, 6);
    da_add(b, 7);
    da_add(b, 8);
    da_add(b, 9);

    size_t indexA;
    size_t indexB;

    check_section("da_index_of", "m3 da_index_of");

    CHECK(da_index_of(a, 6, &indexA) && indexA == 0, "da_index_of gets correct index");
    CHECK(da_index_of(b, 8, &indexB) && indexB == 2, "da_index_of gets correct index");

    da_free(a);
    da_free(b);
}

static void test_da_contains()
{
    da a = da_new();
    da b = da_new();
    da_add(a, 6);
    da_add(a, 7);
    da_add(a, 8);
    da_add(a, 9);

    da_add(b, 6);
    da_add(b, 7);
    da_add(b, 8);
    da_add(b, 9);

    check_section("da_contains", "m3 da_contains");

    CHECK(da_contains(a, 9), "da_contains actually tells you if a da contains a value");
    CHECK(!da_contains(b, 90), "da_contains tells you if a da doesn't contain a value");

    da_free(a);
    da_free(b);
}

static void test_da_remove()
{
    da a = da_new();
    da b = da_new();
    da c = da_new();
    da_add(a, 6);
    da_add(a, 7);
    da_add(a, 8);
    da_add(a, 9);

    da_add(b, 9);
    da_add(b, 9);

    check_section("da_remove", "m3 da_remove");

    CHECK(da_remove(a, 9) == true && !da_contains(a, 9),
          "removing a value that appears in a da 1 time returns true, "
          "and the value is no longer in the da");
    CHECK(da_remove(b, 9) == true && da_contains(b, 9),
          "removing a value that appears 2x in a da returns true, but the value still appears in "
          "the da");
    CHECK(da_remove(c, 9) == false, "removing a value that doesn't exist returns false");

    da_free(a);
    da_free(b);
    da_free(c);
}

static void test_da_free()
{
    da a = da_new();
    da b = da_new();
    da c = da_new();
    da_add(b, 1);
    da_add(b, 1);
    da_add(b, 1);
    da_add(b, 1);
    da_add(b, 1);
    da_add(b, 1);
    da_add(b, 1);
    da_add(b, 1);

    da_add(c, 1);
    da_reserve(c, 1000);
    da_add(c, 1);

    check_section("da_free", "m1 da_free");
    CHECK(true, "This function should return fine without a segfault and in that case things are "
                "probably fine");

    da_free(a);
    da_free(b);
    da_free(c);
}

static void test_da_count()
{
    da a = da_new();
    da b = da_new();
    da c = da_new();
    da d = da_new();
    da_add(a, 6);
    da_add(a, 7);
    da_add(a, 8);
    da_add(a, 9);

    da_add(b, 9);
    da_add(b, 9);
    da_reserve(b, 10);

    da_add(d, 6);
    da_set(d, 0, 7);

    check_section("da_count", "m1 da_count");

    CHECK(da_count(a) == 4, "da_count counts accurately for a regular array");
    CHECK(da_count(b) == 2,
          "da_count returns the da's count on a da that has had da_reserve called on it");
    CHECK(da_count(c) == 0, "da_count returns 0 on an empty da");
    CHECK(da_count(d) == 1,
          "da_count returns a da's count on a da that has had da_set called on it");

    da_free(a);
    da_free(b);
    da_free(c);
    da_free(d);
}

static void test_da_new()
{
    da a = da_new();
    check_section("da_new", "m1 da_new");

    CHECK(a != NULL, "da_new returns a non null memory address");
    CHECK(a->count == 0, "da_new starts with a count of 0");
    CHECK(a->data != NULL, "da_new's data is not NULL");

    da_free(a);
}

static void test_da_add()
{
    da a = da_new();
    da_add(a, 1);

    check_section("da_add", "m1 da_add");

    CHECK(a->count == 1, "adding increments the count");
    CHECK(a->data[0] == 1, "adding actually puts the elemnt in the data");
    CHECK(da_add(NULL, 0) == 0, "da_add on NULL returns 0, and doesn't crash");

    da_free(a);
}

/*** CUSTOM ONES ***/

static void test_da_pop()
{
    da a = da_new();
    da_add(a, 0);
    da_add(a, 1);
    da_add(a, 2);
    da_add(a, 3);

    da empty = da_new();

    check_section("da_pop", "m4 da_pop");

    CHECK(da_pop(a) == 3, "da_pop returns the last item");
    CHECK(arrayeq(a->data, (int[]){0, 1, 2}, 3),
          "da_pop removes the last item from the array's contents");
    CHECK(a->count == 3, "da_pop decrements the count");
    CHECK(da_pop(NULL) == 0, "da_pop returns 0 with NULL argument");
    CHECK(da_pop(empty) == 0 && empty->count == 0,
          "da_pop returns 0 with an empty da, and doesn't change the count");

    da_free(a);
    da_free(empty);
}

static void test_da_reverse()
{
    da a = da_new();
    da_add(a, 0);
    da_add(a, 1);
    da_add(a, 2);
    da_add(a, 3);

    da b = da_new();
    da_add(b, 1);
    da_add(b, 2);
    da_add(b, 3);
    da_add(b, 4);
    da_add(b, 5);

    da c = da_new();
    da_add(c, 1);
    da_add(c, 2);
    da_add(c, 3);
    da_add(c, 4);
    da_add(c, 5);
    da_add(c, 6);
    da_add(c, 7);
    da_add(c, 8);

    da d = da_new();
    da_add(d, 7);

    da empty = da_new();

    check_section("da_reverse", "m4 da_reverse");

    da_reverse(a);
    CHECK(arrayeq(a->data, (int[]){3, 2, 1, 0}, 4), "da_reverse reverses a short array");
    CHECK(da_reverse(NULL) == false, "da_reverse returns false with NULL argument");

    CHECK(da_reverse(b) && arrayeq(b->data, (int[]){5, 4, 3, 2, 1}, 5),
          "reversed odd holds 1 2 3 4 5 (expect 5 4 3 2 1)");
    CHECK(da_reverse(c) && arrayeq(c->data, (int[]){8, 7, 6, 5, 4, 3, 2, 1}, 8),
          "reversed full holds 1 2 3 4 5 6 7 8 (expect 8 7 6 5 4 3 2 1)");
    CHECK(da_reverse(d) && d->data[0] == 7, "reversed one holds 7 (expect 7)");
    CHECK(da_reverse(empty) && empty->count == 0, "reversed empty: count 0 (expect 0)");

    da_free(a);
    da_free(b);
    da_free(c);
    da_free(d);
    da_free(empty);
}

static void test_da_add_all()
{
    da a = da_new();
    da b = da_new();
    size_t old_count = b->count;
    check_section("da_add_all", "m4 da_add_all");

    CHECK(da_add_all(a, (int[]){0, 1, 2, 3}, 4) && arrayeq(a->data, (int[]){0, 1, 2, 3}, 4),
          "da_add_all adds fine on an emty array");
    CHECK(da_add_all(NULL, NULL, 100) == false, "da_add_all returns false with NULL a and values");
    CHECK(da_add_all(b, NULL, 100) == false, "da_add_all returns false with NULL values");
    CHECK(da_add_all(b, (int[]){1}, 0) && b->count == old_count,
          "da_add_al with lenght 0 keeps the count the same");

    da_free(a);
    da_free(b);
}

int main(void)
{
    test_da_get();
    test_da_add();
    test_da_new();
    test_da_free();
    test_da_count();
    test_da_set();
    test_da_reserve();
    test_da_clear();
    test_da_copy_to();
    test_da_index_of();
    test_da_contains();
    test_da_insert();
    test_da_remove();

    test_da_pop();
    test_da_reverse();
    test_da_add_all();
    return check_summary();
}
