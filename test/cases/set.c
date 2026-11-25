/* Tests for set (hash set over dict). */

#include <errno.h>
#include <hash.h>
#include <set.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <utest.h>

static void *ip(intptr_t x) { return (void *)x; }

static size_t intptr_hash(const void *key) { return (size_t)(uintptr_t)key; }

static int intptr_cmp(const void *a, const void *b)
{
    uintptr_t x = (uintptr_t)a;
    uintptr_t y = (uintptr_t)b;

    return (x > y) - (x < y);
}

static struct dict_fns intptr_fns(void)
{
    struct dict_fns fns;

    memset(&fns, 0, sizeof(fns));
    fns.hash = intptr_hash;
    fns.keycmp = intptr_cmp;
    return fns;
}

static size_t str_hash(const void *key)
{
    const char *s = key;

    return (size_t)hash_wy(s, strlen(s), 0);
}

static int str_cmp(const void *a, const void *b) { return strcmp(a, b); }

static void *str_dup(const void *key) { return strdup(key); }

static void str_free(void *p) { free(p); }

static struct dict_fns str_fns(void)
{
    struct dict_fns fns;

    memset(&fns, 0, sizeof(fns));
    fns.hash = str_hash;
    fns.keycmp = str_cmp;
    fns.keydup = str_dup;
    fns.keyfree = str_free;
    return fns;
}

UTEST_CASE(set_len)
{
    struct dict_fns fns = intptr_fns();
    struct set s;

    EXPECT_EQ_INT(set_init(&s, &fns), 0);
    EXPECT_EQ_UINT(set_len(&s), 0);
    EXPECT_EQ_INT(set_add(&s, ip(1)), 0);
    EXPECT_EQ_UINT(set_len(&s), 1);
    EXPECT_EQ_INT(set_add(&s, ip(2)), 0);
    EXPECT_EQ_UINT(set_len(&s), 2);
    EXPECT_EQ_INT(set_rm(&s, ip(1)), 0);
    EXPECT_EQ_UINT(set_len(&s), 1);
    set_free(&s);
}

UTEST_CASE(set_empty)
{
    struct dict_fns fns = intptr_fns();
    struct set s;

    EXPECT_EQ_INT(set_init(&s, &fns), 0);
    EXPECT_TRUE(set_empty(&s));
    EXPECT_EQ_INT(set_add(&s, ip(1)), 0);
    EXPECT_FALSE(set_empty(&s));
    EXPECT_EQ_INT(set_rm(&s, ip(1)), 0);
    EXPECT_TRUE(set_empty(&s));
    set_free(&s);
}

UTEST_CASE(set_free_null)
{
    struct set *p = NULL;

    set_free(p);
    EXPECT_NULL(p);
}

UTEST_CASE(set_clear)
{
    struct dict_fns fns = intptr_fns();
    struct set s;
    size_t cap;

    EXPECT_EQ_INT(set_init(&s, &fns), 0);
    EXPECT_EQ_INT(set_add(&s, ip(1)), 0);
    EXPECT_EQ_INT(set_add(&s, ip(2)), 0);
    cap = s.dict.cap;
    set_clear(&s);
    EXPECT_TRUE(set_empty(&s));
    EXPECT_EQ_UINT(s.dict.cap, cap);
    EXPECT_FALSE(set_contains(&s, ip(1)));
    set_free(&s);
}

UTEST_CASE(set_contains)
{
    struct dict_fns fns = intptr_fns();
    struct set s;

    EXPECT_EQ_INT(set_init(&s, &fns), 0);
    EXPECT_FALSE(set_contains(&s, ip(1)));
    EXPECT_EQ_INT(set_add(&s, ip(1)), 0);
    EXPECT_TRUE(set_contains(&s, ip(1)));
    EXPECT_FALSE(set_contains(&s, ip(2)));
    set_free(&s);
}

UTEST_CASE(set_add_duplicate)
{
    struct dict_fns fns = intptr_fns();
    struct set s;

    EXPECT_EQ_INT(set_init(&s, &fns), 0);
    EXPECT_EQ_INT(set_add(&s, ip(5)), 0);
    EXPECT_EQ_INT(set_add(&s, ip(5)), -EEXIST);
    EXPECT_EQ_UINT(set_len(&s), 1);
    set_free(&s);
}

