/* Tests for dys. */

#include <dys.h>
#include <string.h>
#include <utest.h>

UTEST_CASE(dysgethdr)
{
    dys s;
    struct dyshdr *hdr;

    s = dysnew("hi");
    EXPECT_NOTNULL(s);
    hdr = dysgethdr(s);
    EXPECT_EQ_PTR(hdr->buf, s);
    EXPECT_EQ_UINT(hdr->len, 2);
    EXPECT_GE_UINT(hdr->cap, hdr->len);
    dysfree(s);
}

UTEST_CASE(dyslen)
{
    dys s;
    const char bin[] = {'a', '\0', 'b'};

    s = dysnewempty();
    EXPECT_NOTNULL(s);
    EXPECT_EQ_UINT(dyslen(s), 0);
    dysfree(s);

    s = dysnew("hello");
    EXPECT_NOTNULL(s);
    EXPECT_EQ_UINT(dyslen(s), 5);
    dysfree(s);

    s = dysnewlen(bin, 3);
    EXPECT_NOTNULL(s);
    EXPECT_EQ_UINT(dyslen(s), 3);
    dysfree(s);
}

UTEST_CASE(dyscap)
{
    dys s;

    s = dysnew("abc");
    EXPECT_NOTNULL(s);
    EXPECT_GE_UINT(dyscap(s), dyslen(s));
    dysfree(s);

    s = dysnewempty();
    EXPECT_NOTNULL(s);
    EXPECT_GE_UINT(dyscap(s), 0);
    dysfree(s);
}

UTEST_CASE(dysavail)
{
    dys s;

    s = dysnew("xyz");
    EXPECT_NOTNULL(s);
    EXPECT_EQ_UINT(dysavail(s), dyscap(s) - dyslen(s));
    dysfree(s);
}

UTEST_CASE(dysempty)
{
    dys s;

    s = dysnewempty();
    EXPECT_NOTNULL(s);
    EXPECT_TRUE(dysempty(s));
    dysfree(s);

    s = dysnew("x");
    EXPECT_NOTNULL(s);
    EXPECT_FALSE(dysempty(s));
    dysfree(s);
}

UTEST_CASE(dysnewlen)
{
    dys s;
    const char bin[] = {'a', '\0', 'b', '\0', 'c'};

    s = dysnewlen("hello", 5);
    EXPECT_NOTNULL(s);
    EXPECT_EQ_UINT(dyslen(s), 5);
    EXPECT_GE_UINT(dyscap(s), 5);
    EXPECT_EQ_UINT(dysavail(s), dyscap(s) - dyslen(s));
    EXPECT_FALSE(dysempty(s));
    EXPECT_EQ_STR(s, "hello");
    EXPECT_EQ_CHAR(s[5], '\0');
    dysfree(s);

    s = dysnewlen("hello", 3);
    EXPECT_NOTNULL(s);
    EXPECT_EQ_UINT(dyslen(s), 3);
    EXPECT_EQ_STR(s, "hel");
    EXPECT_EQ_CHAR(s[3], '\0');
    dysfree(s);

    s = dysnewlen("", 0);
    EXPECT_NOTNULL(s);
    EXPECT_EQ_UINT(dyslen(s), 0);
    EXPECT_TRUE(dysempty(s));
    EXPECT_EQ_STR(s, "");
    EXPECT_EQ_CHAR(s[0], '\0');
    dysfree(s);

    s = dysnewlen(bin, 5);
    EXPECT_NOTNULL(s);
    EXPECT_EQ_UINT(dyslen(s), 5);
    EXPECT_EQ_INT(memcmp(s, bin, 5), 0);
    EXPECT_EQ_CHAR(s[5], '\0');
    dysfree(s);

    s = dysnewlen("Z", 1);
    EXPECT_NOTNULL(s);
    EXPECT_EQ_UINT(dyslen(s), 1);
    EXPECT_EQ_CHAR(s[0], 'Z');
    EXPECT_EQ_CHAR(s[1], '\0');
    EXPECT_FALSE(dysempty(s));
    dysfree(s);
}

