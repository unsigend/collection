#include "common.h"

UTEST_CASE(string_find)
{
    {
        struct string *str = string_from_buf("hello world");

        EXPECT_NULL(string_find(NULL, "world"));
        EXPECT_NOTNULL(str);
        EXPECT_NULL(string_find(str, "xyz"));
        string_free(str);
    }

    {
        struct string *str = string_from_buf("hello world");
        const char *match;

        EXPECT_NOTNULL(str);
        match = string_find(str, "world");
        EXPECT_NOTNULL(match);
        EXPECT_EQ_PTR(match, string_raw(str) + 6);
        EXPECT_EQ_STR(match, "world");
        string_free(str);
    }

    {
        struct string *str = string_from_buf("abcabc");
        const char *first;

        EXPECT_NOTNULL(str);
        first = string_find(str, "abc");
        EXPECT_NOTNULL(first);
        EXPECT_EQ_PTR(first, string_raw(str));
        string_free(str);
    }

    {
        struct string *str = string_from_buf("");
        const char *match;

        EXPECT_NOTNULL(str);
        EXPECT_NULL(string_find(str, "a"));
        match = string_find(str, "");
        EXPECT_NULL(match);
        string_free(str);
    }

    {
        struct string *str = string_from_buf("hello");

        EXPECT_NOTNULL(str);
        EXPECT_EQ_PTR(string_find(str, "hello"), string_raw(str));
        string_free(str);
    }
}

UTEST_CASE(string_find_last)
{
    {
        struct string *str = string_from_buf("hello world");

        EXPECT_NULL(string_find_last(NULL, "world"));
        EXPECT_NOTNULL(str);
        EXPECT_NULL(string_find_last(str, "xyz"));
        string_free(str);
    }

    {
        struct string *str = string_from_buf("abcabc");
        const char *last;

        EXPECT_NOTNULL(str);
        last = string_find_last(str, "abc");
        EXPECT_NOTNULL(last);
        EXPECT_EQ_PTR(last, string_raw(str) + 3);
        string_free(str);
    }

    {
        struct string *str = string_from_buf("hello world");
        const char *last;

        EXPECT_NOTNULL(str);
        last = string_find_last(str, "o");
        EXPECT_NOTNULL(last);
        EXPECT_EQ_CHAR(*last, 'o');
        string_free(str);
    }

    {
        struct string *str = string_from_buf("aaa");
        const char *last;

        EXPECT_NOTNULL(str);
        last = string_find_last(str, "a");
        EXPECT_NOTNULL(last);
        EXPECT_EQ_PTR(last, string_raw(str) + 2);
        EXPECT_NULL(string_find_last(str, ""));
        string_free(str);
    }
}

UTEST_CASE(string_contains)
{
    EXPECT_FALSE(string_contains(NULL, "a"));

    {
        struct string *str = string_from_buf("hello world");

        EXPECT_NOTNULL(str);
        EXPECT_TRUE(string_contains(str, "world"));
        EXPECT_TRUE(string_contains(str, "hello"));
        EXPECT_FALSE(string_contains(str, "xyz"));
        string_free(str);
    }

    {
        struct string *str = string_from_buf("hello");

        EXPECT_NOTNULL(str);
        EXPECT_FALSE(string_contains(str, ""));
        EXPECT_FALSE(string_contains(str, "hello world"));
        string_free(str);
    }
}

UTEST_CASE(string_starts_with)
{
    EXPECT_FALSE(string_starts_with(NULL, "a"));

    {
        struct string *str = string_from_buf("hello world");

        EXPECT_NOTNULL(str);
        EXPECT_TRUE(string_starts_with(str, "hello"));
        EXPECT_TRUE(string_starts_with(str, ""));
        EXPECT_FALSE(string_starts_with(str, "world"));
        string_free(str);
    }

    {
        struct string *str = string_from_buf("hi");

        EXPECT_NOTNULL(str);
        EXPECT_FALSE(string_starts_with(str, "hello"));
        EXPECT_FALSE(string_starts_with(str, NULL));
        string_free(str);
    }
}

UTEST_CASE(string_ends_with)
{
    EXPECT_FALSE(string_ends_with(NULL, "a"));

    {
        struct string *str = string_from_buf("hello world");

        EXPECT_NOTNULL(str);
        EXPECT_TRUE(string_ends_with(str, "world"));
        EXPECT_TRUE(string_ends_with(str, ""));
        EXPECT_FALSE(string_ends_with(str, "hello"));
        string_free(str);
    }

    {
        struct string *str = string_from_buf("hi");

        EXPECT_NOTNULL(str);
        EXPECT_FALSE(string_ends_with(str, "hello"));
        EXPECT_FALSE(string_ends_with(str, NULL));
        string_free(str);
    }
}
