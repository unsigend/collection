/* dys test suite */

#include <dys.h>
#include <string.h>
#include <utest.h>

UTEST_CASE(dysnew)
{
    dys s;

    s = dysinit("hello");
    EXPECT_NOTNULL(s);
    EXPECT_EQ_STR(s, "hello");
    EXPECT_EQ_UINT(dyslen(s), 5);
    EXPECT_EQ_UINT(dyscap(s), 6);
    EXPECT_EQ_CHAR(s[5], '\0');
    dysfree(s);

    s = dysinit("x");
    EXPECT_EQ_STR(s, "x");
    EXPECT_EQ_UINT(dyslen(s), 1);
    EXPECT_EQ_CHAR(s[1], '\0');
    dysfree(s);

    s = dysinit("");
    EXPECT_NOTNULL(s);
    EXPECT_EQ_STR(s, "");
    EXPECT_EQ_UINT(dyslen(s), 0);
    EXPECT_EQ_UINT(dyscap(s), 1);
    EXPECT_EQ_CHAR(s[0], '\0');
    dysfree(s);

    s = dysinit(NULL);
    EXPECT_NOTNULL(s);
    EXPECT_EQ_STR(s, "");
    EXPECT_EQ_UINT(dyslen(s), 0);
    EXPECT_EQ_CHAR(s[0], '\0');
    dysfree(s);
}

UTEST_CASE(dysnewlen)
{
    const char raw[] = {'a', 'b', '\0', 'c'};
    dys s;

    s = dysinitlen("hello", 5);
    EXPECT_NOTNULL(s);
    EXPECT_EQ_STR(s, "hello");
    EXPECT_EQ_UINT(dyslen(s), 5);
    EXPECT_EQ_CHAR(s[5], '\0');
    dysfree(s);

    s = dysinitlen(raw, 4);
    EXPECT_NOTNULL(s);
    EXPECT_EQ_UINT(dyslen(s), 4);
    EXPECT_TRUE(memcmp(s, raw, 4) == 0);
    EXPECT_EQ_CHAR(s[4], '\0');
    dysfree(s);

    s = dysinitlen("xyz", 0);
    EXPECT_NOTNULL(s);
    EXPECT_EQ_STR(s, "");
    EXPECT_EQ_UINT(dyslen(s), 0);
    EXPECT_EQ_CHAR(s[0], '\0');
    dysfree(s);

    s = dysinitlen(NULL, 0);
    EXPECT_NOTNULL(s);
    EXPECT_EQ_STR(s, "");
    EXPECT_EQ_UINT(dyslen(s), 0);
    dysfree(s);

    s = dysinitlen(NULL, 4);
    EXPECT_NOTNULL(s);
    EXPECT_EQ_UINT(dyslen(s), 4);
    EXPECT_EQ_UINT(dyscap(s), 5);
    EXPECT_TRUE(s[0] == 0 && s[1] == 0 && s[2] == 0 && s[3] == 0);
    EXPECT_EQ_CHAR(s[4], '\0');
    dysfree(s);

    s = dysinitlen("abcdef", 3);
    EXPECT_EQ_STR(s, "abc");
    EXPECT_EQ_UINT(dyslen(s), 3);
    dysfree(s);
}

UTEST_CASE(dysnew_empty)
{
    dys s = dysinit_empty();

    EXPECT_NOTNULL(s);
    EXPECT_EQ_STR(s, "");
    EXPECT_EQ_UINT(dyslen(s), 0);
    EXPECT_EQ_UINT(dyscap(s), 1);
    EXPECT_EQ_UINT(dysavail(s), 0);
    EXPECT_EQ_CHAR(s[0], '\0');
    dysfree(s);
}

UTEST_CASE(dysfree)
{
    dys s;

    s = dysinit("hello");
    EXPECT_NOTNULL(s);
    dysfree(s);

    s = dysinit_empty();
    EXPECT_NOTNULL(s);
    dysfree(s);

    dysfree(NULL);
}

UTEST_CASE(dyslen)
{
    const char raw[] = {'a', 'b', '\0', 'c'};
    dys s;

    s = dysinit("abc");
    EXPECT_EQ_UINT(dyslen(s), 3);
    dysfree(s);

    s = dysinit("");
    EXPECT_EQ_UINT(dyslen(s), 0);
    dysfree(s);

    s = dysinitlen(raw, 4);
    EXPECT_EQ_UINT(dyslen(s), 4);
    EXPECT_NE_UINT(dyslen(s), strlen(s));
    dysfree(s);
}

