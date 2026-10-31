/* slist test suite */

#include <slist.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <utest.h>

#define V(n) ((void *)(intptr_t)(n))
#define I(p) ((int)(intptr_t)(p))

static void *dup_str(void *p) { return p ? strdup((char *)p) : NULL; }

static void free_str(void *p) { free(p); }

static int match_str(void *p, void *key)
{
    if (!p || !key)
        return 0;
    return strcmp((char *)p, (char *)key) == 0;
}

static void *dup_fail(void *p)
{
    (void)p;
    return NULL;
}

UTEST_CASE(slist_init)
{
    struct slist *l = slist_init();

    EXPECT_NOTNULL(l);
    EXPECT_NULL(slist_first(l));
    EXPECT_NULL(slist_last(l));
    EXPECT_EQ_UINT(slist_len(l), 0);
    EXPECT_TRUE(slist_empty(l));
    EXPECT_NULL(slist_getdup(l));
    EXPECT_NULL(slist_getfree(l));
    EXPECT_NULL(slist_getmatch(l));
    slist_free(l);
}

UTEST_CASE(slist_free)
{
    struct slist *l;

    l = slist_init();
    EXPECT_NOTNULL(slist_add_tail(l, V(1)));
    slist_free(l);

    l = slist_init();
    slist_free(l);

    slist_free(NULL);
}

UTEST_CASE(slist_clear)
{
    struct slist *l = slist_init();
    char *a = strdup("a"), *b = strdup("b");

    slist_setfree(l, free_str);
    EXPECT_NOTNULL(slist_add_tail(l, a));
    EXPECT_NOTNULL(slist_add_tail(l, b));
    slist_clear(l);
    EXPECT_EQ_UINT(slist_len(l), 0);
    EXPECT_NULL(slist_first(l));
    EXPECT_NULL(slist_last(l));
    EXPECT_EQ_PTR(slist_getfree(l), free_str);

    EXPECT_NOTNULL(slist_add_tail(l, strdup("c")));
    EXPECT_EQ_UINT(slist_len(l), 1);
    slist_clear(l);
    EXPECT_EQ_UINT(slist_len(l), 0);
    slist_free(l);
}

UTEST_CASE(slist_len)
{
    struct slist *l = slist_init();

    EXPECT_EQ_UINT(slist_len(l), 0);
    slist_add_head(l, V(1));
    EXPECT_EQ_UINT(slist_len(l), 1);
    slist_add_tail(l, V(2));
    EXPECT_EQ_UINT(slist_len(l), 2);
    slist_del(l, slist_first(l));
    EXPECT_EQ_UINT(slist_len(l), 1);
    slist_free(l);
}

UTEST_CASE(slist_first)
{
    struct slist *l = slist_init();

    EXPECT_NULL(slist_first(l));
    slist_add_tail(l, V(1));
    slist_add_tail(l, V(2));
    EXPECT_EQ_PTR(slist_val(slist_first(l)), V(1));
    slist_add_head(l, V(0));
    EXPECT_EQ_PTR(slist_val(slist_first(l)), V(0));
    slist_free(l);
}

UTEST_CASE(slist_last)
{
    struct slist *l = slist_init();

    EXPECT_NULL(slist_last(l));
    slist_add_head(l, V(1));
    EXPECT_EQ_PTR(slist_val(slist_last(l)), V(1));
    slist_add_tail(l, V(2));
    EXPECT_EQ_PTR(slist_val(slist_last(l)), V(2));
    slist_free(l);
}

UTEST_CASE(slist_empty)
{
    struct slist *l = slist_init();

    EXPECT_TRUE(slist_empty(l));
    slist_add_tail(l, V(1));
    EXPECT_FALSE(slist_empty(l));
    slist_del(l, slist_first(l));
    EXPECT_TRUE(slist_empty(l));
    slist_free(l);
}

UTEST_CASE(slist_nextnode)
{
    struct slist *l = slist_init();
    struct slist_node *n;

    slist_add_tail(l, V(1));
    slist_add_tail(l, V(2));
    n = slist_first(l);
    EXPECT_EQ_PTR(slist_val(slist_nextnode(n)), V(2));
    EXPECT_NULL(slist_nextnode(slist_last(l)));
    slist_free(l);
}

