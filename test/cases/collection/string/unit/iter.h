#include "common.h"

UTEST_CASE(string_iter)
{
    {
        struct string *str = string_from_buf("hello");
        struct string_iter *it = NULL;

        EXPECT_NOTNULL(str);
        EXPECT_EQ_INT(string_iter_init(NULL, str), -1);
        EXPECT_EQ_INT(string_iter_init(&it, NULL), -1);
        string_free(str);
    }

    {
        struct string *str = string_from_buf("");
        struct string_iter *it = NULL;

        EXPECT_NOTNULL(str);
        EXPECT_EQ_INT(string_iter_init(&it, str), 0);
        EXPECT_NOTNULL(it);
        EXPECT_EQ_CHAR(string_iter_get(it), '\0');
        string_iter_inc(it);
        EXPECT_EQ_CHAR(string_iter_get(it), '\0');
        string_iter_dec(it);
        EXPECT_EQ_CHAR(string_iter_get(it), '\0');
        string_iter_free(it);
        string_free(str);
    }

    {
        struct string *str = string_from_buf("a");
        struct string_iter *it = NULL;

        EXPECT_NOTNULL(str);
        EXPECT_EQ_INT(string_iter_init(&it, str), 0);
        EXPECT_EQ_CHAR(string_iter_get(it), 'a');
        string_iter_inc(it);
        EXPECT_EQ_CHAR(string_iter_get(it), '\0');
        string_iter_dec(it);
        EXPECT_EQ_CHAR(string_iter_get(it), 'a');
        string_iter_free(it);
        string_free(str);
    }

    {
        struct string *str = string_from_buf("hello");
        struct string_iter *it = NULL;
        size_t i;
        const char expect[] = "hello";

        EXPECT_NOTNULL(str);
        EXPECT_EQ_INT(string_iter_init(&it, str), 0);
        for (i = 0; i < string_len(str); i++) {
            EXPECT_EQ_CHAR(string_iter_get(it), expect[i]);
            string_iter_inc(it);
        }
        EXPECT_EQ_CHAR(string_iter_get(it), '\0');
        string_iter_inc(it);
        EXPECT_EQ_CHAR(string_iter_get(it), '\0');
        string_iter_free(it);
        string_free(str);
    }

    {
        struct string *str = string_from_buf("hello");
        struct string_iter *it = NULL;
        size_t i;

        EXPECT_NOTNULL(str);
        EXPECT_EQ_INT(string_iter_init(&it, str), 0);
        for (i = 0; i < string_len(str); i++)
            string_iter_inc(it);
        for (i = string_len(str); i > 0; i--) {
            string_iter_dec(it);
            EXPECT_EQ_CHAR(string_iter_get(it), "hello"[i - 1]);
        }
        string_iter_dec(it);
        EXPECT_EQ_CHAR(string_iter_get(it), 'h');
        string_iter_free(it);
        string_free(str);
    }

    {
        struct string *str = string_from_bufn("a\0b", 3);
        struct string_iter *it = NULL;

        EXPECT_NOTNULL(str);
        EXPECT_EQ_INT(string_iter_init(&it, str), 0);
        EXPECT_EQ_CHAR(string_iter_get(it), 'a');
        string_iter_inc(it);
        EXPECT_EQ_CHAR(string_iter_get(it), '\0');
        string_iter_inc(it);
        EXPECT_EQ_CHAR(string_iter_get(it), 'b');
        string_iter_inc(it);
        EXPECT_EQ_CHAR(string_iter_get(it), '\0');
        string_iter_free(it);
        string_free(str);
    }

    {
        struct string *str = string_from_buf("hi");
        struct string_iter *it = NULL;

        EXPECT_NOTNULL(str);
        EXPECT_EQ_INT(string_iter_init(&it, str), 0);
        string_iter_free(it);
        EXPECT_EQ_INT(string_iter_init(&it, str), 0);
        EXPECT_EQ_CHAR(string_iter_get(it), 'h');
        string_iter_free(it);
        string_free(str);
    }

    EXPECT_EQ_CHAR(string_iter_get(NULL), '\0');
    string_iter_inc(NULL);
    string_iter_dec(NULL);
    string_iter_free(NULL);
}
