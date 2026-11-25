/* Tests for dict (chained hash table). */

#include <dict.h>
#include <errno.h>
#include <hash.h>
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

static void *int_dup(const void *val)
{
    int *n;

    n = malloc(sizeof(*n));
    if (n == NULL)
        return NULL;
    *n = *(const int *)val;
    return n;
}

static void int_free(void *p) { free(p); }

static struct dict_fns str_int_fns(void)
{
    struct dict_fns fns;

    memset(&fns, 0, sizeof(fns));
    fns.hash = str_hash;
    fns.keycmp = str_cmp;
    fns.keydup = str_dup;
    fns.keyfree = str_free;
    fns.valdup = int_dup;
    fns.valfree = int_free;
    return fns;
}

static char *kdup(const char *s) { return strdup(s); }

static int *vdup(int x)
{
    int *p = malloc(sizeof(*p));

    if (p)
        *p = x;
    return p;
}

UTEST_CASE(dict_len)
{
    struct dict_fns fns = intptr_fns();
    struct dict d;

    EXPECT_EQ_INT(dict_init(&d, &fns), 0);
    EXPECT_EQ_UINT(dict_len(&d), 0);
    EXPECT_EQ_INT(dict_set(&d, ip(1), ip(10)), 0);
    EXPECT_EQ_UINT(dict_len(&d), 1);
    EXPECT_EQ_INT(dict_set(&d, ip(2), ip(20)), 0);
    EXPECT_EQ_UINT(dict_len(&d), 2);
    EXPECT_EQ_INT(dict_del(&d, ip(1), NULL), 0);
    EXPECT_EQ_UINT(dict_len(&d), 1);
    dict_free(&d);
}

UTEST_CASE(dict_empty)
{
    struct dict_fns fns = intptr_fns();
    struct dict d;

    EXPECT_EQ_INT(dict_init(&d, &fns), 0);
    EXPECT_TRUE(dict_empty(&d));
    EXPECT_EQ_INT(dict_set(&d, ip(1), ip(1)), 0);
    EXPECT_FALSE(dict_empty(&d));
    EXPECT_EQ_INT(dict_del(&d, ip(1), NULL), 0);
    EXPECT_TRUE(dict_empty(&d));
    dict_free(&d);
}

UTEST_CASE(dict_init_fns)
{
    struct dict_fns fns = intptr_fns();
    struct dict d;

    EXPECT_EQ_INT(dict_init(&d, &fns), 0);
    EXPECT_EQ_PTR(dict_gethash(&d), intptr_hash);
    EXPECT_EQ_PTR(dict_getkeycmp(&d), intptr_cmp);
    EXPECT_NULL(dict_getkeydup(&d));
    EXPECT_NULL(dict_getvaldup(&d));
    EXPECT_NULL(dict_getkeyfree(&d));
    EXPECT_NULL(dict_getvalfree(&d));
    EXPECT_GE_UINT(d.cap, 1);
    dict_free(&d);
}

UTEST_CASE(dict_init_null_fns)
{
    struct dict d;

    EXPECT_EQ_INT(dict_init(&d, NULL), 0);
    dict_sethash(&d, intptr_hash);
    dict_setkeycmp(&d, intptr_cmp);
    EXPECT_EQ_INT(dict_set(&d, ip(1), ip(2)), 0);
    EXPECT_TRUE(dict_has(&d, ip(1)));
    dict_free(&d);
}

UTEST_CASE(dict_setters_getters)
{
    struct dict d;

    EXPECT_EQ_INT(dict_init(&d, NULL), 0);
    dict_sethash(&d, str_hash);
    dict_setkeycmp(&d, str_cmp);
    dict_setkeydup(&d, str_dup);
    dict_setvaldup(&d, int_dup);
    dict_setkeyfree(&d, str_free);
    dict_setvalfree(&d, int_free);
    EXPECT_EQ_PTR(dict_gethash(&d), str_hash);
    EXPECT_EQ_PTR(dict_getkeycmp(&d), str_cmp);
    EXPECT_EQ_PTR(dict_getkeydup(&d), str_dup);
    EXPECT_EQ_PTR(dict_getvaldup(&d), int_dup);
    EXPECT_EQ_PTR(dict_getkeyfree(&d), str_free);
    EXPECT_EQ_PTR(dict_getvalfree(&d), int_free);
    dict_free(&d);
}

UTEST_CASE(dict_free_null)
{
    struct dict *p = NULL;

    dict_free(p);
    EXPECT_NULL(p);
}