UTEST_CASE(slist_val)
{
    struct slist *l = slist_init();

    slist_add_tail(l, V(42));
    EXPECT_EQ_INT(I(slist_val(slist_first(l))), 42);
    slist_free(l);
}

UTEST_CASE(slist_setdup)
{
    struct slist *l = slist_init();

    EXPECT_NULL(slist_getdup(l));
    slist_setdup(l, dup_str);
    EXPECT_EQ_PTR(slist_getdup(l), dup_str);
    slist_free(l);
}

UTEST_CASE(slist_setfree)
{
    struct slist *l = slist_init();

    EXPECT_NULL(slist_getfree(l));
    slist_setfree(l, free_str);
    EXPECT_EQ_PTR(slist_getfree(l), free_str);
    slist_free(l);
}

UTEST_CASE(slist_setmatch)
{
    struct slist *l = slist_init();

    EXPECT_NULL(slist_getmatch(l));
    slist_setmatch(l, match_str);
    EXPECT_EQ_PTR(slist_getmatch(l), match_str);
    slist_free(l);
}

UTEST_CASE(slist_getdup)
{
    struct slist *l = slist_init();

    EXPECT_NULL(slist_getdup(l));
    slist_setdup(l, dup_str);
    EXPECT_EQ_PTR(slist_getdup(l), dup_str);
    slist_free(l);
}

UTEST_CASE(slist_getfree)
{
    struct slist *l = slist_init();

    EXPECT_NULL(slist_getfree(l));
    slist_setfree(l, free_str);
    EXPECT_EQ_PTR(slist_getfree(l), free_str);
    slist_free(l);
}

UTEST_CASE(slist_getmatch)
{
    struct slist *l = slist_init();

    EXPECT_NULL(slist_getmatch(l));
    slist_setmatch(l, match_str);
    EXPECT_EQ_PTR(slist_getmatch(l), match_str);
    slist_free(l);
}

UTEST_CASE(slist_add_head)
{
    struct slist *l = slist_init();
    struct slist_node *n;

    EXPECT_EQ_PTR(slist_add_head(l, V(1)), l);
    EXPECT_EQ_UINT(slist_len(l), 1);
    EXPECT_EQ_PTR(slist_first(l), slist_last(l));
    EXPECT_EQ_PTR(slist_val(slist_first(l)), V(1));
    EXPECT_NULL(slist_nextnode(slist_first(l)));

    EXPECT_EQ_PTR(slist_add_head(l, V(0)), l);
    EXPECT_EQ_UINT(slist_len(l), 2);
    EXPECT_EQ_PTR(slist_val(slist_first(l)), V(0));
    EXPECT_EQ_PTR(slist_val(slist_last(l)), V(1));
    n = slist_first(l);
    EXPECT_EQ_PTR(slist_nextnode(n), slist_last(l));
    slist_free(l);
}

UTEST_CASE(slist_add_tail)
{
    struct slist *l = slist_init();
    struct slist_node *n;

    EXPECT_EQ_PTR(slist_add_tail(l, V(1)), l);
    EXPECT_EQ_UINT(slist_len(l), 1);
    EXPECT_EQ_PTR(slist_first(l), slist_last(l));

    EXPECT_EQ_PTR(slist_add_tail(l, V(2)), l);
    EXPECT_EQ_UINT(slist_len(l), 2);
    EXPECT_EQ_PTR(slist_val(slist_first(l)), V(1));
    EXPECT_EQ_PTR(slist_val(slist_last(l)), V(2));
    n = slist_first(l);
    EXPECT_EQ_PTR(slist_nextnode(n), slist_last(l));
    EXPECT_NULL(slist_nextnode(slist_last(l)));
    slist_free(l);
}

