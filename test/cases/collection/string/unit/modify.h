#include "common.h"

UTEST_CASE(string_pushback)
{
    EXPECT_EQ_INT(string_pushback(NULL, 'a'), -1);

    {
        struct string *str = string_from_buf("hel");

        EXPECT_NOTNULL(str);
        EXPECT_EQ_INT(string_pushback(str, 'l'), 0);
        EXPECT_EQ_INT(string_pushback(str, 'o'), 0);
        EXPECT_EQ_STR(string_raw(str), "hello");
        EXPECT_EQ_UINT(string_len(str), 5);
        string_free(str);
    }

    {
        struct string *str = NULL;

        EXPECT_EQ_INT(string_init(&str), 0);
        EXPECT_EQ_INT(string_pushback(str, 'a'), 0);
        EXPECT_EQ_STR(string_raw(str), "a");
        string_free(str);
    }

    {
        struct string *str = NULL;
        size_t i;

        EXPECT_EQ_INT(string_init(&str), 0);
        for (i = 0; i < 32; i++)
            EXPECT_EQ_INT(string_pushback(str, (char)('a' + (int)(i % 26))), 0);
        EXPECT_EQ_UINT(string_len(str), 32);
        EXPECT_GE_UINT(string_capacity(str), 32);
        string_free(str);
    }
}

UTEST_CASE(string_popback)
{
    {
        char out = 'x';

        EXPECT_EQ_INT(string_popback(NULL, &out), -1);
        EXPECT_EQ_INT(string_popback(NULL, NULL), -1);
    }

    {
        struct string *str = string_from_buf("");

        EXPECT_NOTNULL(str);
        EXPECT_EQ_INT(string_popback(str, NULL), -1);
        string_free(str);
    }

    {
        struct string *str = string_from_buf("hello");
        char out = '\0';

        EXPECT_NOTNULL(str);
        EXPECT_EQ_INT(string_popback(str, &out), 0);
        EXPECT_EQ_CHAR(out, 'o');
        EXPECT_EQ_STR(string_raw(str), "hell");
        string_free(str);
    }

    {
        struct string *str = string_from_buf("a");

        EXPECT_NOTNULL(str);
        EXPECT_EQ_INT(string_popback(str, NULL), 0);
        EXPECT_TRUE(string_empty(str));
        string_free(str);
    }

    {
        struct string *str = string_from_buf("abc");

        EXPECT_NOTNULL(str);
        EXPECT_EQ_INT(string_popback(str, NULL), 0);
        EXPECT_EQ_INT(string_popback(str, NULL), 0);
        EXPECT_EQ_INT(string_popback(str, NULL), 0);
        EXPECT_EQ_INT(string_popback(str, NULL), -1);
        EXPECT_TRUE(string_empty(str));
        string_free(str);
    }
}

UTEST_CASE(string_append)
{
    {
        struct string *str = string_from_buf("hello");

        EXPECT_NOTNULL(str);
        EXPECT_EQ_INT(string_append(str, NULL), -1);
        string_free(str);
    }

    {
        struct string *str = string_from_buf("hello");

        EXPECT_NOTNULL(str);
        EXPECT_EQ_INT(string_append(str, ""), 0);
        EXPECT_EQ_STR(string_raw(str), "hello");
        string_free(str);
    }

    {
        struct string *str = string_from_buf("hello");

        EXPECT_NOTNULL(str);
        EXPECT_EQ_INT(string_append(str, " world"), 0);
        EXPECT_EQ_STR(string_raw(str), "hello world");
        EXPECT_EQ_UINT(string_len(str), 11);
        string_free(str);
    }

    {
        struct string *str = NULL;

        EXPECT_EQ_INT(string_init(&str), 0);
        EXPECT_EQ_INT(string_append(str, "abc"), 0);
        EXPECT_EQ_STR(string_raw(str), "abc");
        string_free(str);
    }
}