UTEST_CASE(set_rm)
{
    struct dict_fns fns = intptr_fns();
    struct set s;

    EXPECT_EQ_INT(set_init(&s, &fns), 0);
    EXPECT_EQ_INT(set_rm(&s, ip(1)), -ENOENT);

    EXPECT_EQ_INT(set_add(&s, ip(1)), 0);
    EXPECT_EQ_INT(set_add(&s, ip(2)), 0);
    EXPECT_EQ_INT(set_rm(&s, ip(1)), 0);
    EXPECT_FALSE(set_contains(&s, ip(1)));
    EXPECT_TRUE(set_contains(&s, ip(2)));
    EXPECT_EQ_UINT(set_len(&s), 1);

    EXPECT_EQ_INT(set_rm(&s, ip(2)), 0);
    EXPECT_TRUE(set_empty(&s));
    set_free(&s);
}

UTEST_CASE(set_dup)
{
    struct dict_fns fns = intptr_fns();
    struct set s, copy;

    EXPECT_EQ_INT(set_init(&s, &fns), 0);
    EXPECT_EQ_INT(set_add(&s, ip(1)), 0);
    EXPECT_EQ_INT(set_add(&s, ip(2)), 0);

    EXPECT_EQ_INT(set_dup(&s, &copy), 0);
    EXPECT_EQ_UINT(set_len(&copy), 2);
    EXPECT_TRUE(set_contains(&copy, ip(1)));
    EXPECT_TRUE(set_contains(&copy, ip(2)));

    /* independent */
    EXPECT_EQ_INT(set_add(&copy, ip(3)), 0);
    EXPECT_FALSE(set_contains(&s, ip(3)));

    set_free(&copy);
    set_free(&s);
}

UTEST_CASE(set_grow)
{
    struct dict_fns fns = intptr_fns();
    struct set s;
    size_t cap0;
    intptr_t i;

    EXPECT_EQ_INT(set_init(&s, &fns), 0);
    cap0 = s.dict.cap;
    for (i = 0; i < 64; i++)
        EXPECT_EQ_INT(set_add(&s, ip(i)), 0);
    EXPECT_GT_UINT(s.dict.cap, cap0);
    EXPECT_EQ_UINT(set_len(&s), 64);
    for (i = 0; i < 64; i++)
        EXPECT_TRUE(set_contains(&s, ip(i)));
    set_free(&s);
}

UTEST_CASE(set_iter_empty)
{
    struct dict_fns fns = intptr_fns();
    struct set s;
    struct set_iter it;

    EXPECT_EQ_INT(set_init(&s, &fns), 0);
    set_iter_init(&it, &s);
    EXPECT_TRUE(set_iter_done(&it));
    EXPECT_NULL(set_iter_get(&it));
    set_iter_next(&it);
    EXPECT_TRUE(set_iter_done(&it));
    set_free(&s);
}

UTEST_CASE(set_iter_visit_all)
{
    struct dict_fns fns = intptr_fns();
    struct set s;
    struct set_iter it;
    int seen[32];
    int n = 0;
    intptr_t i;

    memset(seen, 0, sizeof(seen));
    EXPECT_EQ_INT(set_init(&s, &fns), 0);
    for (i = 0; i < 32; i++)
        EXPECT_EQ_INT(set_add(&s, ip(i)), 0);

    for (set_iter_init(&it, &s); !set_iter_done(&it); set_iter_next(&it)) {
        intptr_t k = (intptr_t)set_iter_get(&it);

        EXPECT_GE_INT((int)k, 0);
        EXPECT_LT_INT((int)k, 32);
        seen[k]++;
        n++;
    }
    EXPECT_EQ_INT(n, 32);
    for (i = 0; i < 32; i++)
        EXPECT_EQ_INT(seen[i], 1);
    set_free(&s);
}

UTEST_CASE(set_heap_add_contains)
{
    struct dict_fns fns = str_fns();
    struct set s;

    EXPECT_EQ_INT(set_init(&s, &fns), 0);
    EXPECT_EQ_INT(set_add(&s, strdup("alpha")), 0);
    EXPECT_EQ_INT(set_add(&s, strdup("beta")), 0);
    EXPECT_TRUE(set_contains(&s, "alpha"));
    EXPECT_TRUE(set_contains(&s, "beta"));
    EXPECT_FALSE(set_contains(&s, "gamma"));
    set_free(&s);
}