UTEST_CASE(dysnew)
{
    dys s;

    s = dysnew("hello");
    EXPECT_NOTNULL(s);
    EXPECT_EQ_UINT(dyslen(s), 5);
    EXPECT_GE_UINT(dyscap(s), 5);
    EXPECT_EQ_STR(s, "hello");
    EXPECT_EQ_CHAR(s[5], '\0');
    dysfree(s);

    s = dysnew("");
    EXPECT_NOTNULL(s);
    EXPECT_EQ_UINT(dyslen(s), 0);
    EXPECT_TRUE(dysempty(s));
    EXPECT_EQ_STR(s, "");
    EXPECT_EQ_CHAR(s[0], '\0');
    dysfree(s);

    s = dysnew("Z");
    EXPECT_NOTNULL(s);
    EXPECT_EQ_UINT(dyslen(s), 1);
    EXPECT_EQ_STR(s, "Z");
    dysfree(s);

    s = dysnew("longer-than-a-word");
    EXPECT_NOTNULL(s);
    EXPECT_EQ_UINT(dyslen(s), 18);
    EXPECT_EQ_STR(s, "longer-than-a-word");
    EXPECT_EQ_CHAR(s[18], '\0');
    dysfree(s);
}

UTEST_CASE(dysnewempty)
{
    dys s;

    s = dysnewempty();
    EXPECT_NOTNULL(s);
    EXPECT_EQ_UINT(dyslen(s), 0);
    EXPECT_GE_UINT(dyscap(s), 0);
    EXPECT_TRUE(dysempty(s));
    EXPECT_EQ_STR(s, "");
    EXPECT_EQ_CHAR(s[0], '\0');
    dysfree(s);
}

UTEST_CASE(dysclear)
{
    dys s;
    size_t cap;
    const char bin[] = {'a', '\0', 'b'};

    s = dysnew("hello");
    EXPECT_NOTNULL(s);
    cap = dyscap(s);
    dysclear(s);
    EXPECT_EQ_UINT(dyslen(s), 0);
    EXPECT_TRUE(dysempty(s));
    EXPECT_EQ_UINT(dyscap(s), cap);
    EXPECT_EQ_STR(s, "");
    EXPECT_EQ_CHAR(s[0], '\0');
    dysfree(s);

    s = dysnewlen(bin, 3);
    EXPECT_NOTNULL(s);
    dysclear(s);
    EXPECT_EQ_UINT(dyslen(s), 0);
    EXPECT_EQ_CHAR(s[0], '\0');
    dysfree(s);

    s = dysnewempty();
    EXPECT_NOTNULL(s);
    dysclear(s);
    EXPECT_EQ_UINT(dyslen(s), 0);
    EXPECT_TRUE(dysempty(s));
    dysfree(s);
}

UTEST_CASE(dysfree)
{
    dys s;

    dysfree(NULL);

    s = dysnew("hello");
    EXPECT_NOTNULL(s);
    dysfree(s);
}

UTEST_CASE(dysgrow)
{
    dys s;
    const char bin[] = {'x', '\0', 'y'};

    s = dysnew("hi");
    EXPECT_NOTNULL(s);
    s = dysgrow(s, 16);
    EXPECT_NOTNULL(s);
    EXPECT_GE_UINT(dyscap(s), 16);
    EXPECT_EQ_UINT(dyslen(s), 16);
    EXPECT_EQ_CHAR(s[0], 'h');
    EXPECT_EQ_CHAR(s[1], 'i');
    EXPECT_EQ_CHAR(s[2], '\0');
    EXPECT_EQ_CHAR(s[16], '\0');
    EXPECT_EQ_UINT(dysavail(s), dyscap(s) - dyslen(s));
    dysfree(s);

    s = dysnew("abc");
    EXPECT_NOTNULL(s);
    s = dysgrow(s, 1);
    EXPECT_NOTNULL(s);
    EXPECT_EQ_UINT(dyslen(s), 3);
    EXPECT_EQ_STR(s, "abc");
    dysfree(s);

    s = dysnewempty();
    EXPECT_NOTNULL(s);
    s = dysgrow(s, 8);
    EXPECT_NOTNULL(s);
    EXPECT_GE_UINT(dyscap(s), 8);
    EXPECT_EQ_UINT(dyslen(s), 8);
    EXPECT_EQ_CHAR(s[0], '\0');
    EXPECT_EQ_CHAR(s[8], '\0');
    dysfree(s);

    s = dysnewlen(bin, 3);
    EXPECT_NOTNULL(s);
    s = dysgrow(s, 12);
    EXPECT_NOTNULL(s);
    EXPECT_GE_UINT(dyscap(s), 12);
    EXPECT_EQ_UINT(dyslen(s), 12);
    EXPECT_EQ_INT(memcmp(s, bin, 3), 0);
    EXPECT_EQ_CHAR(s[3], '\0');
    EXPECT_EQ_CHAR(s[12], '\0');
    dysfree(s);

    s = dysnew("hi");
    EXPECT_NOTNULL(s);
    s = dysgrow(s, dyslen(s));
    EXPECT_NOTNULL(s);
    EXPECT_EQ_UINT(dyslen(s), 2);
    EXPECT_EQ_STR(s, "hi");
    dysfree(s);
}