UTEST_CASE(slist_insert)
{
    struct slist *l = slist_init();
    struct slist_node *a, *mid;

    slist_add_tail(l, V(1));
    a = slist_first(l);

    EXPECT_EQ_PTR(slist_insert(l, a, V(2)), l);
    EXPECT_EQ_UINT(slist_len(l), 2);
    EXPECT_EQ_PTR(slist_val(slist_first(l)), V(1));
    EXPECT_EQ_PTR(slist_val(slist_last(l)), V(2));
    EXPECT_EQ_PTR(slist_nextnode(a), slist_last(l));

    EXPECT_EQ_PTR(slist_insert(l, a, V(9)), l);
    EXPECT_EQ_UINT(slist_len(l), 3);
    mid = slist_nextnode(a);
    EXPECT_EQ_PTR(slist_val(mid), V(9));
    EXPECT_EQ_PTR(slist_nextnode(mid), slist_last(l));
    EXPECT_EQ_PTR(slist_val(slist_last(l)), V(2));

    EXPECT_EQ_PTR(slist_insert(l, slist_last(l), V(3)), l);
    EXPECT_EQ_UINT(slist_len(l), 4);
    EXPECT_EQ_PTR(slist_val(slist_first(l)), V(1));
    EXPECT_EQ_PTR(slist_val(slist_index(l, 1)), V(9));
    EXPECT_EQ_PTR(slist_val(slist_index(l, 2)), V(2));
    EXPECT_EQ_PTR(slist_val(slist_last(l)), V(3));
    slist_free(l);
}

UTEST_CASE(slist_del)
{
    struct slist *l = slist_init();
    struct slist_node *n;

    slist_add_tail(l, V(1));
    slist_add_tail(l, V(2));
    slist_add_tail(l, V(3));

    n = slist_first(l);
    slist_del(l, n);
    EXPECT_EQ_UINT(slist_len(l), 2);
    EXPECT_EQ_PTR(slist_val(slist_first(l)), V(2));
    EXPECT_EQ_PTR(slist_val(slist_last(l)), V(3));

    n = slist_nextnode(slist_first(l));
    slist_del(l, n);
    EXPECT_EQ_UINT(slist_len(l), 1);
    EXPECT_EQ_PTR(slist_val(slist_first(l)), V(2));
    EXPECT_EQ_PTR(slist_first(l), slist_last(l));
    EXPECT_NULL(slist_nextnode(slist_first(l)));

    slist_del(l, slist_first(l));
    EXPECT_EQ_UINT(slist_len(l), 0);
    EXPECT_NULL(slist_first(l));
    EXPECT_NULL(slist_last(l));
    slist_free(l);

    l = slist_init();
    slist_add_tail(l, V(1));
    slist_add_tail(l, V(2));
    slist_add_tail(l, V(3));
    slist_del(l, slist_last(l));
    EXPECT_EQ_UINT(slist_len(l), 2);
    EXPECT_EQ_PTR(slist_val(slist_first(l)), V(1));
    EXPECT_EQ_PTR(slist_val(slist_last(l)), V(2));
    EXPECT_NULL(slist_nextnode(slist_last(l)));
    slist_free(l);

    l = slist_init();
    slist_setfree(l, free_str);
    slist_add_tail(l, strdup("x"));
    slist_del(l, slist_first(l));
    EXPECT_EQ_UINT(slist_len(l), 0);
    slist_free(l);
}

