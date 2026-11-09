/* dynamic_array.c - your implementation of dynamic_array.h.

   The one rule: do not call realloc. Allocate the bigger block, copy the data across and
   free the old block yourself. The rest of the standard C library is allowed.

   Every function below is a stub. It compiles and returns something harmless, so
   `make m1` runs from the start. Replace each stub's body with your own code. */

#include <stdlib.h>

#include "dynamic_array.h"

/* Define struct DynamicArray here. */
// defined in dynamic_array.h because then I can interact with dynamic arrays in files #included

#define INITIAL_SIZE 8
#define X(p)                                                                                       \
    if (!p)                                                                                        \
        return NULL;
#define MIN(x, y) (x > y ? y : x)

DynamicArray *da_new(void)
{
    da new = malloc(sizeof(DynamicArray));
    X(new);

    new->count = 0;
    new->data = malloc((sizeof(int) * INITIAL_SIZE));
    new->capacity = INITIAL_SIZE;
    if (!new->data) {
        free(new->data);
        free(new);
        return NULL;
    }
    return new;
}

void da_free(da a)
{
    if (!a)
        return;
    free(a->data);
    free(a);
}

size_t da_count(const DynamicArray *a)
{
    if (!a)
        return 0;
    return a->count;
}

bool da_reserve(DynamicArray *a, size_t n)
{
    if (!a)
        return false;
    if (n <= a->capacity)
        return true;
    int *newmem = malloc(sizeof(int) * n);
    if (!newmem)
        return false;

    for (size_t i = 0; i < a->count; i++)
        newmem[i] = a->data[i];
    free(a->data);
    a->data = newmem;
    a->capacity = n;
    return true;
}

bool da_add(DynamicArray *a, int value)
{
    if (!a)
        return false;

    if (a->count >= a->capacity)
        if (!da_reserve(a, a->capacity * 2))
            return false;

    a->data[a->count] = value;
    a->count++;
    return true;
}

bool da_insert(DynamicArray *a, size_t i, int value)
{
    if (!a || i > a->count)
        return false;

    if (a->count >= a->capacity)
        if (!da_reserve(a, a->capacity * 2)) // double size
            return false;

    for (size_t j = a->count; j > i; j--) {
        a->data[j] = a->data[j - 1];
    }
    a->data[i] = value;
    a->count++;
    return true;
}

bool da_get(const DynamicArray *a, size_t i, int *out)
{
    if (!a || !out || i >= a->count)
        return false;
    *out = a->data[i];
    return true;
}

bool da_set(DynamicArray *a, size_t i, int value)
{
    if (!a)
        return false;
    if (i >= a->count) // || i < 0
        return false;
    a->data[i] = value;
    return true;
}

bool da_copy_to(const DynamicArray *a, int *dest, size_t dest_len)
{
    if (!a || !dest || a->count > dest_len)
        return false;

    for (size_t i = 0; i < MIN(dest_len, a->count); i++)
        dest[i] = a->data[i];
    return true;
}

bool da_index_of(const DynamicArray *a, int value, size_t *index)
{
    if (!a || !index)
        return false;

    for (size_t i = 0; i < a->count; i++) {
        if (a->data[i] == value) {
            *index = i;
            return true;
        }
    }
    return false;
}

bool da_contains(const DynamicArray *a, int value)
{
    if (!a)
        return false;
    for (size_t i = 0; i < a->count; i++)
        if (a->data[i] == value)
            return true;
    return false;
}

bool da_remove_at(DynamicArray *a, size_t i, int *out)
{
    if (!a || i >= a->count)
        return false;
    if (out)
        *out = a->data[i];

    for (size_t j = i; j < a->count - 1; j++) {
        a->data[j] = a->data[j + 1];
    }
    a->count--;
    return true;
}

bool da_remove(DynamicArray *a, int value)
{
    if (!a)
        return false;
    for (size_t i = 0; i < a->count; i++) {
        if (a->data[i] == value) {
            for (size_t j = i; j < a->count - 1; j++)
                a->data[j] = a->data[j + 1];
            a->count--;
            return true;
        }
    }
    return false;
}

void da_clear(DynamicArray *a)
{
    if (!a)
        return;
    a->count = 0;
}

/*** CUSTOM ONES (3 small) ****/

int da_pop(da a)
{
    if (!a)
        return 0;
    int value = 0;
    da_remove_at(a, a->count - 1, &value);

    return value;
}

//bool da_reverse(da a)
//{
//    if (!a || !a->data)
//        return false;
//    int *new = malloc(a->capacity * sizeof(int));
//    if (!new)
//        return false;
//
//    for (size_t i = 0; i < a->count; i++) {
//        new[i] = a->data[a->count - i - 1];
//    }
//    free(a->data);
//    a->data = new;
//    return true;
//}

bool da_reverse(da a)
{
    if (!a || !a->data)
        return false;
    for (size_t i = 0; i < a->count / 2; i++) {
        int first = a->data[i];
        int second = a->data[a->count - i - 1];
        a->data[i] = second;
        a->data[a->count - i - 1] = first;
    }
    return true;
}

bool da_add_all(da a, int *values, size_t n)
{
    if (!a || !values || !da_reserve(a, a->count + n))
        return false;
    for (size_t i = 0; i < n; i++)
        da_add(a, values[i]);
    return true;
}
