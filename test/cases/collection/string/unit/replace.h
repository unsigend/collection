#include "common.h"

UTEST_CASE(string_replace)
{
    {
        struct string *str = string_from_buf("hello world");

        EXPECT_NOTNULL(str);
        EXPECT_EQ_INT(string_replace(str, NULL, "x"), -1);
        EXPECT_EQ_INT(string_replace(str, "world", NULL), -1);
        string_free(str);
    }

    {
        struct string *str = string_from_buf("hello world");

        EXPECT_NOTNULL(str);
        EXPECT_EQ_INT(string_replace(str, "xyz", "abc"), 0);
        EXPECT_EQ_STR(string_raw(str), "hello world");
        string_free(str);
    }

    {
        struct string *str = string_from_buf("hello world");

        EXPECT_NOTNULL(str);
        EXPECT_EQ_INT(string_replace(str, "world", "there"), 0);
        EXPECT_EQ_STR(string_raw(str), "hello there");
        string_free(str);
    }

    {
        struct string *str = string_from_buf("abcabc");

        EXPECT_NOTNULL(str);
        EXPECT_EQ_INT(string_replace(str, "abc", "x"), 0);
        EXPECT_EQ_STR(string_raw(str), "xabc");
        string_free(str);
    }

    {
        struct string *str = string_from_buf("hello");

        EXPECT_NOTNULL(str);
        EXPECT_EQ_INT(string_replace(str, "hello", ""), 0);
        EXPECT_TRUE(string_empty(str));
        string_free(str);
    }

    {
        struct string *str = string_from_buf("foo");

        EXPECT_NOTNULL(str);
        EXPECT_EQ_INT(string_replace(str, "", "bar"), 0);
        EXPECT_EQ_STR(string_raw(str), "foo");
        string_free(str);
    }

    {
        struct string *str = string_from_buf("x");

        EXPECT_NOTNULL(str);
        EXPECT_EQ_INT(string_replace(str, "x", "longer"), 0);
        EXPECT_EQ_STR(string_raw(str), "longer");
        string_free(str);
    }
}

UTEST_CASE(string_replace_all)
{
    {
        struct string *str = string_from_buf("hello world");

        EXPECT_NOTNULL(str);
        EXPECT_EQ_INT(string_replace_all(str, NULL, "x"), -1);
        EXPECT_EQ_INT(string_replace_all(str, "world", NULL), -1);
        string_free(str);
    }

    {
        struct string *str = string_from_buf("hello world");

        EXPECT_NOTNULL(str);
        EXPECT_EQ_INT(string_replace_all(str, "xyz", "abc"), 0);
        EXPECT_EQ_STR(string_raw(str), "hello world");
        string_free(str);
    }

    {
        struct string *str = string_from_buf("abcabc");

        EXPECT_NOTNULL(str);
        EXPECT_EQ_INT(string_replace_all(str, "abc", "x"), 0);
        EXPECT_EQ_STR(string_raw(str), "xx");
        string_free(str);
    }

    {
        struct string *str = string_from_buf("a.a.a");

        EXPECT_NOTNULL(str);
        EXPECT_EQ_INT(string_replace_all(str, ".", "-"), 0);
        EXPECT_EQ_STR(string_raw(str), "a-a-a");
        string_free(str);
    }

    {
        struct string *str = string_from_buf("aaaa");

        EXPECT_NOTNULL(str);
        EXPECT_EQ_INT(string_replace_all(str, "aa", "b"), 0);
        EXPECT_EQ_STR(string_raw(str), "bb");
        string_free(str);
    }

    {
        struct string *str = string_from_buf("foo");

        EXPECT_NOTNULL(str);
        EXPECT_EQ_INT(string_replace_all(str, "", "x"), 0);
        EXPECT_EQ_STR(string_raw(str), "foo");
        string_free(str);
    }
}