UTEST_CASE(dict_clear)
{
    struct dict_fns fns = intptr_fns();
    struct dict d;
    size_t cap;

    EXPECT_EQ_INT(dict_init(&d, &fns), 0);
    EXPECT_EQ_INT(dict_set(&d, ip(1), ip(1)), 0);
    EXPECT_EQ_INT(dict_set(&d, ip(2), ip(2)), 0);
    cap = d.cap;
    dict_clear(&d);
    EXPECT_TRUE(dict_empty(&d));
    EXPECT_EQ_UINT(d.cap, cap);
    EXPECT_FALSE(dict_has(&d, ip(1)));
    dict_free(&d);
}

UTEST_CASE(dict_has_get)
{
    struct dict_fns fns = intptr_fns();
    struct dict d;
    void *val = NULL;

    EXPECT_EQ_INT(dict_init(&d, &fns), 0);
    EXPECT_FALSE(dict_has(&d, ip(1)));
    EXPECT_EQ_INT(dict_get(&d, ip(1), &val), -ENOENT);

    EXPECT_EQ_INT(dict_set(&d, ip(1), ip(100)), 0);
    EXPECT_TRUE(dict_has(&d, ip(1)));
    EXPECT_EQ_INT(dict_get(&d, ip(1), &val), 0);
    EXPECT_EQ_PTR(val, ip(100));
    EXPECT_FALSE(dict_has(&d, ip(2)));
    dict_free(&d);
}

UTEST_CASE(dict_getn)
{
    struct dict_fns fns = intptr_fns();
    struct dict d;
    struct dict_node *node = NULL;

    EXPECT_EQ_INT(dict_init(&d, &fns), 0);
    EXPECT_EQ_INT(dict_getn(&d, ip(1), &node), -ENOENT);
    EXPECT_NULL(node);

    EXPECT_EQ_INT(dict_set(&d, ip(7), ip(70)), 0);
    EXPECT_EQ_INT(dict_getn(&d, ip(7), &node), 0);
    EXPECT_NOTNULL(node);
    EXPECT_EQ_PTR(dict_key(node), ip(7));
    EXPECT_EQ_PTR(dict_val(node), ip(70));
    dict_free(&d);
}

UTEST_CASE(dict_set_overwrite)
{
    struct dict_fns fns = intptr_fns();
    struct dict d;
    void *val = NULL;

    EXPECT_EQ_INT(dict_init(&d, &fns), 0);
    EXPECT_EQ_INT(dict_set(&d, ip(1), ip(10)), 0);
    EXPECT_EQ_INT(dict_set(&d, ip(1), ip(99)), 0);
    EXPECT_EQ_UINT(dict_len(&d), 1);
    EXPECT_EQ_INT(dict_get(&d, ip(1), &val), 0);
    EXPECT_EQ_PTR(val, ip(99));
    dict_free(&d);
}

UTEST_CASE(dict_sets)
{
    struct dict_fns fns = intptr_fns();
    struct dict d;
    void *val = NULL;

    EXPECT_EQ_INT(dict_init(&d, &fns), 0);
    EXPECT_EQ_INT(dict_sets(&d, ip(1), ip(10)), 0);
    EXPECT_EQ_INT(dict_sets(&d, ip(1), ip(20)), -EEXIST);
    EXPECT_EQ_UINT(dict_len(&d), 1);
    EXPECT_EQ_INT(dict_get(&d, ip(1), &val), 0);
    EXPECT_EQ_PTR(val, ip(10));
    dict_free(&d);
}

UTEST_CASE(dict_del)
{
    struct dict_fns fns = intptr_fns();
    struct dict d;
    struct dict_pair pair;

    EXPECT_EQ_INT(dict_init(&d, &fns), 0);
    EXPECT_EQ_INT(dict_del(&d, ip(1), NULL), -ENOENT);

    EXPECT_EQ_INT(dict_set(&d, ip(1), ip(10)), 0);
    EXPECT_EQ_INT(dict_set(&d, ip(2), ip(20)), 0);
    EXPECT_EQ_INT(dict_del(&d, ip(1), NULL), 0);
    EXPECT_FALSE(dict_has(&d, ip(1)));
    EXPECT_TRUE(dict_has(&d, ip(2)));
    EXPECT_EQ_UINT(dict_len(&d), 1);

    memset(&pair, 0, sizeof(pair));
    EXPECT_EQ_INT(dict_del(&d, ip(2), &pair), 0);
    EXPECT_EQ_PTR(pair.key, ip(2));
    EXPECT_EQ_PTR(pair.val, ip(20));
    EXPECT_TRUE(dict_empty(&d));
    dict_free(&d);
}