UTEST_CASE(dyscap)
{
    dys s;

    s = dysinit("abc");
    EXPECT_EQ_UINT(dyscap(s), 4);
    EXPECT_EQ_UINT(dyscap(s), dyslen(s) + 1);
    dysfree(s);

    s = dysinit_empty();
    EXPECT_EQ_UINT(dyscap(s), 1);
    dysfree(s);

    s = dysinitlen("xy", 2);
    EXPECT_EQ_UINT(dyscap(s), 3);
    dysfree(s);
}

UTEST_CASE(dysavail)
{
    dys s;

    s = dysinit("abc");
    EXPECT_EQ_UINT(dysavail(s), 0);
    EXPECT_EQ_UINT(dysavail(s), dyscap(s) - dyslen(s) - 1);
    dysfree(s);

    s = dysinit_empty();
    EXPECT_EQ_UINT(dysavail(s), 0);
    dysfree(s);

    s = dysinit("hello");
    s = dysclear(s);
    EXPECT_EQ_UINT(dysavail(s), 5);
    dysfree(s);
}

UTEST_CASE(dysdup)
{
    const char raw[] = {'a', 'b', '\0', 'c'};
    dys s, d;

    s = dysinit("hello");
    d = dysdup(s);
    EXPECT_NOTNULL(d);
    EXPECT_NE_PTR(d, s);
    EXPECT_EQ_STR(d, "hello");
    EXPECT_EQ_UINT(dyslen(d), 5);
    EXPECT_EQ_UINT(dyscap(d), 6);
    EXPECT_EQ_STR(s, "hello");
    d[0] = 'H';
    EXPECT_EQ_STR(s, "hello");
    EXPECT_EQ_STR(d, "Hello");
    dysfree(d);
    dysfree(s);

    s = dysinitlen(raw, 4);
    d = dysdup(s);
    EXPECT_EQ_UINT(dyslen(d), 4);
    EXPECT_TRUE(memcmp(d, raw, 4) == 0);
    EXPECT_EQ_CHAR(d[4], '\0');
    dysfree(d);
    dysfree(s);

    s = dysinit_empty();
    d = dysdup(s);
    EXPECT_EQ_STR(d, "");
    EXPECT_EQ_UINT(dyslen(d), 0);
    EXPECT_EQ_UINT(dyscap(d), 1);
    dysfree(d);
    dysfree(s);

    s = dysinit("ab");
    s = dysgrow(s, 8);
    d = dysdup(s);
    EXPECT_EQ_UINT(dyslen(d), 8);
    EXPECT_EQ_UINT(dyscap(d), 9);
    EXPECT_LT_UINT(dyscap(d), dyscap(s));
    dysfree(d);
    dysfree(s);
}

UTEST_CASE(dysclear)
{
    dys s, r;
    size_t cap;

    s = dysinit("hello");
    cap = dyscap(s);
    r = dysclear(s);
    EXPECT_EQ_PTR(r, s);
    EXPECT_EQ_STR(s, "");
    EXPECT_EQ_CHAR(s[0], '\0');
    EXPECT_EQ_UINT(dyslen(s), 0);
    EXPECT_EQ_UINT(dyscap(s), cap);
    EXPECT_EQ_UINT(dysavail(s), cap - 1);
    dysfree(s);

    s = dysinit_empty();
    r = dysclear(s);
    EXPECT_EQ_PTR(r, s);
    EXPECT_EQ_STR(s, "");
    EXPECT_EQ_UINT(dyslen(s), 0);
    dysfree(s);

    s = dysinit("abc");
    s = dysclear(s);
    s = dysclear(s);
    EXPECT_EQ_STR(s, "");
    EXPECT_EQ_UINT(dyslen(s), 0);
    dysfree(s);
}