UTEST_CASE(dysdup)
{
    dys s, d;
    const char bin[] = {'a', '\0', 'b'};

    s = dysnew("hello");
    EXPECT_NOTNULL(s);
    d = dysdup(s);
    EXPECT_NOTNULL(d);
    EXPECT_NE_PTR(d, s);
    EXPECT_EQ_UINT(dyslen(d), 5);
    EXPECT_EQ_STR(d, "hello");
    EXPECT_EQ_CHAR(d[5], '\0');
    s[0] = 'H';
    EXPECT_EQ_STR(d, "hello");
    dysfree(d);
    dysfree(s);

    s = dysnewempty();
    EXPECT_NOTNULL(s);
    d = dysdup(s);
    EXPECT_NOTNULL(d);
    EXPECT_NE_PTR(d, s);
    EXPECT_EQ_UINT(dyslen(d), 0);
    EXPECT_EQ_STR(d, "");
    dysfree(d);
    dysfree(s);

    s = dysnewlen(bin, 3);
    EXPECT_NOTNULL(s);
    d = dysdup(s);
    EXPECT_NOTNULL(d);
    EXPECT_EQ_UINT(dyslen(d), 3);
    EXPECT_EQ_INT(memcmp(d, bin, 3), 0);
    EXPECT_EQ_CHAR(d[3], '\0');
    dysfree(d);
    dysfree(s);
}

UTEST_CASE(dysndup)
{
    dys s, d;
    const char bin[] = {'a', '\0', 'b', '\0', 'c'};

    s = dysnew("hello");
    EXPECT_NOTNULL(s);
    d = dysndup(s, 3);
    EXPECT_NOTNULL(d);
    EXPECT_NE_PTR(d, s);
    EXPECT_EQ_UINT(dyslen(d), 3);
    EXPECT_EQ_STR(d, "hel");
    EXPECT_EQ_CHAR(d[3], '\0');
    EXPECT_EQ_STR(s, "hello");
    dysfree(d);
    dysfree(s);

    s = dysnew("hello");
    EXPECT_NOTNULL(s);
    d = dysndup(s, 0);
    EXPECT_NOTNULL(d);
    EXPECT_EQ_UINT(dyslen(d), 0);
    EXPECT_EQ_STR(d, "");
    dysfree(d);
    dysfree(s);

    s = dysnew("hello");
    EXPECT_NOTNULL(s);
    d = dysndup(s, 5);
    EXPECT_NOTNULL(d);
    EXPECT_EQ_UINT(dyslen(d), 5);
    EXPECT_EQ_STR(d, "hello");
    dysfree(d);
    dysfree(s);

    s = dysnewlen(bin, 5);
    EXPECT_NOTNULL(s);
    d = dysndup(s, 3);
    EXPECT_NOTNULL(d);
    EXPECT_EQ_UINT(dyslen(d), 3);
    EXPECT_EQ_INT(memcmp(d, bin, 3), 0);
    EXPECT_EQ_CHAR(d[3], '\0');
    dysfree(d);

    d = dysndup(s, 100);
    EXPECT_NOTNULL(d);
    EXPECT_EQ_UINT(dyslen(d), 5);
    EXPECT_EQ_INT(memcmp(d, bin, 5), 0);
    EXPECT_EQ_CHAR(d[5], '\0');
    EXPECT_EQ_UINT(dyslen(s), 5);
    dysfree(d);
    dysfree(s);
}

