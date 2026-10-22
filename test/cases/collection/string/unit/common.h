#ifndef STRING_TEST_COMMON_H
#define STRING_TEST_COMMON_H

#include <str.h>
#include <string.h>
#include <utest.h>

static struct string *string_from_buf(const char *src)
{
    struct string *str = NULL;

    if (string_init_buf(&str, src) != 0)
        return NULL;
    return str;
}

static struct string *string_from_bufn(const char *src, size_t n)
{
    struct string *str = NULL;

    if (string_init_bufn(&str, src, n) != 0)
        return NULL;
    return str;
}

#endif