UTEST_CASE(dysgrow)
{
    dys s;

    s = dysinit("abc");
    s = dysgrow(s, 3);
    EXPECT_NOTNULL(s);
    EXPECT_EQ_STR(s, "abc");
    EXPECT_EQ_UINT(dyslen(s), 3);
    EXPECT_EQ_UINT(dyscap(s), 4);
    EXPECT_EQ_UINT(dysavail(s), 0);

    s = dysgrow(s, 1);
    EXPECT_EQ_STR(s, "abc");
    EXPECT_EQ_UINT(dyslen(s), 3);
    EXPECT_EQ_UINT(dyscap(s), 4);

    s = dysgrow(s, 0);
    EXPECT_EQ_UINT(dyslen(s), 3);

    s = dysgrow(s, 4);
    EXPECT_NOTNULL(s);
    EXPECT_EQ_STR(s, "abc");
    EXPECT_EQ_UINT(dyslen(s), 4);
    EXPECT_EQ_CHAR(s[3], '\0');
    EXPECT_EQ_UINT(dyscap(s), 9);
    EXPECT_EQ_UINT(dysavail(s), 4);

    s = dysgrow(s, 10);
    EXPECT_NOTNULL(s);
    EXPECT_EQ_STR(s, "abc");
    EXPECT_EQ_UINT(dyslen(s), 10);
    EXPECT_EQ_UINT(dyscap(s), 21);
    EXPECT_EQ_UINT(dysavail(s), 10);
    dysfree(s);

    s = dysinit_empty();
    s = dysgrow(s, 8);
    EXPECT_NOTNULL(s);
    EXPECT_EQ_STR(s, "");
    EXPECT_EQ_UINT(dyslen(s), 8);
    EXPECT_EQ_UINT(dyscap(s), 17);
    EXPECT_EQ_UINT(dysavail(s), 8);
    dysfree(s);

    s = dysinit("x");
    s = dysgrow(s, 1);
    EXPECT_EQ_STR(s, "x");
    EXPECT_EQ_UINT(dyslen(s), 1);
    dysfree(s);
}

UTEST_CASE(dyscat)
{
    dys s;

    s = dysinit("hello");
    s = dyscat(s, " world");
    EXPECT_NOTNULL(s);
    EXPECT_EQ_STR(s, "hello world");
    EXPECT_EQ_UINT(dyslen(s), 11);
    EXPECT_EQ_CHAR(s[11], '\0');
    dysfree(s);

    s = dysinit_empty();
    s = dyscat(s, "hi");
    EXPECT_EQ_STR(s, "hi");
    EXPECT_EQ_UINT(dyslen(s), 2);
    dysfree(s);

    s = dysinit("abc");
    s = dyscat(s, "");
    EXPECT_EQ_STR(s, "abc");
    EXPECT_EQ_UINT(dyslen(s), 3);
    s = dyscat(s, NULL);
    EXPECT_EQ_STR(s, "abc");
    EXPECT_EQ_UINT(dyslen(s), 3);
    dysfree(s);

    s = dysinit("hello");
    s = dysclear(s);
    s = dyscat(s, "hi");
    EXPECT_EQ_STR(s, "hi");
    EXPECT_EQ_UINT(dyslen(s), 2);
    EXPECT_EQ_UINT(dyscap(s), 6);
    dysfree(s);

    s = dysinit("a");
    s = dyscat(s, "b");
    s = dyscat(s, "c");
    EXPECT_EQ_STR(s, "abc");
    EXPECT_EQ_UINT(dyslen(s), 3);
    dysfree(s);
}

UTEST_CASE(dysncat)
{
    const char raw[] = {'c', '\0', 'd'};
    dys s;

    s = dysinit("ab");
    s = dysncat(s, "xyz", 2);
    EXPECT_NOTNULL(s);
    EXPECT_EQ_STR(s, "abxy");
    EXPECT_EQ_UINT(dyslen(s), 4);
    EXPECT_EQ_CHAR(s[4], '\0');
    dysfree(s);

    s = dysinit("ab");
    s = dysncat(s, raw, 3);
    EXPECT_EQ_UINT(dyslen(s), 5);
    EXPECT_TRUE(memcmp(s, "ab", 2) == 0);
    EXPECT_TRUE(memcmp(s + 2, raw, 3) == 0);
    EXPECT_EQ_CHAR(s[5], '\0');
    dysfree(s);

    s = dysinit("abc");
    s = dysncat(s, "xyz", 0);
    EXPECT_EQ_STR(s, "abc");
    EXPECT_EQ_UINT(dyslen(s), 3);
    s = dysncat(s, NULL, 3);
    EXPECT_EQ_STR(s, "abc");
    dysfree(s);

    s = dysinit_empty();
    s = dysncat(s, "xyz", 1);
    EXPECT_EQ_STR(s, "x");
    EXPECT_EQ_UINT(dyslen(s), 1);
    dysfree(s);
}

