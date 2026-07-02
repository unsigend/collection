#include "common.h"

UTEST_CASE(string_substr)
{
    {
        struct string *str = string_from_buf("hello");

        EXPECT_NOTNULL(str);
        EXPECT_EQ_INT(string_substr(NULL, str, 0, 2), -1);
        string_free(str);
    }

    {
        struct string *str = string_from_buf("hello");
        struct string *out = NULL;

        EXPECT_NOTNULL(str);
        EXPECT_EQ_INT(string_substr(&out, str, 10, 1), -1);
        string_free(str);
    }

    {
        struct string *str = string_from_buf("hello");
        struct string *out = NULL;

        EXPECT_NOTNULL(str);
        EXPECT_EQ_INT(string_substr(&out, str, 1, 3), 0);
        EXPECT_NOTNULL(out);
        EXPECT_EQ_STR(string_raw(out), "ell");
        string_free(out);
        string_free(str);
    }

    {
        struct string *str = string_from_buf("hello");
        struct string *out = NULL;

        EXPECT_NOTNULL(str);
        EXPECT_EQ_INT(string_substr(&out, str, 2, 100), 0);
        EXPECT_NOTNULL(out);
        EXPECT_EQ_STR(string_raw(out), "llo");
        string_free(out);
        string_free(str);
    }

    {
        struct string *str = string_from_buf("hello");
        struct string *out = NULL;

        EXPECT_NOTNULL(str);
        EXPECT_EQ_INT(string_substr(&out, str, 0, 0), 0);
        EXPECT_NOTNULL(out);
        EXPECT_TRUE(string_empty(out));
        string_free(out);
        string_free(str);
    }

    {
        struct string *str = string_from_buf("hello");
        struct string *out = NULL;

        EXPECT_NOTNULL(str);
        EXPECT_EQ_INT(string_substr(&out, str, 5, 0), 0);
        EXPECT_NOTNULL(out);
        EXPECT_TRUE(string_empty(out));
        string_free(out);
        string_free(str);
    }

    {
        struct string *str = string_from_buf("hello");
        struct string *out = NULL;

        EXPECT_NOTNULL(str);
        EXPECT_EQ_INT(string_substr(&out, str, 0, 5), 0);
        EXPECT_NOTNULL(out);
        EXPECT_TRUE(string_eq(out, str));
        string_free(out);
        string_free(str);
    }
}