UTEST_CASE(dict_deln)
{
    struct dict_fns fns = intptr_fns();
    struct dict d;
    struct dict_node *node = NULL;
    struct dict_pair pair;

    EXPECT_EQ_INT(dict_init(&d, &fns), 0);
    EXPECT_EQ_INT(dict_set(&d, ip(1), ip(10)), 0);
    EXPECT_EQ_INT(dict_set(&d, ip(2), ip(20)), 0);
    EXPECT_EQ_INT(dict_getn(&d, ip(1), &node), 0);

    memset(&pair, 0, sizeof(pair));
    EXPECT_EQ_INT(dict_deln(&d, node, &pair), 0);
    EXPECT_EQ_PTR(pair.key, ip(1));
    EXPECT_EQ_PTR(pair.val, ip(10));
    EXPECT_FALSE(dict_has(&d, ip(1)));
    EXPECT_TRUE(dict_has(&d, ip(2)));
    EXPECT_EQ_UINT(dict_len(&d), 1);

    EXPECT_EQ_INT(dict_getn(&d, ip(2), &node), 0);
    EXPECT_EQ_INT(dict_deln(&d, node, NULL), 0);
    EXPECT_TRUE(dict_empty(&d));
    dict_free(&d);
}

UTEST_CASE(dict_deln_missing)
{
    struct dict_fns fns = intptr_fns();
    struct dict d;
    struct dict_node orphan = {.pair = {.key = ip(99), .val = ip(1)},
                               .next = NULL};

    EXPECT_EQ_INT(dict_init(&d, &fns), 0);
    EXPECT_EQ_INT(dict_set(&d, ip(1), ip(10)), 0);
    EXPECT_EQ_INT(dict_deln(&d, &orphan, NULL), -ENOENT);
    EXPECT_TRUE(dict_has(&d, ip(1)));
    dict_free(&d);
}

UTEST_CASE(dict_grow)
{
    struct dict_fns fns = intptr_fns();
    struct dict d;
    size_t cap0;
    void *val = NULL;
    intptr_t i;

    EXPECT_EQ_INT(dict_init(&d, &fns), 0);
    cap0 = d.cap;
    for (i = 0; i < 64; i++)
        EXPECT_EQ_INT(dict_set(&d, ip(i), ip(i * 10)), 0);
    EXPECT_GT_UINT(d.cap, cap0);
    EXPECT_EQ_UINT(dict_len(&d), 64);
    for (i = 0; i < 64; i++) {
        EXPECT_EQ_INT(dict_get(&d, ip(i), &val), 0);
        EXPECT_EQ_PTR(val, ip(i * 10));
    }
    dict_free(&d);
}

UTEST_CASE(dict_shrink)
{
    struct dict_fns fns = intptr_fns();
    struct dict d;
    size_t cap_big;
    intptr_t i;

    EXPECT_EQ_INT(dict_init(&d, &fns), 0);
    for (i = 0; i < 64; i++)
        EXPECT_EQ_INT(dict_set(&d, ip(i), ip(i)), 0);
    cap_big = d.cap;
    EXPECT_GT_UINT(cap_big, 16);

    for (i = 0; i < 64; i++)
        EXPECT_EQ_INT(dict_del(&d, ip(i), NULL), 0);
    EXPECT_TRUE(dict_empty(&d));
    EXPECT_LE_UINT(d.cap, cap_big);
    EXPECT_GE_UINT(d.cap, 16);
    dict_free(&d);
}

UTEST_CASE(dict_iter_empty)
{
    struct dict_fns fns = intptr_fns();
    struct dict d;
    struct dict_iter it;

    EXPECT_EQ_INT(dict_init(&d, &fns), 0);
    dict_iter_init(&it, &d);
    EXPECT_TRUE(dict_iter_done(&it));
    EXPECT_NULL(dict_iter_node(&it));
    EXPECT_NULL(dict_iter_get(&it));
    dict_iter_next(&it);
    EXPECT_TRUE(dict_iter_done(&it));
    dict_free(&d);
}