UTEST_CASE(dyscatdys)
{
    const char raw[] = {'a', 'b', '\0', 'c'};
    dys s, t;

    s = dysinit("hello");
    t = dysinit(" world");
    s = dyscatdys(s, t);
    EXPECT_NOTNULL(s);
    EXPECT_EQ_STR(s, "hello world");
    EXPECT_EQ_UINT(dyslen(s), 11);
    EXPECT_EQ_STR(t, " world");
    dysfree(s);
    dysfree(t);

    s = dysinit("ab");
    s = dyscatdys(s, s);
    EXPECT_EQ_STR(s, "abab");
    EXPECT_EQ_UINT(dyslen(s), 4);
    EXPECT_EQ_CHAR(s[4], '\0');
    s = dyscatdys(s, s);
    EXPECT_EQ_STR(s, "abababab");
    EXPECT_EQ_UINT(dyslen(s), 8);
    dysfree(s);

    s = dysinitlen(raw, 4);
    t = dysinit("x");
    s = dyscatdys(s, t);
    EXPECT_EQ_UINT(dyslen(s), 5);
    EXPECT_TRUE(memcmp(s, raw, 4) == 0);
    EXPECT_EQ_CHAR(s[4], 'x');
    EXPECT_EQ_CHAR(s[5], '\0');
    dysfree(s);
    dysfree(t);

    s = dysinitlen(raw, 4);
    s = dyscatdys(s, s);
    EXPECT_EQ_UINT(dyslen(s), 8);
    EXPECT_TRUE(memcmp(s, raw, 4) == 0);
    EXPECT_TRUE(memcmp(s + 4, raw, 4) == 0);
    EXPECT_EQ_CHAR(s[8], '\0');
    dysfree(s);

    s = dysinit_empty();
    t = dysinit("hi");
    s = dyscatdys(s, t);
    EXPECT_EQ_STR(s, "hi");
    EXPECT_EQ_UINT(dyslen(s), 2);
    dysfree(s);
    dysfree(t);

    s = dysinit_empty();
    s = dyscatdys(s, s);
    EXPECT_EQ_STR(s, "");
    EXPECT_EQ_UINT(dyslen(s), 0);
    dysfree(s);

    s = dysinit("left");
    t = dysinit_empty();
    s = dyscatdys(s, t);
    EXPECT_EQ_STR(s, "left");
    EXPECT_EQ_UINT(dyslen(s), 4);
    dysfree(s);
    dysfree(t);
}

UTEST_CASE(dyscpy)
{
    dys s;
    size_t cap;

    s = dysinit("hello");
    cap = dyscap(s);
    s = dyscpy(s, "hi");
    EXPECT_NOTNULL(s);
    EXPECT_EQ_STR(s, "hi");
    EXPECT_EQ_UINT(dyslen(s), 2);
    EXPECT_EQ_CHAR(s[2], '\0');
    EXPECT_EQ_UINT(dyscap(s), cap);
    dysfree(s);

    s = dysinit("hi");
    s = dyscpy(s, "hello");
    EXPECT_EQ_STR(s, "hello");
    EXPECT_EQ_UINT(dyslen(s), 5);
    EXPECT_EQ_CHAR(s[5], '\0');
    dysfree(s);

    s = dysinit("abc");
    s = dyscpy(s, "");
    EXPECT_EQ_STR(s, "");
    EXPECT_EQ_UINT(dyslen(s), 0);
    EXPECT_EQ_CHAR(s[0], '\0');
    dysfree(s);

    s = dysinit_empty();
    s = dyscpy(s, "x");
    EXPECT_EQ_STR(s, "x");
    EXPECT_EQ_UINT(dyslen(s), 1);
    dysfree(s);

    s = dysinit("same");
    s = dyscpy(s, "same");
    EXPECT_EQ_STR(s, "same");
    EXPECT_EQ_UINT(dyslen(s), 4);
    dysfree(s);

    s = dysinit("abc");
    s = dyscpy(s, NULL);
    EXPECT_EQ_STR(s, "");
    EXPECT_EQ_UINT(dyslen(s), 0);
    dysfree(s);
}

