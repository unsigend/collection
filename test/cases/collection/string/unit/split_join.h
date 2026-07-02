#include "common.h"

UTEST_CASE(string_split)
{
    {
        struct string *str = string_from_buf("a,b,c");
        struct string **parts = NULL;
        size_t count = 0;

        EXPECT_NOTNULL(str);
        EXPECT_EQ_INT(string_split(NULL, &count, str, ","), -1);
        EXPECT_EQ_INT(string_split(&parts, NULL, str, ","), -1);
        string_free(str);
    }

    {
        struct string *str = string_from_buf("a,b,c");
        struct string **parts = NULL;
        size_t count = 0;

        EXPECT_NOTNULL(str);
        EXPECT_EQ_INT(string_split(&parts, &count, str, ""), -1);
        string_free(str);
    }

    {
        struct string *str = string_from_buf("a,b,c");
        struct string **parts = NULL;
        size_t count = 0;

        EXPECT_NOTNULL(str);
        EXPECT_EQ_INT(string_split(&parts, &count, str, ","), 0);
        EXPECT_EQ_UINT(count, 3);
        EXPECT_NOTNULL(parts);
        EXPECT_EQ_STR(string_raw(parts[0]), "a");
        EXPECT_EQ_STR(string_raw(parts[1]), "b");
        EXPECT_EQ_STR(string_raw(parts[2]), "c");
        string_split_free(parts, count);
        string_free(str);
    }

    {
        struct string *str = string_from_buf("hello");
        struct string **parts = NULL;
        size_t count = 0;

        EXPECT_NOTNULL(str);
        EXPECT_EQ_INT(string_split(&parts, &count, str, ","), 0);
        EXPECT_EQ_UINT(count, 1);
        EXPECT_NOTNULL(parts);
        EXPECT_EQ_STR(string_raw(parts[0]), "hello");
        string_split_free(parts, count);
        string_free(str);
    }

    {
        struct string *str = string_from_buf("a,,b");
        struct string **parts = NULL;
        size_t count = 0;

        EXPECT_NOTNULL(str);
        EXPECT_EQ_INT(string_split(&parts, &count, str, ","), 0);
        EXPECT_EQ_UINT(count, 3);
        EXPECT_NOTNULL(parts);
        EXPECT_EQ_STR(string_raw(parts[0]), "a");
        EXPECT_TRUE(string_empty(parts[1]));
        EXPECT_EQ_STR(string_raw(parts[2]), "b");
        string_split_free(parts, count);
        string_free(str);
    }

    {
        struct string *str = string_from_buf("");
        struct string **parts = NULL;
        size_t count = 0;

        EXPECT_NOTNULL(str);
        EXPECT_EQ_INT(string_split(&parts, &count, str, ","), 0);
        EXPECT_EQ_UINT(count, 1);
        EXPECT_NOTNULL(parts);
        EXPECT_TRUE(string_empty(parts[0]));
        string_split_free(parts, count);
        string_free(str);
    }

    {
        struct string *str = string_from_buf("a,b,");
        struct string **parts = NULL;
        size_t count = 0;

        EXPECT_NOTNULL(str);
        EXPECT_EQ_INT(string_split(&parts, &count, str, ","), 0);
        EXPECT_EQ_UINT(count, 3);
        EXPECT_EQ_STR(string_raw(parts[0]), "a");
        EXPECT_EQ_STR(string_raw(parts[1]), "b");
        EXPECT_TRUE(string_empty(parts[2]));
        string_split_free(parts, count);
        string_free(str);
    }
}

UTEST_CASE(string_split_free)
{
    string_split_free(NULL, 0);
    string_split_free(NULL, 3);

    {
        struct string *str = string_from_buf("a,b");
        struct string **parts = NULL;
        size_t count = 0;

        EXPECT_NOTNULL(str);
        EXPECT_EQ_INT(string_split(&parts, &count, str, ","), 0);
        string_split_free(parts, count);
        string_free(str);
    }

    {
        struct string **parts = NULL;

        string_split_free(parts, 0);
    }
}