UTEST_CASE(set_heap_add_duplicate)
{
    struct dict_fns fns = str_fns();
    struct set s;
    char *k;

    EXPECT_EQ_INT(set_init(&s, &fns), 0);
    EXPECT_EQ_INT(set_add(&s, strdup("x")), 0);
    k = strdup("x");
    EXPECT_EQ_INT(set_add(&s, k), -EEXIST);
    free(k);
    EXPECT_EQ_UINT(set_len(&s), 1);
    set_free(&s);
}

UTEST_CASE(set_heap_rm)
{
    struct dict_fns fns = str_fns();
    struct set s;

    EXPECT_EQ_INT(set_init(&s, &fns), 0);
    EXPECT_EQ_INT(set_add(&s, strdup("a")), 0);
    EXPECT_EQ_INT(set_add(&s, strdup("b")), 0);
    EXPECT_EQ_INT(set_rm(&s, "a"), 0);
    EXPECT_FALSE(set_contains(&s, "a"));
    EXPECT_TRUE(set_contains(&s, "b"));
    EXPECT_EQ_INT(set_rm(&s, "missing"), -ENOENT);
    set_free(&s);
}

UTEST_CASE(set_heap_clear)
{
    struct dict_fns fns = str_fns();
    struct set s;

    EXPECT_EQ_INT(set_init(&s, &fns), 0);
    EXPECT_EQ_INT(set_add(&s, strdup("a")), 0);
    EXPECT_EQ_INT(set_add(&s, strdup("b")), 0);
    set_clear(&s);
    EXPECT_TRUE(set_empty(&s));
    EXPECT_EQ_INT(set_add(&s, strdup("c")), 0);
    set_free(&s);
}

UTEST_CASE(set_heap_dup)
{
    struct dict_fns fns = str_fns();
    struct set s, copy;

    EXPECT_EQ_INT(set_init(&s, &fns), 0);
    EXPECT_EQ_INT(set_add(&s, strdup("one")), 0);
    EXPECT_EQ_INT(set_add(&s, strdup("two")), 0);

    EXPECT_EQ_INT(set_dup(&s, &copy), 0);
    EXPECT_EQ_UINT(set_len(&copy), 2);
    EXPECT_TRUE(set_contains(&copy, "one"));
    EXPECT_TRUE(set_contains(&copy, "two"));

    /* independent ownership */
    EXPECT_EQ_INT(set_rm(&copy, "one"), 0);
    EXPECT_TRUE(set_contains(&s, "one"));
    EXPECT_FALSE(set_contains(&copy, "one"));

    set_free(&copy);
    set_free(&s);
}

UTEST_CASE(set_heap_iter)
{
    struct dict_fns fns = str_fns();
    struct set s;
    struct set_iter it;
    int n = 0;

    EXPECT_EQ_INT(set_init(&s, &fns), 0);
    EXPECT_EQ_INT(set_add(&s, strdup("a")), 0);
    EXPECT_EQ_INT(set_add(&s, strdup("b")), 0);
    EXPECT_EQ_INT(set_add(&s, strdup("c")), 0);

    for (set_iter_init(&it, &s); !set_iter_done(&it); set_iter_next(&it)) {
        EXPECT_NOTNULL(set_iter_get(&it));
        n++;
    }
    EXPECT_EQ_INT(n, 3);
    set_free(&s);
}

UTEST_SUITE(set)
{
    UTEST_RUNCASE(set_len);
    UTEST_RUNCASE(set_empty);
    UTEST_RUNCASE(set_free_null);
    UTEST_RUNCASE(set_clear);

    UTEST_RUNCASE(set_contains);
    UTEST_RUNCASE(set_add_duplicate);
    UTEST_RUNCASE(set_rm);
    UTEST_RUNCASE(set_dup);
    UTEST_RUNCASE(set_grow);

    UTEST_RUNCASE(set_iter_empty);
    UTEST_RUNCASE(set_iter_visit_all);

    UTEST_RUNCASE(set_heap_add_contains);
    UTEST_RUNCASE(set_heap_add_duplicate);
    UTEST_RUNCASE(set_heap_rm);
    UTEST_RUNCASE(set_heap_clear);
    UTEST_RUNCASE(set_heap_dup);
    UTEST_RUNCASE(set_heap_iter);
}
