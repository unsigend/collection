#include "common.h"

UTEST_CASE(string_lowercase)
{
    string_lowercase(NULL);

    {
        struct string *str = string_from_buf("HeLLo WoRLD!");

        EXPECT_NOTNULL(str);
        string_lowercase(str);
        EXPECT_EQ_STR(string_raw(str), "hello world!");
        string_free(str);
    }

    {
        struct string *str = string_from_buf("123");

        EXPECT_NOTNULL(str);
        string_lowercase(str);
        EXPECT_EQ_STR(string_raw(str), "123");
        string_free(str);
    }

    {
        struct string *str = string_from_buf("already");

        EXPECT_NOTNULL(str);
        string_lowercase(str);
        EXPECT_EQ_STR(string_raw(str), "already");
        string_free(str);
    }

    {
        struct string *str = string_from_buf("");

        EXPECT_NOTNULL(str);
        string_lowercase(str);
        EXPECT_TRUE(string_empty(str));
        string_free(str);
    }
}

UTEST_CASE(string_uppercase)
{
    string_uppercase(NULL);

    {
        struct string *str = string_from_buf("HeLLo WoRLD!");

        EXPECT_NOTNULL(str);
        string_uppercase(str);
        EXPECT_EQ_STR(string_raw(str), "HELLO WORLD!");
        string_free(str);
    }

    {
        struct string *str = string_from_buf("123");

        EXPECT_NOTNULL(str);
        string_uppercase(str);
        EXPECT_EQ_STR(string_raw(str), "123");
        string_free(str);
    }

    {
        struct string *str = string_from_buf("");

        EXPECT_NOTNULL(str);
        string_uppercase(str);
        EXPECT_TRUE(string_empty(str));
        string_free(str);
    }
}

UTEST_CASE(string_trim_start)
{
    string_trim_start(NULL);

    {
        struct string *str = string_from_buf("   hello");

        EXPECT_NOTNULL(str);
        string_trim_start(str);
        EXPECT_EQ_STR(string_raw(str), "hello");
        string_free(str);
    }

    {
        struct string *str = string_from_buf("hello");

        EXPECT_NOTNULL(str);
        string_trim_start(str);
        EXPECT_EQ_STR(string_raw(str), "hello");
        string_free(str);
    }

    {
        struct string *str = string_from_buf(" \t\n\r");

        EXPECT_NOTNULL(str);
        string_trim_start(str);
        EXPECT_TRUE(string_empty(str));
        string_free(str);
    }

    {
        struct string *str = string_from_buf("  a  b  ");

        EXPECT_NOTNULL(str);
        string_trim_start(str);
        EXPECT_EQ_STR(string_raw(str), "a  b  ");
        string_free(str);
    }
}

UTEST_CASE(string_trim_end)
{
    string_trim_end(NULL);

    {
        struct string *str = string_from_buf("hello   ");

        EXPECT_NOTNULL(str);
        string_trim_end(str);
        EXPECT_EQ_STR(string_raw(str), "hello");
        string_free(str);
    }

    {
        struct string *str = string_from_buf("hello");

        EXPECT_NOTNULL(str);
        string_trim_end(str);
        EXPECT_EQ_STR(string_raw(str), "hello");
        string_free(str);
    }

    {
        struct string *str = string_from_buf("   ");

        EXPECT_NOTNULL(str);
        string_trim_end(str);
        EXPECT_TRUE(string_empty(str));
        string_free(str);
    }
}

UTEST_CASE(string_trim)
{
    string_trim(NULL);

    {
        struct string *str = string_from_buf("  hello world  ");

        EXPECT_NOTNULL(str);
        string_trim(str);
        EXPECT_EQ_STR(string_raw(str), "hello world");
        string_free(str);
    }

    {
        struct string *str = string_from_buf(" \t hello \n ");

        EXPECT_NOTNULL(str);
        string_trim(str);
        EXPECT_EQ_STR(string_raw(str), "hello");
        string_free(str);
    }

    {
        struct string *str = string_from_buf("   ");

        EXPECT_NOTNULL(str);
        string_trim(str);
        EXPECT_TRUE(string_empty(str));
        string_free(str);
    }
}

UTEST_CASE(string_reverse)
{
    string_reverse(NULL);

    {
        struct string *str = string_from_buf("hello");

        EXPECT_NOTNULL(str);
        string_reverse(str);
        EXPECT_EQ_STR(string_raw(str), "olleh");
        string_free(str);
    }

    {
        struct string *str = string_from_buf("");

        EXPECT_NOTNULL(str);
        string_reverse(str);
        EXPECT_TRUE(string_empty(str));
        string_free(str);
    }

    {
        struct string *str = string_from_buf("ab");

        EXPECT_NOTNULL(str);
        string_reverse(str);
        EXPECT_EQ_STR(string_raw(str), "ba");
        string_free(str);
    }

    {
        struct string *str = string_from_buf("a");

        EXPECT_NOTNULL(str);
        string_reverse(str);
        EXPECT_EQ_STR(string_raw(str), "a");
        string_free(str);
    }
}
