#include "common.h"

UTEST_CASE(string_reserve)
{
    EXPECT_EQ_INT(string_reserve(NULL, 8), -1);

    {
        struct string *str = string_from_buf("hello");
        size_t len_before;

        EXPECT_NOTNULL(str);
        len_before = string_len(str);
        EXPECT_EQ_INT(string_reserve(str, 32), 0);
        EXPECT_EQ_UINT(string_len(str), len_before);
        EXPECT_EQ_STR(string_raw(str), "hello");
        EXPECT_GE_UINT(string_capacity(str), 32);
        string_free(str);
    }

    {
        struct string *str = string_from_buf("hello");

        EXPECT_NOTNULL(str);
        EXPECT_EQ_INT(string_reserve(str, 2), 0);
        EXPECT_EQ_UINT(string_len(str), 5);
        EXPECT_EQ_STR(string_raw(str), "hello");
        EXPECT_GE_UINT(string_capacity(str), string_len(str));
        string_free(str);
    }

    {
        struct string *str = string_from_buf("hello");
        size_t cap_before;

        EXPECT_NOTNULL(str);
        cap_before = string_capacity(str);
        EXPECT_EQ_INT(string_reserve(str, cap_before), 0);
        EXPECT_EQ_UINT(string_capacity(str), cap_before);
        EXPECT_EQ_STR(string_raw(str), "hello");
        string_free(str);
    }
}

UTEST_CASE(string_shrink)
{
    EXPECT_EQ_INT(string_shrink(NULL), -1);

    {
        struct string *str = string_from_buf("hello");

        EXPECT_NOTNULL(str);
        EXPECT_EQ_INT(string_reserve(str, 64), 0);
        EXPECT_GE_UINT(string_capacity(str), 64);
        EXPECT_EQ_INT(string_shrink(str), 0);
        EXPECT_EQ_UINT(string_len(str), 5);
        EXPECT_EQ_STR(string_raw(str), "hello");
        EXPECT_GE_UINT(string_capacity(str), string_len(str));
        string_free(str);
    }

    {
        struct string *str = string_from_buf("");

        EXPECT_NOTNULL(str);
        EXPECT_EQ_INT(string_shrink(str), 0);
        EXPECT_TRUE(string_empty(str));
        string_free(str);
    }

    {
        struct string *str = string_from_buf("hi");

        EXPECT_NOTNULL(str);
        EXPECT_EQ_UINT(string_capacity(str), string_len(str));
        EXPECT_EQ_INT(string_shrink(str), 0);
        EXPECT_EQ_STR(string_raw(str), "hi");
        string_free(str);
    }
}

UTEST_CASE(string_clear)
{
    string_clear(NULL);

    {
        struct string *str = string_from_buf("hello");
        size_t cap_before;

        EXPECT_NOTNULL(str);
        cap_before = string_capacity(str);
        string_clear(str);
        EXPECT_TRUE(string_empty(str));
        EXPECT_EQ_UINT(string_len(str), 0);
        EXPECT_EQ_STR(string_raw(str), "");
        EXPECT_GE_UINT(string_capacity(str), 0);
        (void)cap_before;
        string_free(str);
    }

    {
        struct string *str = string_from_buf("hello");
        size_t cap_before;

        EXPECT_NOTNULL(str);
        cap_before = string_capacity(str);
        string_clear(str);
        EXPECT_EQ_INT(string_pushback(str, 'a'), 0);
        EXPECT_EQ_STR(string_raw(str), "a");
        EXPECT_GE_UINT(string_capacity(str), cap_before);
        string_free(str);
    }
}