UTEST_CASE(slist_dup)
{
    struct slist *l, *c;
    char *s;

    l = slist_init();
    slist_add_tail(l, V(1));
    slist_add_tail(l, V(2));
    c = slist_dup(l);
    EXPECT_NOTNULL(c);
    EXPECT_NE_PTR(c, l);
    EXPECT_EQ_UINT(slist_len(c), 2);
    EXPECT_EQ_PTR(slist_val(slist_first(c)), V(1));
    EXPECT_EQ_PTR(slist_val(slist_last(c)), V(2));
    EXPECT_NE_PTR(slist_first(c), slist_first(l));
    slist_del(c, slist_first(c));
    EXPECT_EQ_UINT(slist_len(l), 2);
    slist_free(c);
    slist_free(l);

    l = slist_init();
    slist_setdup(l, dup_str);
    slist_setfree(l, free_str);
    slist_setmatch(l, match_str);
    s = strdup("hi");
    slist_add_tail(l, s);
    c = slist_dup(l);
    EXPECT_NOTNULL(c);
    EXPECT_NE_PTR(slist_val(slist_first(c)), s);
    EXPECT_EQ_STR((char *)slist_val(slist_first(c)), "hi");
    EXPECT_EQ_PTR(slist_getdup(c), dup_str);
    EXPECT_EQ_PTR(slist_getfree(c), free_str);
    EXPECT_EQ_PTR(slist_getmatch(c), match_str);
    slist_free(c);
    slist_free(l);

    l = slist_init();
    c = slist_dup(l);
    EXPECT_NOTNULL(c);
    EXPECT_EQ_UINT(slist_len(c), 0);
    slist_free(c);
    slist_free(l);

    l = slist_init();
    slist_setdup(l, dup_fail);
    slist_setfree(l, free_str);
    slist_add_tail(l, strdup("keep"));
    c = slist_dup(l);
    EXPECT_NULL(c);
    EXPECT_EQ_UINT(slist_len(l), 1);
    slist_free(l);
}

UTEST_CASE(slist_search)
{
    struct slist *l = slist_init();
    struct slist_node *n;

    slist_add_tail(l, V(1));
    slist_add_tail(l, V(2));
    slist_add_tail(l, V(2));
    slist_add_tail(l, V(3));
    n = slist_search(l, V(2));
    EXPECT_NOTNULL(n);
    EXPECT_EQ_PTR(slist_val(n), V(2));
    EXPECT_EQ_PTR(n, slist_nextnode(slist_first(l)));
    EXPECT_NULL(slist_search(l, V(9)));
    slist_free(l);

    l = slist_init();
    slist_setmatch(l, match_str);
    slist_setfree(l, free_str);
    slist_add_tail(l, strdup("aa"));
    slist_add_tail(l, strdup("bb"));
    n = slist_search(l, "bb");
    EXPECT_NOTNULL(n);
    EXPECT_EQ_STR((char *)slist_val(n), "bb");
    EXPECT_NULL(slist_search(l, "cc"));
    slist_free(l);
}

UTEST_CASE(slist_index)
{
    struct slist *l = slist_init();
    struct slist_node *n;

    EXPECT_NULL(slist_index(l, 0));
    slist_add_tail(l, V(10));
    slist_add_tail(l, V(20));
    slist_add_tail(l, V(30));
    n = slist_index(l, 0);
    EXPECT_EQ_PTR(n, slist_first(l));
    EXPECT_EQ_PTR(slist_val(n), V(10));
    n = slist_index(l, 1);
    EXPECT_EQ_PTR(slist_val(n), V(20));
    n = slist_index(l, 2);
    EXPECT_EQ_PTR(n, slist_last(l));
    EXPECT_EQ_PTR(slist_val(n), V(30));
    EXPECT_NULL(slist_index(l, 3));
    EXPECT_NULL(slist_index(l, 100));
    slist_free(l);
}

UTEST_SUITE(slist)
{
    UTEST_RUNCASE(slist_init);
    UTEST_RUNCASE(slist_free);
    UTEST_RUNCASE(slist_clear);
    UTEST_RUNCASE(slist_len);
    UTEST_RUNCASE(slist_first);
    UTEST_RUNCASE(slist_last);
    UTEST_RUNCASE(slist_empty);
    UTEST_RUNCASE(slist_nextnode);
    UTEST_RUNCASE(slist_val);
    UTEST_RUNCASE(slist_setdup);
    UTEST_RUNCASE(slist_setfree);
    UTEST_RUNCASE(slist_setmatch);
    UTEST_RUNCASE(slist_getdup);
    UTEST_RUNCASE(slist_getfree);
    UTEST_RUNCASE(slist_getmatch);
    UTEST_RUNCASE(slist_add_head);
    UTEST_RUNCASE(slist_add_tail);
    UTEST_RUNCASE(slist_insert);
    UTEST_RUNCASE(slist_del);
    UTEST_RUNCASE(slist_dup);
    UTEST_RUNCASE(slist_search);
    UTEST_RUNCASE(slist_index);
}
