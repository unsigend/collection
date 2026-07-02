#include "common.h"

UTEST_CASE(string_assign)
{
    {
        struct string *str = string_from_buf("hello");

        EXPECT_NOTNULL(str);
        EXPECT_EQ_INT(string_assign(str, NULL), -1);
        string_free(str);
    }

    {
        struct string *str = string_from_buf("hello");

        EXPECT_NOTNULL(str);
        EXPECT_EQ_INT(string_assign(str, ""), 0);
        EXPECT_TRUE(string_empty(str));
        EXPECT_EQ_STR(string_raw(str), "");
        string_free(str);
    }

    {
        struct string *str = string_from_buf("hello");

        EXPECT_NOTNULL(str);
        EXPECT_EQ_INT(string_assign(str, "world"), 0);
        EXPECT_EQ_UINT(string_len(str), 5);
        EXPECT_EQ_STR(string_raw(str), "world");
        string_free(str);
    }

    {
        struct string *str = string_from_buf("short");

        EXPECT_NOTNULL(str);
        EXPECT_EQ_INT(string_assign(str, "much longer text"), 0);
        EXPECT_EQ_STR(string_raw(str), "much longer text");
        EXPECT_EQ_INT(string_assign(str, "x"), 0);
        EXPECT_EQ_STR(string_raw(str), "x");
        string_free(str);
    }
}

UTEST_CASE(string_assignn)
{
    {
        struct string *str = string_from_buf("hello");

        EXPECT_NOTNULL(str);
        EXPECT_EQ_INT(string_assignn(str, NULL, 3), -1);
        string_free(str);
    }

    {
        struct string *str = string_from_buf("hello");

        EXPECT_NOTNULL(str);
        EXPECT_EQ_INT(string_assignn(str, "world", 0), 0);
        EXPECT_TRUE(string_empty(str));
        EXPECT_EQ_STR(string_raw(str), "");
        string_free(str);
    }

    {
        struct string *str = string_from_buf("hello");

        EXPECT_NOTNULL(str);
        EXPECT_EQ_INT(string_assignn(str, "world", 3), 0);
        EXPECT_EQ_UINT(string_len(str), 3);
        EXPECT_EQ_STR(string_raw(str), "wor");
        string_free(str);
    }

    {
        struct string *str = string_from_buf("hello");
        const char src[] = {'a', 'b', '\0', 'c'};

        EXPECT_NOTNULL(str);
        EXPECT_EQ_INT(string_assignn(str, src, 3), 0);
        EXPECT_EQ_UINT(string_len(str), 3);
        EXPECT_EQ_CHAR(string_at(str, 0), 'a');
        EXPECT_EQ_CHAR(string_at(str, 1), 'b');
        EXPECT_EQ_CHAR(string_at(str, 2), '\0');
        string_free(str);
    }

    {
        struct string *str = string_from_buf("tiny");

        EXPECT_NOTNULL(str);
        EXPECT_EQ_INT(string_assignn(str, "0123456789", 10), 0);
        EXPECT_EQ_UINT(string_len(str), 10);
        EXPECT_EQ_STR(string_raw(str), "0123456789");
        string_free(str);
    }
}

UTEST_CASE(string_copy)
{
    {
        struct string *dest = string_from_buf("hello");
        struct string *src = string_from_buf("world");

        EXPECT_NOTNULL(dest);
        EXPECT_NOTNULL(src);
        EXPECT_EQ_INT(string_copy(dest, NULL), -1);
        string_free(dest);
        string_free(src);
    }

    {
        struct string *dest = string_from_buf("hello");
        struct string *src = string_from_buf("world");

        EXPECT_NOTNULL(dest);
        EXPECT_NOTNULL(src);
        EXPECT_EQ_INT(string_copy(dest, src), 0);
        EXPECT_EQ_STR(string_raw(dest), "world");
        EXPECT_EQ_STR(string_raw(src), "world");
        EXPECT_NE_PTR(dest, src);
        string_free(dest);
        string_free(src);
    }

    {
        struct string *dest = string_from_buf("hello");
        struct string *src = string_from_buf("world");

        EXPECT_NOTNULL(dest);
        EXPECT_NOTNULL(src);
        EXPECT_EQ_INT(string_copy(dest, src), 0);
        EXPECT_EQ_INT(string_assign(src, "changed"), 0);
        EXPECT_EQ_STR(string_raw(dest), "world");
        EXPECT_EQ_STR(string_raw(src), "changed");
        string_free(dest);
        string_free(src);
    }

    {
        struct string *dest = string_from_buf("old");
        struct string *src = string_from_buf("");

        EXPECT_NOTNULL(dest);
        EXPECT_NOTNULL(src);
        EXPECT_EQ_INT(string_copy(dest, src), 0);
        EXPECT_TRUE(string_empty(dest));
        string_free(dest);
        string_free(src);
    }
}