UTEST_CASE(dyscat)
{
    dys s;

    s = dysnew("hello");
    EXPECT_NOTNULL(s);
    s = dyscat(s, " world");
    EXPECT_NOTNULL(s);
    EXPECT_EQ_UINT(dyslen(s), 11);
    EXPECT_GE_UINT(dyscap(s), 11);
    EXPECT_EQ_STR(s, "hello world");
    EXPECT_EQ_CHAR(s[11], '\0');
    dysfree(s);

    s = dysnewempty();
    EXPECT_NOTNULL(s);
    s = dyscat(s, "x");
    EXPECT_NOTNULL(s);
    EXPECT_EQ_STR(s, "x");
    EXPECT_EQ_UINT(dyslen(s), 1);
    dysfree(s);

    s = dysnew("keep");
    EXPECT_NOTNULL(s);
    s = dyscat(s, "");
    EXPECT_NOTNULL(s);
    EXPECT_EQ_STR(s, "keep");
    EXPECT_EQ_UINT(dyslen(s), 4);
    dysfree(s);

    s = dysnew("a");
    EXPECT_NOTNULL(s);
    s = dyscat(s, "b");
    EXPECT_NOTNULL(s);
    s = dyscat(s, "c");
    EXPECT_NOTNULL(s);
    EXPECT_EQ_STR(s, "abc");
    EXPECT_EQ_UINT(dyslen(s), 3);
    dysfree(s);

    {
        char big[64];
        size_t i;

        for (i = 0; i < 63; i++)
            big[i] = 'q';
        big[63] = '\0';
        s = dysnew("z");
        EXPECT_NOTNULL(s);
        s = dyscat(s, big);
        EXPECT_NOTNULL(s);
        EXPECT_EQ_UINT(dyslen(s), 64);
        EXPECT_GE_UINT(dyscap(s), 64);
        EXPECT_EQ_CHAR(s[0], 'z');
        EXPECT_EQ_CHAR(s[1], 'q');
        EXPECT_EQ_CHAR(s[63], 'q');
        EXPECT_EQ_CHAR(s[64], '\0');
        dysfree(s);
    }
}

UTEST_CASE(dysncat)
{
    dys s;
    const char bin[] = {'\0', 'z'};

    s = dysnew("ab");
    EXPECT_NOTNULL(s);
    s = dysncat(s, "cdef", 2);
    EXPECT_NOTNULL(s);
    EXPECT_EQ_UINT(dyslen(s), 4);
    EXPECT_EQ_STR(s, "abcd");
    EXPECT_EQ_CHAR(s[4], '\0');
    dysfree(s);

    s = dysnew("ab");
    EXPECT_NOTNULL(s);
    s = dysncat(s, bin, 2);
    EXPECT_NOTNULL(s);
    EXPECT_EQ_UINT(dyslen(s), 4);
    EXPECT_EQ_CHAR(s[0], 'a');
    EXPECT_EQ_CHAR(s[1], 'b');
    EXPECT_EQ_CHAR(s[2], '\0');
    EXPECT_EQ_CHAR(s[3], 'z');
    EXPECT_EQ_CHAR(s[4], '\0');
    dysfree(s);

    s = dysnew("ab");
    EXPECT_NOTNULL(s);
    s = dysncat(s, "xyz", 0);
    EXPECT_NOTNULL(s);
    EXPECT_EQ_STR(s, "ab");
    EXPECT_EQ_UINT(dyslen(s), 2);
    dysfree(s);
}