UTEST_CASE(dict_iter_visit_all)
{
    struct dict_fns fns = intptr_fns();
    struct dict d;
    struct dict_iter it;
    int seen[32];
    int n = 0;
    intptr_t i;

    memset(seen, 0, sizeof(seen));
    EXPECT_EQ_INT(dict_init(&d, &fns), 0);
    for (i = 0; i < 32; i++)
        EXPECT_EQ_INT(dict_set(&d, ip(i), ip(i + 100)), 0);

    for (dict_iter_init(&it, &d); !dict_iter_done(&it); dict_iter_next(&it)) {
        struct dict_pair *p = dict_iter_get(&it);
        intptr_t k;

        EXPECT_NOTNULL(p);
        EXPECT_EQ_PTR(dict_iter_node(&it)->pair.key, p->key);
        k = (intptr_t)p->key;
        EXPECT_GE_INT((int)k, 0);
        EXPECT_LT_INT((int)k, 32);
        EXPECT_EQ_PTR(p->val, ip(k + 100));
        seen[k]++;
        n++;
    }
    EXPECT_EQ_INT(n, 32);
    for (i = 0; i < 32; i++)
        EXPECT_EQ_INT(seen[i], 1);
    dict_free(&d);
}

UTEST_CASE(dict_iter_deln)
{
    struct dict_fns fns = intptr_fns();
    struct dict d;
    struct dict_iter it;
    struct dict_node *node;

    EXPECT_EQ_INT(dict_init(&d, &fns), 0);
    EXPECT_EQ_INT(dict_set(&d, ip(1), ip(10)), 0);
    EXPECT_EQ_INT(dict_set(&d, ip(2), ip(20)), 0);
    EXPECT_EQ_INT(dict_set(&d, ip(3), ip(30)), 0);

    dict_iter_init(&it, &d);
    while (!dict_iter_done(&it)) {
        node = dict_iter_node(&it);
        dict_iter_next(&it);
        if ((intptr_t)dict_key(node) == 2)
            EXPECT_EQ_INT(dict_deln(&d, node, NULL), 0);
    }
    EXPECT_EQ_UINT(dict_len(&d), 2);
    EXPECT_FALSE(dict_has(&d, ip(2)));
    EXPECT_TRUE(dict_has(&d, ip(1)));
    EXPECT_TRUE(dict_has(&d, ip(3)));
    dict_free(&d);
}

UTEST_CASE(dict_dup_intptr)
{
    struct dict_fns fns = intptr_fns();
    struct dict d, copy;
    void *val = NULL;

    EXPECT_EQ_INT(dict_init(&d, &fns), 0);
    EXPECT_EQ_INT(dict_set(&d, ip(1), ip(10)), 0);
    EXPECT_EQ_INT(dict_set(&d, ip(2), ip(20)), 0);

    EXPECT_EQ_INT(dict_dup(&d, &d), -EINVAL);
    EXPECT_EQ_INT(dict_dup(&d, &copy), 0);
    EXPECT_EQ_UINT(dict_len(&copy), 2);
    EXPECT_EQ_INT(dict_get(&copy, ip(1), &val), 0);
    EXPECT_EQ_PTR(val, ip(10));
    EXPECT_EQ_INT(dict_get(&copy, ip(2), &val), 0);
    EXPECT_EQ_PTR(val, ip(20));

    EXPECT_EQ_INT(dict_set(&copy, ip(3), ip(30)), 0);
    EXPECT_FALSE(dict_has(&d, ip(3)));
    EXPECT_TRUE(dict_has(&copy, ip(3)));

    dict_free(&copy);
    dict_free(&d);
}

UTEST_CASE(dict_heap_set_get)
{
    struct dict_fns fns = str_int_fns();
    struct dict d;
    void *val = NULL;

    EXPECT_EQ_INT(dict_init(&d, &fns), 0);
    EXPECT_EQ_INT(dict_set(&d, kdup("alpha"), vdup(1)), 0);
    EXPECT_EQ_INT(dict_set(&d, kdup("beta"), vdup(2)), 0);

    EXPECT_TRUE(dict_has(&d, "alpha"));
    EXPECT_TRUE(dict_has(&d, "beta"));
    EXPECT_FALSE(dict_has(&d, "gamma"));

    EXPECT_EQ_INT(dict_get(&d, "alpha", &val), 0);
    EXPECT_EQ_INT(*(int *)val, 1);
    EXPECT_EQ_INT(dict_get(&d, "beta", &val), 0);
    EXPECT_EQ_INT(*(int *)val, 2);
    EXPECT_EQ_INT(dict_get(&d, "gamma", &val), -ENOENT);

    dict_free(&d);
}

