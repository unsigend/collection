#include "common.h"

UTEST_CASE(string_init)
{
    {
        EXPECT_EQ_INT(string_init(NULL), -1);
    }

    {
        struct string *str = NULL;

        EXPECT_EQ_INT(string_init(&str), 0);
        EXPECT_NOTNULL(str);
        EXPECT_TRUE(string_empty(str));
        EXPECT_EQ_UINT(string_len(str), 0);
        EXPECT_NOTNULL(string_raw(str));
        EXPECT_EQ_STR(string_raw(str), "");
        EXPECT_EQ_CHAR(string_at(str, 0), '\0');
        string_free(str);
    }

    {
        struct string *str = NULL;

        EXPECT_EQ_INT(string_init(&str), 0);
        EXPECT_EQ_INT(string_pushback(str, 'x'), 0);
        EXPECT_EQ_INT(string_pushback(str, 'y'), 0);
        EXPECT_EQ_STR(string_raw(str), "xy");
        string_free(str);
    }
}

UTEST_CASE(string_init_buf)
{
    {
        struct string *str = NULL;

        EXPECT_EQ_INT(string_init_buf(NULL, "hello"), -1);
        EXPECT_EQ_INT(string_init_buf(&str, NULL), -1);
    }

    {
        struct string *str = NULL;

        EXPECT_EQ_INT(string_init_buf(&str, ""), 0);
        EXPECT_TRUE(string_empty(str));
        EXPECT_EQ_UINT(string_len(str), 0);
        EXPECT_EQ_STR(string_raw(str), "");
        string_free(str);
    }

    {
        struct string *str = NULL;

        EXPECT_EQ_INT(string_init_buf(&str, "hello"), 0);
        EXPECT_FALSE(string_empty(str));
        EXPECT_EQ_UINT(string_len(str), 5);
        EXPECT_EQ_STR(string_raw(str), "hello");
        EXPECT_EQ_CHAR(string_at(str, 0), 'h');
        EXPECT_EQ_CHAR(string_at(str, 4), 'o');
        string_free(str);
    }

    {
        struct string *str = NULL;

        EXPECT_EQ_INT(string_init_buf(&str, "a"), 0);
        EXPECT_EQ_UINT(string_len(str), 1);
        EXPECT_EQ_STR(string_raw(str), "a");
        string_free(str);
    }
}

UTEST_CASE(string_init_bufn)
{
    {
        struct string *str = NULL;

        EXPECT_EQ_INT(string_init_bufn(NULL, "hello", 3), -1);
        EXPECT_EQ_INT(string_init_bufn(&str, NULL, 3), -1);
    }

    {
        struct string *str = NULL;

        EXPECT_EQ_INT(string_init_bufn(&str, "hello", 0), 0);
        EXPECT_TRUE(string_empty(str));
        EXPECT_EQ_UINT(string_len(str), 0);
        EXPECT_EQ_STR(string_raw(str), "");
        string_free(str);
    }

    {
        struct string *str = NULL;

        EXPECT_EQ_INT(string_init_bufn(&str, "hello", 3), 0);
        EXPECT_EQ_UINT(string_len(str), 3);
        EXPECT_EQ_STR(string_raw(str), "hel");
        string_free(str);
    }

    {
        struct string *str = NULL;
        const char src[] = {'a', 'b', '\0', 'c', 'd'};

        EXPECT_EQ_INT(string_init_bufn(&str, src, 4), 0);
        EXPECT_EQ_UINT(string_len(str), 4);
        EXPECT_EQ_CHAR(string_at(str, 0), 'a');
        EXPECT_EQ_CHAR(string_at(str, 1), 'b');
        EXPECT_EQ_CHAR(string_at(str, 2), '\0');
        EXPECT_EQ_CHAR(string_at(str, 3), 'c');
        EXPECT_EQ_CHAR(string_at(str, 4), '\0');
        string_free(str);
    }

    {
        struct string *str = NULL;

        EXPECT_EQ_INT(string_init_bufn(&str, "hello", 5), 0);
        EXPECT_EQ_UINT(string_len(str), 5);
        EXPECT_EQ_STR(string_raw(str), "hello");
        string_free(str);
    }

    {
        struct string *str = NULL;

        EXPECT_EQ_INT(string_init_bufn(&str, NULL, 0), 0);
        EXPECT_TRUE(string_empty(str));
        string_free(str);
    }
}

UTEST_CASE(string_dup)
{
    {
        struct string *src = string_from_buf("hello");
        struct string *dest = NULL;

        EXPECT_NOTNULL(src);
        EXPECT_EQ_INT(string_dup(NULL, src), -1);
        EXPECT_EQ_INT(string_dup(&dest, NULL), -1);
        string_free(src);
    }

    {
        struct string *src = string_from_buf("hello");
        struct string *dest = NULL;

        EXPECT_NOTNULL(src);
        EXPECT_EQ_INT(string_dup(&dest, src), 0);
        EXPECT_NOTNULL(dest);
        EXPECT_NE_PTR(dest, src);
        EXPECT_TRUE(string_eq(src, dest));
        EXPECT_EQ_STR(string_raw(dest), "hello");
        EXPECT_EQ_UINT(string_len(dest), 5);
        string_free(dest);
        string_free(src);
    }

    {
        struct string *src = string_from_buf("hello");
        struct string *dest = NULL;

        EXPECT_NOTNULL(src);
        EXPECT_EQ_INT(string_dup(&dest, src), 0);
        EXPECT_EQ_INT(string_assign(src, "world"), 0);
        EXPECT_FALSE(string_eq(src, dest));
        EXPECT_EQ_STR(string_raw(src), "world");
        EXPECT_EQ_STR(string_raw(dest), "hello");
        string_free(dest);
        string_free(src);
    }

    {
        struct string *src = string_from_buf("");
        struct string *dest = NULL;

        EXPECT_NOTNULL(src);
        EXPECT_EQ_INT(string_dup(&dest, src), 0);
        EXPECT_TRUE(string_empty(dest));
        EXPECT_TRUE(string_eq(src, dest));
        string_free(dest);
        string_free(src);
    }

    {
        struct string *src = string_from_bufn("ab\0cd", 4);
        struct string *dest = NULL;

        EXPECT_NOTNULL(src);
        EXPECT_EQ_INT(string_dup(&dest, src), 0);
        EXPECT_EQ_UINT(string_len(dest), 4);
        EXPECT_TRUE(string_eq(src, dest));
        string_free(dest);
        string_free(src);
    }
}

UTEST_CASE(string_free)
{
    string_free(NULL);

    {
        struct string *str = string_from_buf("hello");

        EXPECT_NOTNULL(str);
        string_free(str);
    }

    {
        struct string *a = string_from_buf("a");
        struct string *b = string_from_buf("b");

        EXPECT_NOTNULL(a);
        EXPECT_NOTNULL(b);
        string_free(a);
        string_free(b);
    }
}
