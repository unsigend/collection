#include "common.h"

UTEST_CASE(string_empty)
{
    EXPECT_TRUE(string_empty(NULL));

    {
        struct string *str = string_from_buf("");

        EXPECT_NOTNULL(str);
        EXPECT_TRUE(string_empty(str));
        string_free(str);
    }

    {
        struct string *str = string_from_buf("hello");

        EXPECT_NOTNULL(str);
        EXPECT_FALSE(string_empty(str));
        string_free(str);
    }

    {
        struct string *str = string_from_bufn("a\0b", 3);

        EXPECT_NOTNULL(str);
        EXPECT_FALSE(string_empty(str));
        string_free(str);
    }
}

UTEST_CASE(string_len)
{
    EXPECT_EQ_UINT(string_len(NULL), 0);

    {
        struct string *str = string_from_buf("");

        EXPECT_NOTNULL(str);
        EXPECT_EQ_UINT(string_len(str), 0);
        string_free(str);
    }

    {
        struct string *str = string_from_buf("hello");

        EXPECT_NOTNULL(str);
        EXPECT_EQ_UINT(string_len(str), 5);
        string_free(str);
    }

    {
        struct string *str = string_from_bufn("ab\0cd", 4);

        EXPECT_NOTNULL(str);
        EXPECT_EQ_UINT(string_len(str), 4);
        EXPECT_EQ_UINT(strlen(string_raw(str)), 2);
        string_free(str);
    }
}

UTEST_CASE(string_capacity)
{
    EXPECT_EQ_UINT(string_capacity(NULL), 0);

    {
        struct string *str = NULL;

        EXPECT_EQ_INT(string_init(&str), 0);
        EXPECT_GE_UINT(string_capacity(str), string_len(str));
        string_free(str);
    }

    {
        struct string *str = string_from_buf("hello");
        size_t cap_before;

        EXPECT_NOTNULL(str);
        cap_before = string_capacity(str);
        EXPECT_GE_UINT(cap_before, string_len(str));
        EXPECT_EQ_INT(string_reserve(str, 64), 0);
        EXPECT_GE_UINT(string_capacity(str), 64);
        string_free(str);
    }

    {
        struct string *str = string_from_buf("hi");

        EXPECT_NOTNULL(str);
        EXPECT_EQ_UINT(string_capacity(str), string_len(str));
        string_free(str);
    }
}

UTEST_CASE(string_raw)
{
    EXPECT_NULL(string_raw(NULL));

    {
        struct string *str = string_from_buf("hello");
        const char *raw;

        EXPECT_NOTNULL(str);
        raw = string_raw(str);
        EXPECT_NOTNULL(raw);
        EXPECT_EQ_STR(raw, "hello");
        EXPECT_EQ_CHAR(raw[5], '\0');
        string_free(str);
    }

    {
        struct string *str = string_from_buf("hello");

        EXPECT_NOTNULL(str);
        EXPECT_EQ_INT(string_pushback(str, '!'), 0);
        EXPECT_EQ_STR(string_raw(str), "hello!");
        string_free(str);
    }
}

UTEST_CASE(string_at)
{
    EXPECT_EQ_CHAR(string_at(NULL, 0), '\0');
    EXPECT_EQ_CHAR(string_at(NULL, 3), '\0');

    {
        struct string *str = string_from_buf("hello");

        EXPECT_NOTNULL(str);
        EXPECT_EQ_CHAR(string_at(str, 0), 'h');
        EXPECT_EQ_CHAR(string_at(str, 2), 'l');
        EXPECT_EQ_CHAR(string_at(str, 4), 'o');
        EXPECT_EQ_CHAR(string_at(str, 5), '\0');
        EXPECT_EQ_CHAR(string_at(str, 99), '\0');
        string_free(str);
    }

    {
        struct string *str = string_from_buf("ab");

        EXPECT_NOTNULL(str);
        EXPECT_EQ_CHAR(string_at(str, 1), 'b');
        EXPECT_EQ_CHAR(string_at(str, 2), '\0');
        string_free(str);
    }
}