UTEST_CASE(dict_heap_set_overwrite)
{
    struct dict_fns fns = str_int_fns();
    struct dict d;
    void *val = NULL;
    char *key;

    EXPECT_EQ_INT(dict_init(&d, &fns), 0);
    key = kdup("k");
    EXPECT_EQ_INT(dict_set(&d, key, vdup(1)), 0);
    /* same key pointer, new value — old value freed */
    EXPECT_EQ_INT(dict_set(&d, key, vdup(2)), 0);
    EXPECT_EQ_UINT(dict_len(&d), 1);
    EXPECT_EQ_INT(dict_get(&d, "k", &val), 0);
    EXPECT_EQ_INT(*(int *)val, 2);

    /* same value pointer: must not double-free */
    EXPECT_EQ_INT(dict_set(&d, key, val), 0);
    EXPECT_EQ_INT(dict_get(&d, "k", &val), 0);
    EXPECT_EQ_INT(*(int *)val, 2);
    dict_free(&d);
}

UTEST_CASE(dict_heap_sets)
{
    struct dict_fns fns = str_int_fns();
    struct dict d;
    char *k2;
    int *v2;

    EXPECT_EQ_INT(dict_init(&d, &fns), 0);
    EXPECT_EQ_INT(dict_sets(&d, kdup("a"), vdup(1)), 0);
    k2 = kdup("a");
    v2 = vdup(9);
    EXPECT_EQ_INT(dict_sets(&d, k2, v2), -EEXIST);
    free(k2);
    free(v2);
    EXPECT_EQ_UINT(dict_len(&d), 1);
    dict_free(&d);
}

UTEST_CASE(dict_heap_del_free)
{
    struct dict_fns fns = str_int_fns();
    struct dict d;

    EXPECT_EQ_INT(dict_init(&d, &fns), 0);
    EXPECT_EQ_INT(dict_set(&d, kdup("x"), vdup(5)), 0);
    EXPECT_EQ_INT(dict_del(&d, "x", NULL), 0);
    EXPECT_TRUE(dict_empty(&d));
    dict_free(&d);
}

UTEST_CASE(dict_heap_del_steal)
{
    struct dict_fns fns = str_int_fns();
    struct dict d;
    struct dict_pair pair;

    EXPECT_EQ_INT(dict_init(&d, &fns), 0);
    EXPECT_EQ_INT(dict_set(&d, kdup("y"), vdup(7)), 0);
    memset(&pair, 0, sizeof(pair));
    EXPECT_EQ_INT(dict_del(&d, "y", &pair), 0);
    EXPECT_EQ_STR((char *)pair.key, "y");
    EXPECT_EQ_INT(*(int *)pair.val, 7);
    free(pair.key);
    free(pair.val);
    EXPECT_TRUE(dict_empty(&d));
    dict_free(&d);
}

UTEST_CASE(dict_heap_clear)
{
    struct dict_fns fns = str_int_fns();
    struct dict d;

    EXPECT_EQ_INT(dict_init(&d, &fns), 0);
    EXPECT_EQ_INT(dict_set(&d, kdup("a"), vdup(1)), 0);
    EXPECT_EQ_INT(dict_set(&d, kdup("b"), vdup(2)), 0);
    dict_clear(&d);
    EXPECT_TRUE(dict_empty(&d));
    EXPECT_EQ_INT(dict_set(&d, kdup("c"), vdup(3)), 0);
    dict_free(&d);
}

UTEST_CASE(dict_heap_dup)
{
    struct dict_fns fns = str_int_fns();
    struct dict d, copy;
    void *val = NULL;

    EXPECT_EQ_INT(dict_init(&d, &fns), 0);
    EXPECT_EQ_INT(dict_set(&d, kdup("one"), vdup(1)), 0);
    EXPECT_EQ_INT(dict_set(&d, kdup("two"), vdup(2)), 0);

    EXPECT_EQ_INT(dict_dup(&d, &copy), 0);
    EXPECT_EQ_UINT(dict_len(&copy), 2);
    EXPECT_EQ_INT(dict_get(&copy, "one", &val), 0);
    EXPECT_EQ_INT(*(int *)val, 1);
    EXPECT_EQ_INT(dict_get(&copy, "two", &val), 0);
    EXPECT_EQ_INT(*(int *)val, 2);

    /* independent ownership */
    EXPECT_EQ_INT(dict_del(&copy, "one", NULL), 0);
    EXPECT_TRUE(dict_has(&d, "one"));
    EXPECT_FALSE(dict_has(&copy, "one"));

    dict_free(&copy);
    dict_free(&d);
}

