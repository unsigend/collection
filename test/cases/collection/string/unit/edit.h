#include "common.h"

UTEST_CASE(string_insert)
{
    {
        struct string *str = string_from_buf("hello");

        EXPECT_NOTNULL(str);
        EXPECT_EQ_INT(string_insert(str, 0, NULL), -1);
        string_free(str);
    }

    {
        struct string *str = string_from_buf("lo");

        EXPECT_NOTNULL(str);
        EXPECT_EQ_INT(string_insert(str, 0, "hel"), 0);
        EXPECT_EQ_STR(string_raw(str), "hello");
        string_free(str);
    }

    {
        struct string *str = string_from_buf("heo");

        EXPECT_NOTNULL(str);
        EXPECT_EQ_INT(string_insert(str, 2, "ll"), 0);
        EXPECT_EQ_STR(string_raw(str), "hello");
        string_free(str);
    }

    {
        struct string *str = string_from_buf("hel");

        EXPECT_NOTNULL(str);
        EXPECT_EQ_INT(string_insert(str, 10, "lo"), 0);
        EXPECT_EQ_STR(string_raw(str), "hello");
        string_free(str);
    }

    {
        struct string *str = string_from_buf("hello");

        EXPECT_NOTNULL(str);
        EXPECT_EQ_INT(string_insert(str, 5, "!"), 0);
        EXPECT_EQ_STR(string_raw(str), "hello!");
        string_free(str);
    }

    {
        struct string *str = string_from_buf("abc");

        EXPECT_NOTNULL(str);
        EXPECT_EQ_INT(string_insert(str, 1, ""), 0);
        EXPECT_EQ_STR(string_raw(str), "abc");
        string_free(str);
    }
}

UTEST_CASE(string_erase)
{
    {
        EXPECT_EQ_INT(string_erase(NULL, 0, 1), -1);
    }

    {
        struct string *str = string_from_buf("hello");

        EXPECT_NOTNULL(str);
        EXPECT_EQ_INT(string_erase(str, 10, 1), -1);
        string_free(str);
    }

    {
        struct string *str = string_from_buf("hello");

        EXPECT_NOTNULL(str);
        EXPECT_EQ_INT(string_erase(str, 1, 3), 0);
        EXPECT_EQ_STR(string_raw(str), "ho");
        string_free(str);
    }

    {
        struct string *str = string_from_buf("hello");

        EXPECT_NOTNULL(str);
        EXPECT_EQ_INT(string_erase(str, 3, 100), 0);
        EXPECT_EQ_STR(string_raw(str), "hel");
        string_free(str);
    }

    {
        struct string *str = string_from_buf("hello");

        EXPECT_NOTNULL(str);
        EXPECT_EQ_INT(string_erase(str, 0, 5), 0);
        EXPECT_TRUE(string_empty(str));
        string_free(str);
    }

    {
        struct string *str = string_from_buf("hello");

        EXPECT_NOTNULL(str);
        EXPECT_EQ_INT(string_erase(str, 5, 0), 0);
        EXPECT_EQ_STR(string_raw(str), "hello");
        string_free(str);
    }

    {
        struct string *str = string_from_buf("hello");

        EXPECT_NOTNULL(str);
        EXPECT_EQ_INT(string_erase(str, 0, 0), 0);
        EXPECT_EQ_STR(string_raw(str), "hello");
        string_free(str);
    }
}