UTEST_CASE(string_join)
{
    {

        EXPECT_EQ_INT(string_join(NULL, ",", 2, "a", "b"), -1);
    }

    {
        struct string *out = NULL;

        EXPECT_EQ_INT(string_join(&out, ",", 0), 0);
        EXPECT_NOTNULL(out);
        EXPECT_TRUE(string_empty(out));
        string_free(out);
    }

    {
        struct string *out = NULL;

        EXPECT_EQ_INT(string_join(&out, ",", 3, "a", "b", "c"), 0);
        EXPECT_NOTNULL(out);
        EXPECT_EQ_STR(string_raw(out), "a,b,c");
        string_free(out);
    }

    {
        struct string *out = NULL;

        EXPECT_EQ_INT(string_join(&out, NULL, 3, "a", "b", "c"), 0);
        EXPECT_NOTNULL(out);
        EXPECT_EQ_STR(string_raw(out), "abc");
        string_free(out);
    }

    {
        struct string *out = NULL;

        EXPECT_EQ_INT(string_join(&out, "-", 1, "only"), 0);
        EXPECT_NOTNULL(out);
        EXPECT_EQ_STR(string_raw(out), "only");
        string_free(out);
    }

    {
        struct string *out = NULL;

        EXPECT_EQ_INT(string_join(&out, ",", 3, "a", NULL, "c"), 0);
        EXPECT_NOTNULL(out);
        EXPECT_EQ_STR(string_raw(out), "a,c");
        string_free(out);
    }
}

UTEST_CASE(string_join_str)
{
    {
        struct string *a = string_from_buf("a");
        struct string *b = string_from_buf("b");
        struct string *c = string_from_buf("c");
        const struct string *parts[] = {a, b, c};

        EXPECT_NOTNULL(a);
        EXPECT_NOTNULL(b);
        EXPECT_NOTNULL(c);
        EXPECT_EQ_INT(string_join_str(NULL, ",", parts, 3), -1);
        string_free(a);
        string_free(b);
        string_free(c);
    }

    {
        struct string *a = string_from_buf("a");
        struct string *b = string_from_buf("b");
        struct string *c = string_from_buf("c");
        const struct string *parts[] = {a, b, c};
        struct string *out = NULL;

        EXPECT_NOTNULL(a);
        EXPECT_NOTNULL(b);
        EXPECT_NOTNULL(c);
        EXPECT_EQ_INT(string_join_str(&out, ",", parts, 3), 0);
        EXPECT_NOTNULL(out);
        EXPECT_EQ_STR(string_raw(out), "a,b,c");
        string_free(out);
        string_free(a);
        string_free(b);
        string_free(c);
    }

    {
        struct string *a = string_from_buf("a");
        struct string *b = string_from_buf("b");
        const struct string *parts[] = {a, b};
        struct string *out = NULL;

        EXPECT_NOTNULL(a);
        EXPECT_NOTNULL(b);
        EXPECT_EQ_INT(string_join_str(&out, NULL, parts, 2), 0);
        EXPECT_NOTNULL(out);
        EXPECT_EQ_STR(string_raw(out), "ab");
        string_free(out);
        string_free(a);
        string_free(b);
    }

    {
        struct string *a = string_from_buf("solo");
        const struct string *parts[] = {a};
        struct string *out = NULL;

        EXPECT_NOTNULL(a);
        EXPECT_EQ_INT(string_join_str(&out, "|", parts, 1), 0);
        EXPECT_NOTNULL(out);
        EXPECT_EQ_STR(string_raw(out), "solo");
        string_free(out);
        string_free(a);
    }

    {
        struct string *out = NULL;
        const struct string *parts[] = {NULL};

        EXPECT_EQ_INT(string_join_str(&out, ",", parts, 0), 0);
        EXPECT_NOTNULL(out);
        EXPECT_TRUE(string_empty(out));
        string_free(out);
    }

    {
        struct string *a = string_from_buf("a");
        struct string *empty = string_from_buf("");
        struct string *c = string_from_buf("c");
        const struct string *parts[] = {a, empty, c};
        struct string *out = NULL;

        EXPECT_NOTNULL(a);
        EXPECT_NOTNULL(empty);
        EXPECT_NOTNULL(c);
        EXPECT_EQ_INT(string_join_str(&out, ",", parts, 3), 0);
        EXPECT_EQ_STR(string_raw(out), "a,,c");
        string_free(out);
        string_free(a);
        string_free(empty);
        string_free(c);
    }

    {
        struct string *a = string_from_buf("a");
        struct string *c = string_from_buf("c");
        const struct string *parts[] = {a, NULL, c};
        struct string *out = NULL;

        EXPECT_NOTNULL(a);
        EXPECT_NOTNULL(c);
        EXPECT_EQ_INT(string_join_str(&out, ",", parts, 3), 0);
        EXPECT_EQ_STR(string_raw(out), "a,c");
        string_free(out);
        string_free(a);
        string_free(c);
    }
}