UTEST_CASE(string_appendn)
{
    {
        struct string *str = string_from_buf("hello");

        EXPECT_NOTNULL(str);
        EXPECT_EQ_INT(string_appendn(str, NULL, 3), -1);
        string_free(str);
    }

    {
        struct string *str = string_from_buf("hello");

        EXPECT_NOTNULL(str);
        EXPECT_EQ_INT(string_appendn(str, " world!", 0), 0);
        EXPECT_EQ_STR(string_raw(str), "hello");
        string_free(str);
    }

    {
        struct string *str = string_from_buf("hello");

        EXPECT_NOTNULL(str);
        EXPECT_EQ_INT(string_appendn(str, " world!", 3), 0);
        EXPECT_EQ_STR(string_raw(str), "hello wo");
        string_free(str);
    }

    {
        struct string *str = string_from_buf("x");
        const char src[] = {'y', '\0', 'z'};

        EXPECT_NOTNULL(str);
        EXPECT_EQ_INT(string_appendn(str, src, 3), 0);
        EXPECT_EQ_UINT(string_len(str), 4);
        EXPECT_EQ_CHAR(string_at(str, 0), 'x');
        EXPECT_EQ_CHAR(string_at(str, 1), 'y');
        EXPECT_EQ_CHAR(string_at(str, 2), '\0');
        EXPECT_EQ_CHAR(string_at(str, 3), 'z');
        string_free(str);
    }
}

UTEST_CASE(string_append_str)
{
    {
        struct string *str = string_from_buf("hello");
        struct string *other = string_from_buf(" world");

        EXPECT_NOTNULL(str);
        EXPECT_NOTNULL(other);
        EXPECT_EQ_INT(string_append_str(str, NULL), -1);
        string_free(str);
        string_free(other);
    }

    {
        struct string *str = string_from_buf("hello");
        struct string *other = string_from_buf(" world");

        EXPECT_NOTNULL(str);
        EXPECT_NOTNULL(other);
        EXPECT_EQ_INT(string_append_str(str, other), 0);
        EXPECT_EQ_STR(string_raw(str), "hello world");
        EXPECT_EQ_STR(string_raw(other), " world");
        string_free(str);
        string_free(other);
    }

    {
        struct string *str = string_from_buf("hello");
        struct string *other = string_from_buf("");

        EXPECT_NOTNULL(str);
        EXPECT_NOTNULL(other);
        EXPECT_EQ_INT(string_append_str(str, other), 0);
        EXPECT_EQ_STR(string_raw(str), "hello");
        string_free(str);
        string_free(other);
    }
}

UTEST_CASE(string_prepend)
{
    {
        struct string *str = string_from_buf("world");

        EXPECT_NOTNULL(str);
        EXPECT_EQ_INT(string_prepend(str, NULL), -1);
        string_free(str);
    }

    {
        struct string *str = string_from_buf("world");

        EXPECT_NOTNULL(str);
        EXPECT_EQ_INT(string_prepend(str, ""), 0);
        EXPECT_EQ_STR(string_raw(str), "world");
        string_free(str);
    }

    {
        struct string *str = string_from_buf("world");

        EXPECT_NOTNULL(str);
        EXPECT_EQ_INT(string_prepend(str, "hello "), 0);
        EXPECT_EQ_STR(string_raw(str), "hello world");
        string_free(str);
    }

    {
        struct string *str = NULL;

        EXPECT_EQ_INT(string_init(&str), 0);
        EXPECT_EQ_INT(string_prepend(str, "pre"), 0);
        EXPECT_EQ_STR(string_raw(str), "pre");
        string_free(str);
    }
}

UTEST_CASE(string_prependn)
{
    {
        struct string *str = string_from_buf("world");

        EXPECT_NOTNULL(str);
        EXPECT_EQ_INT(string_prependn(str, NULL, 3), -1);
        string_free(str);
    }

    {
        struct string *str = string_from_buf("world");

        EXPECT_NOTNULL(str);
        EXPECT_EQ_INT(string_prependn(str, "hello ", 0), 0);
        EXPECT_EQ_STR(string_raw(str), "world");
        string_free(str);
    }

    {
        struct string *str = string_from_buf("world");

        EXPECT_NOTNULL(str);
        EXPECT_EQ_INT(string_prependn(str, "hello ", 3), 0);
        EXPECT_EQ_STR(string_raw(str), "helworld");
        string_free(str);
    }

    {
        struct string *str = string_from_buf("end");

        EXPECT_NOTNULL(str);
        EXPECT_EQ_INT(string_prependn(str, "start", 5), 0);
        EXPECT_EQ_STR(string_raw(str), "startend");
        string_free(str);
    }
}