UTEST_CASE(dyscatdys)
{
    dys s, t;
    const char bin[] = {'\0', 'q'};

    s = dysnew("hello");
    EXPECT_NOTNULL(s);
    t = dysnew("!");
    EXPECT_NOTNULL(t);
    s = dyscatdys(s, t);
    EXPECT_NOTNULL(s);
    EXPECT_EQ_UINT(dyslen(s), 6);
    EXPECT_EQ_STR(s, "hello!");
    EXPECT_EQ_STR(t, "!");
    dysfree(t);
    dysfree(s);

    s = dysnew("ab");
    EXPECT_NOTNULL(s);
    t = dysnewlen(bin, 2);
    EXPECT_NOTNULL(t);
    s = dyscatdys(s, t);
    EXPECT_NOTNULL(s);
    EXPECT_EQ_UINT(dyslen(s), 4);
    EXPECT_EQ_CHAR(s[0], 'a');
    EXPECT_EQ_CHAR(s[1], 'b');
    EXPECT_EQ_CHAR(s[2], '\0');
    EXPECT_EQ_CHAR(s[3], 'q');
    EXPECT_EQ_CHAR(s[4], '\0');
    dysfree(t);
    dysfree(s);

    s = dysnew("keep");
    EXPECT_NOTNULL(s);
    t = dysnewempty();
    EXPECT_NOTNULL(t);
    s = dyscatdys(s, t);
    EXPECT_NOTNULL(s);
    EXPECT_EQ_STR(s, "keep");
    dysfree(t);
    dysfree(s);

    s = dysnew("ab");
    EXPECT_NOTNULL(s);
    s = dyscatdys(s, s);
    EXPECT_NOTNULL(s);
    EXPECT_EQ_UINT(dyslen(s), 4);
    EXPECT_EQ_STR(s, "abab");
    EXPECT_EQ_CHAR(s[4], '\0');
    dysfree(s);
}

UTEST_CASE(dyscpy)
{
    dys s;

    s = dysnew("hello");
    EXPECT_NOTNULL(s);
    s = dyscpy(s, "hi");
    EXPECT_NOTNULL(s);
    EXPECT_EQ_UINT(dyslen(s), 2);
    EXPECT_EQ_STR(s, "hi");
    EXPECT_EQ_CHAR(s[2], '\0');
    dysfree(s);

    s = dysnew("hi");
    EXPECT_NOTNULL(s);
    s = dyscpy(s, "hello");
    EXPECT_NOTNULL(s);
    EXPECT_EQ_UINT(dyslen(s), 5);
    EXPECT_GE_UINT(dyscap(s), 5);
    EXPECT_EQ_STR(s, "hello");
    EXPECT_EQ_CHAR(s[5], '\0');
    dysfree(s);

    s = dysnew("hello");
    EXPECT_NOTNULL(s);
    s = dyscpy(s, "");
    EXPECT_NOTNULL(s);
    EXPECT_EQ_UINT(dyslen(s), 0);
    EXPECT_TRUE(dysempty(s));
    EXPECT_EQ_STR(s, "");
    dysfree(s);
}

UTEST_CASE(dysncpy)
{
    dys s;
    const char bin[] = {'a', '\0', 'b'};

    s = dysnew("hello");
    EXPECT_NOTNULL(s);
    s = dysncpy(s, "xyz", 2);
    EXPECT_NOTNULL(s);
    EXPECT_EQ_UINT(dyslen(s), 2);
    EXPECT_EQ_STR(s, "xy");
    EXPECT_EQ_CHAR(s[2], '\0');
    dysfree(s);

    s = dysnew("hi");
    EXPECT_NOTNULL(s);
    s = dysncpy(s, bin, 3);
    EXPECT_NOTNULL(s);
    EXPECT_EQ_UINT(dyslen(s), 3);
    EXPECT_GE_UINT(dyscap(s), 3);
    EXPECT_EQ_INT(memcmp(s, bin, 3), 0);
    EXPECT_EQ_CHAR(s[3], '\0');
    dysfree(s);

    s = dysnew("hello");
    EXPECT_NOTNULL(s);
    s = dysncpy(s, "xyz", 0);
    EXPECT_NOTNULL(s);
    EXPECT_EQ_UINT(dyslen(s), 0);
    EXPECT_EQ_STR(s, "");
    dysfree(s);
}