UTEST_CASE(dysncpy)
{
    const char raw[] = {'a', 'b', '\0', 'c'};
    dys s;

    s = dysinit("hello");
    s = dysncpy(s, "xyz", 2);
    EXPECT_EQ_STR(s, "xy");
    EXPECT_EQ_UINT(dyslen(s), 2);
    EXPECT_EQ_CHAR(s[2], '\0');
    dysfree(s);

    s = dysinit("ab");
    s = dysncpy(s, raw, 4);
    EXPECT_EQ_UINT(dyslen(s), 4);
    EXPECT_TRUE(memcmp(s, raw, 4) == 0);
    EXPECT_EQ_CHAR(s[4], '\0');
    dysfree(s);

    s = dysinit("abc");
    s = dysncpy(s, "zzz", 0);
    EXPECT_EQ_STR(s, "");
    EXPECT_EQ_UINT(dyslen(s), 0);
    EXPECT_EQ_CHAR(s[0], '\0');
    dysfree(s);

    s = dysinit("abcd");
    s = dysncpy(s, "xy", 2);
    EXPECT_EQ_STR(s, "xy");
    EXPECT_EQ_UINT(dyslen(s), 2);
    dysfree(s);

    s = dysinit_empty();
    s = dysncpy(s, "hello", 5);
    EXPECT_EQ_STR(s, "hello");
    EXPECT_EQ_UINT(dyslen(s), 5);
    dysfree(s);
}

UTEST_CASE(dystrim)
{
    dys s, r;

    s = dysinit("  hello  ");
    r = dystrim(s, " ");
    EXPECT_EQ_PTR(r, s);
    EXPECT_EQ_STR(s, "hello");
    EXPECT_EQ_UINT(dyslen(s), 5);
    EXPECT_EQ_CHAR(s[5], '\0');
    dysfree(s);

    s = dysinit("xxhello");
    s = dystrim(s, "x");
    EXPECT_EQ_STR(s, "hello");
    EXPECT_EQ_UINT(dyslen(s), 5);
    dysfree(s);

    s = dysinit("helloyy");
    s = dystrim(s, "y");
    EXPECT_EQ_STR(s, "hello");
    dysfree(s);

    s = dysinit(" \t hi \n");
    s = dystrim(s, " \t\n");
    EXPECT_EQ_STR(s, "hi");
    EXPECT_EQ_UINT(dyslen(s), 2);
    dysfree(s);

    s = dysinit("aaaa");
    s = dystrim(s, "a");
    EXPECT_EQ_STR(s, "");
    EXPECT_EQ_UINT(dyslen(s), 0);
    EXPECT_EQ_CHAR(s[0], '\0');
    dysfree(s);

    s = dysinit("hello");
    s = dystrim(s, "xyz");
    EXPECT_EQ_STR(s, "hello");
    EXPECT_EQ_UINT(dyslen(s), 5);
    dysfree(s);

    s = dysinit("hello");
    s = dystrim(s, "");
    EXPECT_EQ_STR(s, "hello");
    dysfree(s);

    s = dysinit_empty();
    s = dystrim(s, " ");
    EXPECT_EQ_STR(s, "");
    EXPECT_EQ_UINT(dyslen(s), 0);
    dysfree(s);

    s = dysinit("x");
    s = dystrim(s, "x");
    EXPECT_EQ_STR(s, "");
    dysfree(s);

    s = dysinit("xhellox");
    s = dystrim(s, "x");
    EXPECT_EQ_STR(s, "hello");
    dysfree(s);

    s = dysinit("abchellocba");
    s = dystrim(s, "abc");
    EXPECT_EQ_STR(s, "hello");
    dysfree(s);
}