UTEST_CASE(dict_heap_grow)
{
    struct dict_fns fns = str_int_fns();
    struct dict d;
    char buf[32];
    void *val = NULL;
    int i;
    size_t cap0;

    EXPECT_EQ_INT(dict_init(&d, &fns), 0);
    cap0 = d.cap;
    for (i = 0; i < 40; i++) {
        snprintf(buf, sizeof(buf), "k%d", i);
        EXPECT_EQ_INT(dict_set(&d, kdup(buf), vdup(i)), 0);
    }
    EXPECT_GT_UINT(d.cap, cap0);
    for (i = 0; i < 40; i++) {
        snprintf(buf, sizeof(buf), "k%d", i);
        EXPECT_EQ_INT(dict_get(&d, buf, &val), 0);
        EXPECT_EQ_INT(*(int *)val, i);
    }
    dict_free(&d);
}

UTEST_CASE(dict_heap_iter)
{
    struct dict_fns fns = str_int_fns();
    struct dict d;
    struct dict_iter it;
    int n = 0;

    EXPECT_EQ_INT(dict_init(&d, &fns), 0);
    EXPECT_EQ_INT(dict_set(&d, kdup("a"), vdup(1)), 0);
    EXPECT_EQ_INT(dict_set(&d, kdup("b"), vdup(2)), 0);
    EXPECT_EQ_INT(dict_set(&d, kdup("c"), vdup(3)), 0);

    for (dict_iter_init(&it, &d); !dict_iter_done(&it); dict_iter_next(&it)) {
        EXPECT_NOTNULL(dict_iter_get(&it));
        EXPECT_NOTNULL(dict_key(dict_iter_node(&it)));
        n++;
    }
    EXPECT_EQ_INT(n, 3);
    dict_free(&d);
}

UTEST_CASE(dict_pair_macros)
{
    struct dict_fns fns = intptr_fns();
    struct dict d;
    struct dict_node *node = NULL;

    EXPECT_EQ_INT(dict_init(&d, &fns), 0);
    EXPECT_EQ_INT(dict_set(&d, ip(5), ip(50)), 0);
    EXPECT_EQ_INT(dict_getn(&d, ip(5), &node), 0);
    EXPECT_EQ_PTR(dict_key(node), ip(5));
    EXPECT_EQ_PTR(dict_val(node), ip(50));
    EXPECT_EQ_PTR(dict_pair(node).key, ip(5));
    EXPECT_EQ_PTR(dict_pair(node).val, ip(50));
    dict_free(&d);
}

UTEST_SUITE(dict)
{
    UTEST_RUNCASE(dict_len);
    UTEST_RUNCASE(dict_empty);
    UTEST_RUNCASE(dict_init_fns);
    UTEST_RUNCASE(dict_init_null_fns);
    UTEST_RUNCASE(dict_setters_getters);
    UTEST_RUNCASE(dict_free_null);
    UTEST_RUNCASE(dict_clear);

    UTEST_RUNCASE(dict_has_get);
    UTEST_RUNCASE(dict_getn);
    UTEST_RUNCASE(dict_set_overwrite);
    UTEST_RUNCASE(dict_sets);
    UTEST_RUNCASE(dict_del);
    UTEST_RUNCASE(dict_deln);
    UTEST_RUNCASE(dict_deln_missing);

    UTEST_RUNCASE(dict_grow);
    UTEST_RUNCASE(dict_shrink);

    UTEST_RUNCASE(dict_iter_empty);
    UTEST_RUNCASE(dict_iter_visit_all);
    UTEST_RUNCASE(dict_iter_deln);

    UTEST_RUNCASE(dict_dup_intptr);

    UTEST_RUNCASE(dict_heap_set_get);
    UTEST_RUNCASE(dict_heap_set_overwrite);
    UTEST_RUNCASE(dict_heap_sets);
    UTEST_RUNCASE(dict_heap_del_free);
    UTEST_RUNCASE(dict_heap_del_steal);
    UTEST_RUNCASE(dict_heap_clear);
    UTEST_RUNCASE(dict_heap_dup);
    UTEST_RUNCASE(dict_heap_grow);
    UTEST_RUNCASE(dict_heap_iter);

    UTEST_RUNCASE(dict_pair_macros);
}