UTEST_CASE(dystrim)
{
    dys s;

    s = dysnew("xxhelloxx");
    EXPECT_NOTNULL(s);
    dystrim(s, "x");
    EXPECT_EQ_UINT(dyslen(s), 5);
    EXPECT_EQ_STR(s, "hello");
    EXPECT_EQ_CHAR(s[5], '\0');
    dysfree(s);

    s = dysnew("  \thello\t  ");
    EXPECT_NOTNULL(s);
    dystrim(s, " \t");
    EXPECT_EQ_STR(s, "hello");
    EXPECT_EQ_UINT(dyslen(s), 5);
    dysfree(s);

    s = dysnew("hello");
    EXPECT_NOTNULL(s);
    dystrim(s, "x");
    EXPECT_EQ_STR(s, "hello");
    EXPECT_EQ_UINT(dyslen(s), 5);
    dysfree(s);

    s = dysnew("xxxx");
    EXPECT_NOTNULL(s);
    dystrim(s, "x");
    EXPECT_EQ_UINT(dyslen(s), 0);
    EXPECT_TRUE(dysempty(s));
    EXPECT_EQ_STR(s, "");
    EXPECT_EQ_CHAR(s[0], '\0');
    dysfree(s);

    s = dysnew("hello");
    EXPECT_NOTNULL(s);
    dystrim(s, "");
    EXPECT_EQ_STR(s, "hello");
    dysfree(s);

    s = dysnew("xxhello");
    EXPECT_NOTNULL(s);
    dystrim(s, "x");
    EXPECT_EQ_STR(s, "hello");
    EXPECT_EQ_UINT(dyslen(s), 5);
    dysfree(s);

    s = dysnew("helloxx");
    EXPECT_NOTNULL(s);
    dystrim(s, "x");
    EXPECT_EQ_STR(s, "hello");
    EXPECT_EQ_UINT(dyslen(s), 5);
    dysfree(s);

    s = dysnew("hexxllo");
    EXPECT_NOTNULL(s);
    dystrim(s, "x");
    EXPECT_EQ_STR(s, "hexxllo");
    EXPECT_EQ_UINT(dyslen(s), 7);
    dysfree(s);

    s = dysnewempty();
    EXPECT_NOTNULL(s);
    dystrim(s, "x");
    EXPECT_TRUE(dysempty(s));
    EXPECT_EQ_STR(s, "");
    dysfree(s);
}

UTEST_CASE(dyscmp)
{
    dys a, b;
    const char bin1[] = {'a', '\0', 'b'};
    const char bin2[] = {'a', '\0', 'c'};

    a = dysnew("abc");
    EXPECT_NOTNULL(a);
    b = dysnew("abc");
    EXPECT_NOTNULL(b);
    EXPECT_EQ_INT(dyscmp(a, b), 0);
    dysfree(b);
    dysfree(a);

    a = dysnew("abc");
    EXPECT_NOTNULL(a);
    b = dysnew("abd");
    EXPECT_NOTNULL(b);
    EXPECT_LT_INT(dyscmp(a, b), 0);
    EXPECT_GT_INT(dyscmp(b, a), 0);
    dysfree(b);
    dysfree(a);

    a = dysnew("ab");
    EXPECT_NOTNULL(a);
    b = dysnew("abc");
    EXPECT_NOTNULL(b);
    EXPECT_LT_INT(dyscmp(a, b), 0);
    EXPECT_GT_INT(dyscmp(b, a), 0);
    dysfree(b);
    dysfree(a);

    a = dysnewempty();
    EXPECT_NOTNULL(a);
    b = dysnewempty();
    EXPECT_NOTNULL(b);
    EXPECT_EQ_INT(dyscmp(a, b), 0);
    dysfree(b);
    dysfree(a);

    a = dysnewlen(bin1, 3);
    EXPECT_NOTNULL(a);
    b = dysnewlen(bin1, 3);
    EXPECT_NOTNULL(b);
    EXPECT_EQ_INT(dyscmp(a, b), 0);
    dysfree(b);
    b = dysnewlen(bin2, 3);
    EXPECT_NOTNULL(b);
    EXPECT_LT_INT(dyscmp(a, b), 0);
    EXPECT_GT_INT(dyscmp(b, a), 0);
    dysfree(b);
    dysfree(a);

    a = dysnew("same");
    EXPECT_NOTNULL(a);
    EXPECT_EQ_INT(dyscmp(a, a), 0);
    dysfree(a);

    a = dysnewempty();
    EXPECT_NOTNULL(a);
    b = dysnew("a");
    EXPECT_NOTNULL(b);
    EXPECT_LT_INT(dyscmp(a, b), 0);
    EXPECT_GT_INT(dyscmp(b, a), 0);
    dysfree(b);
    dysfree(a);
}