UTEST_CASE(dyscmp)
{
    const char raw[] = {'a', 'b', '\0', 'c'};
    dys a, b;

    a = dysinit("abc");
    b = dysinit("abc");
    EXPECT_EQ_INT(dyscmp(a, b), 0);
    dysfree(a);
    dysfree(b);

    a = dysinit("abc");
    b = dysinit("abd");
    EXPECT_TRUE(dyscmp(a, b) < 0);
    EXPECT_TRUE(dyscmp(b, a) > 0);
    dysfree(a);
    dysfree(b);

    a = dysinit("ab");
    b = dysinit("abc");
    EXPECT_TRUE(dyscmp(a, b) < 0);
    EXPECT_TRUE(dyscmp(b, a) > 0);
    dysfree(a);
    dysfree(b);

    a = dysinit_empty();
    b = dysinit_empty();
    EXPECT_EQ_INT(dyscmp(a, b), 0);
    dysfree(a);
    dysfree(b);

    a = dysinit_empty();
    b = dysinit("a");
    EXPECT_TRUE(dyscmp(a, b) < 0);
    EXPECT_TRUE(dyscmp(b, a) > 0);
    dysfree(a);
    dysfree(b);

    a = dysinitlen(raw, 4);
    b = dysinit("ab");
    EXPECT_TRUE(dyscmp(a, b) > 0);
    EXPECT_TRUE(dyscmp(b, a) < 0);
    dysfree(a);
    dysfree(b);

    a = dysinitlen(raw, 4);
    b = dysinitlen(raw, 4);
    EXPECT_EQ_INT(dyscmp(a, b), 0);
    dysfree(a);
    dysfree(b);

    a = dysinit("a");
    EXPECT_EQ_INT(dyscmp(a, a), 0);
    dysfree(a);
}

UTEST_CASE(dystolower)
{
    const char raw[] = {'A', 'B', '\0', 'C'};
    dys s;

    s = dysinit("HeLLo");
    dystolower(s);
    EXPECT_EQ_STR(s, "hello");
    EXPECT_EQ_UINT(dyslen(s), 5);
    dysfree(s);

    s = dysinit("already");
    dystolower(s);
    EXPECT_EQ_STR(s, "already");
    dysfree(s);

    s = dysinit("ABC123!@#");
    dystolower(s);
    EXPECT_EQ_STR(s, "abc123!@#");
    dysfree(s);

    s = dysinit_empty();
    dystolower(s);
    EXPECT_EQ_STR(s, "");
    dysfree(s);

    s = dysinitlen(raw, 4);
    dystolower(s);
    EXPECT_EQ_CHAR(s[0], 'a');
    EXPECT_EQ_CHAR(s[1], 'b');
    EXPECT_EQ_CHAR(s[2], '\0');
    EXPECT_EQ_CHAR(s[3], 'c');
    EXPECT_EQ_UINT(dyslen(s), 4);
    dysfree(s);
}

UTEST_CASE(dystoupper)
{
    const char raw[] = {'a', 'b', '\0', 'c'};
    dys s;

    s = dysinit("HeLLo");
    dystoupper(s);
    EXPECT_EQ_STR(s, "HELLO");
    EXPECT_EQ_UINT(dyslen(s), 5);
    dysfree(s);

    s = dysinit("ALREADY");
    dystoupper(s);
    EXPECT_EQ_STR(s, "ALREADY");
    dysfree(s);

    s = dysinit("abc123!@#");
    dystoupper(s);
    EXPECT_EQ_STR(s, "ABC123!@#");
    dysfree(s);

    s = dysinit_empty();
    dystoupper(s);
    EXPECT_EQ_STR(s, "");
    dysfree(s);

    s = dysinitlen(raw, 4);
    dystoupper(s);
    EXPECT_EQ_CHAR(s[0], 'A');
    EXPECT_EQ_CHAR(s[1], 'B');
    EXPECT_EQ_CHAR(s[2], '\0');
    EXPECT_EQ_CHAR(s[3], 'C');
    EXPECT_EQ_UINT(dyslen(s), 4);
    dysfree(s);
}

UTEST_SUITE(dys)
{
    UTEST_RUNCASE(dysnew);
    UTEST_RUNCASE(dysnewlen);
    UTEST_RUNCASE(dysnew_empty);
    UTEST_RUNCASE(dysfree);
    UTEST_RUNCASE(dyslen);
    UTEST_RUNCASE(dyscap);
    UTEST_RUNCASE(dysavail);
    UTEST_RUNCASE(dysdup);
    UTEST_RUNCASE(dysclear);
    UTEST_RUNCASE(dysgrow);
    UTEST_RUNCASE(dyscat);
    UTEST_RUNCASE(dysncat);
    UTEST_RUNCASE(dyscatdys);
    UTEST_RUNCASE(dyscpy);
    UTEST_RUNCASE(dysncpy);
    UTEST_RUNCASE(dystrim);
    UTEST_RUNCASE(dyscmp);
    UTEST_RUNCASE(dystolower);
    UTEST_RUNCASE(dystoupper);
}
