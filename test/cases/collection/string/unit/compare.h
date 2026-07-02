#include "common.h"

UTEST_CASE(string_eq)
{
    EXPECT_TRUE(string_eq(NULL, NULL));

    {
        struct string *a = string_from_buf("a");

        EXPECT_NOTNULL(a);
        EXPECT_FALSE(string_eq(NULL, a));
        EXPECT_FALSE(string_eq(a, NULL));
        string_free(a);
    }

    {
        struct string *a = string_from_buf("hello");
        struct string *b = string_from_buf("hello");
        struct string *c = string_from_buf("world");

        EXPECT_NOTNULL(a);
        EXPECT_NOTNULL(b);
        EXPECT_NOTNULL(c);
        EXPECT_TRUE(string_eq(a, b));
        EXPECT_FALSE(string_eq(a, c));
        string_free(a);
        string_free(b);
        string_free(c);
    }

    {
        struct string *a = string_from_buf("");
        struct string *b = string_from_buf("");

        EXPECT_NOTNULL(a);
        EXPECT_NOTNULL(b);
        EXPECT_TRUE(string_eq(a, b));
        string_free(a);
        string_free(b);
    }

    {
        struct string *a = string_from_buf("hello");
        struct string *b = string_from_buf("hell");

        EXPECT_NOTNULL(a);
        EXPECT_NOTNULL(b);
        EXPECT_FALSE(string_eq(a, b));
        string_free(a);
        string_free(b);
    }
}

UTEST_CASE(string_eq_buf)
{
    EXPECT_FALSE(string_eq_buf(NULL, "hello"));

    {
        struct string *str = string_from_buf("hello");

        EXPECT_NOTNULL(str);
        EXPECT_TRUE(string_eq_buf(str, "hello"));
        EXPECT_FALSE(string_eq_buf(str, "world"));
        EXPECT_FALSE(string_eq_buf(str, ""));
        string_free(str);
    }

    {
        struct string *str = string_from_buf("");

        EXPECT_NOTNULL(str);
        EXPECT_TRUE(string_eq_buf(str, ""));
        EXPECT_FALSE(string_eq_buf(str, "a"));
        string_free(str);
    }

    {
        struct string *str = string_from_bufn("ab\0c", 3);

        EXPECT_NOTNULL(str);
        EXPECT_FALSE(string_eq_buf(str, "ab"));
        EXPECT_FALSE(string_eq_buf(str, "abc"));
        string_free(str);
    }
}

UTEST_CASE(string_cmp)
{
    {
        struct string *a = string_from_buf("a");

        EXPECT_NOTNULL(a);
        EXPECT_LT_INT(string_cmp(NULL, a), 0);
        EXPECT_GT_INT(string_cmp(a, NULL), 0);
        string_free(a);
    }

    EXPECT_EQ_INT(string_cmp(NULL, NULL), 0);

    {
        struct string *a = string_from_buf("abc");
        struct string *b = string_from_buf("abc");
        struct string *c = string_from_buf("abd");
        struct string *d = string_from_buf("abb");

        EXPECT_NOTNULL(a);
        EXPECT_NOTNULL(b);
        EXPECT_NOTNULL(c);
        EXPECT_NOTNULL(d);
        EXPECT_EQ_INT(string_cmp(a, b), 0);
        EXPECT_LT_INT(string_cmp(a, c), 0);
        EXPECT_GT_INT(string_cmp(a, d), 0);
        string_free(a);
        string_free(b);
        string_free(c);
        string_free(d);
    }

    {
        struct string *a = string_from_buf("abc");
        struct string *b = string_from_buf("abcd");

        EXPECT_NOTNULL(a);
        EXPECT_NOTNULL(b);
        EXPECT_LT_INT(string_cmp(a, b), 0);
        EXPECT_GT_INT(string_cmp(b, a), 0);
        string_free(a);
        string_free(b);
    }

    {
        struct string *empty = string_from_buf("");
        struct string *nonempty = string_from_buf("a");

        EXPECT_NOTNULL(empty);
        EXPECT_NOTNULL(nonempty);
        EXPECT_LT_INT(string_cmp(empty, nonempty), 0);
        EXPECT_GT_INT(string_cmp(nonempty, empty), 0);
        EXPECT_EQ_INT(string_cmp(empty, empty), 0);
        string_free(empty);
        string_free(nonempty);
    }
}