UTEST_CASE(dystolower)
{
    dys s;

    s = dysnew("AbC");
    EXPECT_NOTNULL(s);
    dystolower(s);
    EXPECT_EQ_STR(s, "abc");
    EXPECT_EQ_UINT(dyslen(s), 3);
    EXPECT_EQ_CHAR(s[3], '\0');
    dysfree(s);

    s = dysnew("A1!Z");
    EXPECT_NOTNULL(s);
    dystolower(s);
    EXPECT_EQ_STR(s, "a1!z");
    dysfree(s);

    s = dysnewempty();
    EXPECT_NOTNULL(s);
    dystolower(s);
    EXPECT_EQ_STR(s, "");
    dysfree(s);

    s = dysnew("abc");
    EXPECT_NOTNULL(s);
    dystolower(s);
    EXPECT_EQ_STR(s, "abc");
    EXPECT_EQ_UINT(dyslen(s), 3);
    dysfree(s);

    {
        const char bin[] = {'A', '\0', 'B'};

        s = dysnewlen(bin, 3);
        EXPECT_NOTNULL(s);
        dystolower(s);
        EXPECT_EQ_UINT(dyslen(s), 3);
        EXPECT_EQ_CHAR(s[0], 'a');
        EXPECT_EQ_CHAR(s[1], '\0');
        EXPECT_EQ_CHAR(s[2], 'b');
        EXPECT_EQ_CHAR(s[3], '\0');
        dysfree(s);
    }
}

UTEST_CASE(dystoupper)
{
    dys s;

    s = dysnew("AbC");
    EXPECT_NOTNULL(s);
    dystoupper(s);
    EXPECT_EQ_STR(s, "ABC");
    EXPECT_EQ_UINT(dyslen(s), 3);
    EXPECT_EQ_CHAR(s[3], '\0');
    dysfree(s);

    s = dysnew("a1!z");
    EXPECT_NOTNULL(s);
    dystoupper(s);
    EXPECT_EQ_STR(s, "A1!Z");
    dysfree(s);

    s = dysnewempty();
    EXPECT_NOTNULL(s);
    dystoupper(s);
    EXPECT_EQ_STR(s, "");
    dysfree(s);

    s = dysnew("ABC");
    EXPECT_NOTNULL(s);
    dystoupper(s);
    EXPECT_EQ_STR(s, "ABC");
    EXPECT_EQ_UINT(dyslen(s), 3);
    dysfree(s);

    {
        const char bin[] = {'a', '\0', 'z'};

        s = dysnewlen(bin, 3);
        EXPECT_NOTNULL(s);
        dystoupper(s);
        EXPECT_EQ_UINT(dyslen(s), 3);
        EXPECT_EQ_CHAR(s[0], 'A');
        EXPECT_EQ_CHAR(s[1], '\0');
        EXPECT_EQ_CHAR(s[2], 'Z');
        dysfree(s);
    }
}

UTEST_SUITE(dys)
{
    UTEST_RUNCASE(dysgethdr);
    UTEST_RUNCASE(dyslen);
    UTEST_RUNCASE(dyscap);
    UTEST_RUNCASE(dysavail);
    UTEST_RUNCASE(dysempty);
    UTEST_RUNCASE(dysnewlen);
    UTEST_RUNCASE(dysnew);
    UTEST_RUNCASE(dysnewempty);
    UTEST_RUNCASE(dysclear);
    UTEST_RUNCASE(dysfree);
    UTEST_RUNCASE(dysgrow);
    UTEST_RUNCASE(dysdup);
    UTEST_RUNCASE(dysndup);
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
