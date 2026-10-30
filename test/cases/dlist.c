/* dlist test suite */

#include <dlist.h>
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

UTEST_CASE(dlist_init)
{
    struct dlist *l = dlist_init();

    EXPECT_NOTNULL(l);
    EXPECT_NULL(dlist_first(l));
    EXPECT_NULL(dlist_last(l));
    EXPECT_EQ_UINT(dlist_len(l), 0);
    EXPECT_NULL(dlist_getdup(l));
    EXPECT_NULL(dlist_getfree(l));
    EXPECT_NULL(dlist_getmatch(l));
    dlist_free(l);
}

UTEST_CASE(dlist_free)
{
    struct dlist *l;

    l = dlist_init();
    EXPECT_NOTNULL(dlist_add_tail(l, V(1)));
    dlist_free(l);

    l = dlist_init();
    dlist_free(l);

    dlist_free(NULL);
}

UTEST_CASE(dlist_clear)
{
    struct dlist *l = dlist_init();
    char *a = strdup("a"), *b = strdup("b");

    dlist_setfree(l, free_str);
    EXPECT_NOTNULL(dlist_add_tail(l, a));
    EXPECT_NOTNULL(dlist_add_tail(l, b));
    dlist_clear(l);
    EXPECT_EQ_UINT(dlist_len(l), 0);
    EXPECT_NULL(dlist_first(l));
    EXPECT_NULL(dlist_last(l));
    EXPECT_EQ_PTR(dlist_getfree(l), free_str);

    EXPECT_NOTNULL(dlist_add_tail(l, strdup("c")));
    EXPECT_EQ_UINT(dlist_len(l), 1);
    dlist_clear(l);
    EXPECT_EQ_UINT(dlist_len(l), 0);
    dlist_free(l);
}

UTEST_CASE(dlist_len)
{
    struct dlist *l = dlist_init();

    EXPECT_EQ_UINT(dlist_len(l), 0);
    dlist_add_head(l, V(1));
    EXPECT_EQ_UINT(dlist_len(l), 1);
    dlist_add_tail(l, V(2));
    EXPECT_EQ_UINT(dlist_len(l), 2);
    dlist_del(l, dlist_first(l));
    EXPECT_EQ_UINT(dlist_len(l), 1);
    dlist_free(l);
}

UTEST_CASE(dlist_first)
{
    struct dlist *l = dlist_init();

    EXPECT_NULL(dlist_first(l));
    dlist_add_tail(l, V(1));
    dlist_add_tail(l, V(2));
    EXPECT_EQ_PTR(dlist_nodevalue(dlist_first(l)), V(1));
    dlist_add_head(l, V(0));
    EXPECT_EQ_PTR(dlist_nodevalue(dlist_first(l)), V(0));
    dlist_free(l);
}

UTEST_CASE(dlist_last)
{
    struct dlist *l = dlist_init();

    EXPECT_NULL(dlist_last(l));
    dlist_add_head(l, V(1));
    EXPECT_EQ_PTR(dlist_nodevalue(dlist_last(l)), V(1));
    dlist_add_tail(l, V(2));
    EXPECT_EQ_PTR(dlist_nodevalue(dlist_last(l)), V(2));
    dlist_free(l);
}

UTEST_CASE(dlist_empty)
{
    struct dlist *l = dlist_init();

    EXPECT_TRUE(dlist_empty(l));
    dlist_add_tail(l, V(1));
    EXPECT_FALSE(dlist_empty(l));
    dlist_del(l, dlist_first(l));
    EXPECT_TRUE(dlist_empty(l));
    dlist_free(l);
}

UTEST_CASE(dlist_prevnode)
{
    struct dlist *l = dlist_init();
    struct dlist_node *n;

    dlist_add_tail(l, V(1));
    dlist_add_tail(l, V(2));
    n = dlist_last(l);
    EXPECT_EQ_PTR(dlist_nodevalue(dlist_prevnode(n)), V(1));
    EXPECT_NULL(dlist_prevnode(dlist_first(l)));
    dlist_free(l);
}

UTEST_CASE(dlist_nextnode)
{
    struct dlist *l = dlist_init();
    struct dlist_node *n;

    dlist_add_tail(l, V(1));
    dlist_add_tail(l, V(2));
    n = dlist_first(l);
    EXPECT_EQ_PTR(dlist_nodevalue(dlist_nextnode(n)), V(2));
    EXPECT_NULL(dlist_nextnode(dlist_last(l)));
    dlist_free(l);
}

UTEST_CASE(dlist_nodevalue)
{
    struct dlist *l = dlist_init();

    dlist_add_tail(l, V(42));
    EXPECT_EQ_INT(I(dlist_nodevalue(dlist_first(l))), 42);
    dlist_free(l);
}

UTEST_CASE(dlist_setdup)
{
    struct dlist *l = dlist_init();

    EXPECT_NULL(dlist_getdup(l));
    dlist_setdup(l, dup_str);
    EXPECT_EQ_PTR(dlist_getdup(l), dup_str);
    dlist_free(l);
}

UTEST_CASE(dlist_setfree)
{
    struct dlist *l = dlist_init();

    EXPECT_NULL(dlist_getfree(l));
    dlist_setfree(l, free_str);
    EXPECT_EQ_PTR(dlist_getfree(l), free_str);
    dlist_free(l);
}

UTEST_CASE(dlist_setmatch)
{
    struct dlist *l = dlist_init();

    EXPECT_NULL(dlist_getmatch(l));
    dlist_setmatch(l, match_str);
    EXPECT_EQ_PTR(dlist_getmatch(l), match_str);
    dlist_free(l);
}

UTEST_CASE(dlist_getdup)
{
    struct dlist *l = dlist_init();

    EXPECT_NULL(dlist_getdup(l));
    dlist_setdup(l, dup_str);
    EXPECT_EQ_PTR(dlist_getdup(l), dup_str);
    dlist_free(l);
}

UTEST_CASE(dlist_getfree)
{
    struct dlist *l = dlist_init();

    EXPECT_NULL(dlist_getfree(l));
    dlist_setfree(l, free_str);
    EXPECT_EQ_PTR(dlist_getfree(l), free_str);
    dlist_free(l);
}

UTEST_CASE(dlist_getmatch)
{
    struct dlist *l = dlist_init();

    EXPECT_NULL(dlist_getmatch(l));
    dlist_setmatch(l, match_str);
    EXPECT_EQ_PTR(dlist_getmatch(l), match_str);
    dlist_free(l);
}

UTEST_CASE(dlist_add_head)
{
    struct dlist *l = dlist_init();
    struct dlist_node *n;

    EXPECT_EQ_PTR(dlist_add_head(l, V(1)), l);
    EXPECT_EQ_UINT(dlist_len(l), 1);
    EXPECT_EQ_PTR(dlist_first(l), dlist_last(l));
    EXPECT_EQ_PTR(dlist_nodevalue(dlist_first(l)), V(1));
    EXPECT_NULL(dlist_prevnode(dlist_first(l)));
    EXPECT_NULL(dlist_nextnode(dlist_first(l)));

    EXPECT_EQ_PTR(dlist_add_head(l, V(0)), l);
    EXPECT_EQ_UINT(dlist_len(l), 2);
    EXPECT_EQ_PTR(dlist_nodevalue(dlist_first(l)), V(0));
    EXPECT_EQ_PTR(dlist_nodevalue(dlist_last(l)), V(1));
    n = dlist_first(l);
    EXPECT_EQ_PTR(dlist_nextnode(n), dlist_last(l));
    EXPECT_EQ_PTR(dlist_prevnode(dlist_last(l)), n);
    dlist_free(l);
}

UTEST_CASE(dlist_add_tail)
{
    struct dlist *l = dlist_init();
    struct dlist_node *n;

    EXPECT_EQ_PTR(dlist_add_tail(l, V(1)), l);
    EXPECT_EQ_UINT(dlist_len(l), 1);
    EXPECT_EQ_PTR(dlist_first(l), dlist_last(l));

    EXPECT_EQ_PTR(dlist_add_tail(l, V(2)), l);
    EXPECT_EQ_UINT(dlist_len(l), 2);
    EXPECT_EQ_PTR(dlist_nodevalue(dlist_first(l)), V(1));
    EXPECT_EQ_PTR(dlist_nodevalue(dlist_last(l)), V(2));
    n = dlist_first(l);
    EXPECT_EQ_PTR(dlist_nextnode(n), dlist_last(l));
    EXPECT_EQ_PTR(dlist_prevnode(dlist_last(l)), n);
    EXPECT_NULL(dlist_nextnode(dlist_last(l)));
    dlist_free(l);
}

UTEST_CASE(dlist_insert)
{
    struct dlist *l = dlist_init();
    struct dlist_node *a, *b, *c;

    dlist_add_tail(l, V(1));
    a = dlist_first(l);

    EXPECT_EQ_PTR(dlist_insert(l, a, V(2), 1), l);
    EXPECT_EQ_UINT(dlist_len(l), 2);
    EXPECT_EQ_PTR(dlist_nodevalue(dlist_first(l)), V(1));
    EXPECT_EQ_PTR(dlist_nodevalue(dlist_last(l)), V(2));
    EXPECT_EQ_PTR(dlist_nextnode(a), dlist_last(l));

    EXPECT_EQ_PTR(dlist_insert(l, a, V(0), 0), l);
    EXPECT_EQ_UINT(dlist_len(l), 3);
    EXPECT_EQ_PTR(dlist_nodevalue(dlist_first(l)), V(0));
    EXPECT_EQ_PTR(dlist_prevnode(a), dlist_first(l));

    b = dlist_nextnode(a);
    EXPECT_EQ_PTR(dlist_insert(l, a, V(9), 1), l);
    EXPECT_EQ_UINT(dlist_len(l), 4);
    c = dlist_nextnode(a);
    EXPECT_EQ_PTR(dlist_nodevalue(c), V(9));
    EXPECT_EQ_PTR(dlist_nextnode(c), b);
    EXPECT_EQ_PTR(dlist_prevnode(b), c);
    EXPECT_EQ_PTR(dlist_nodevalue(dlist_first(l)), V(0));
    EXPECT_EQ_PTR(dlist_nodevalue(dlist_last(l)), V(2));
    dlist_free(l);
}

UTEST_CASE(dlist_del)
{
    struct dlist *l = dlist_init();
    struct dlist_node *n;

    dlist_add_tail(l, V(1));
    dlist_add_tail(l, V(2));
    dlist_add_tail(l, V(3));

    n = dlist_first(l);
    dlist_del(l, n);
    EXPECT_EQ_UINT(dlist_len(l), 2);
    EXPECT_EQ_PTR(dlist_nodevalue(dlist_first(l)), V(2));
    EXPECT_NULL(dlist_prevnode(dlist_first(l)));

    n = dlist_last(l);
    dlist_del(l, n);
    EXPECT_EQ_UINT(dlist_len(l), 1);
    EXPECT_EQ_PTR(dlist_nodevalue(dlist_last(l)), V(2));
    EXPECT_EQ_PTR(dlist_first(l), dlist_last(l));
    EXPECT_NULL(dlist_nextnode(dlist_first(l)));

    dlist_del(l, dlist_first(l));
    EXPECT_EQ_UINT(dlist_len(l), 0);
    EXPECT_NULL(dlist_first(l));
    EXPECT_NULL(dlist_last(l));
    dlist_free(l);

    l = dlist_init();
    dlist_setfree(l, free_str);
    dlist_add_tail(l, strdup("x"));
    dlist_del(l, dlist_first(l));
    EXPECT_EQ_UINT(dlist_len(l), 0);
    dlist_free(l);
}

UTEST_CASE(dlist_dup)
{
    struct dlist *l, *c;
    char *s;

    l = dlist_init();
    dlist_add_tail(l, V(1));
    dlist_add_tail(l, V(2));
    c = dlist_dup(l);
    EXPECT_NOTNULL(c);
    EXPECT_NE_PTR(c, l);
    EXPECT_EQ_UINT(dlist_len(c), 2);
    EXPECT_EQ_PTR(dlist_nodevalue(dlist_first(c)), V(1));
    EXPECT_EQ_PTR(dlist_nodevalue(dlist_last(c)), V(2));
    EXPECT_NE_PTR(dlist_first(c), dlist_first(l));
    dlist_del(c, dlist_first(c));
    EXPECT_EQ_UINT(dlist_len(l), 2);
    dlist_free(c);
    dlist_free(l);

    l = dlist_init();
    dlist_setdup(l, dup_str);
    dlist_setfree(l, free_str);
    s = strdup("hi");
    dlist_add_tail(l, s);
    c = dlist_dup(l);
    EXPECT_NOTNULL(c);
    EXPECT_NE_PTR(dlist_nodevalue(dlist_first(c)), s);
    EXPECT_EQ_STR((char *)dlist_nodevalue(dlist_first(c)), "hi");
    EXPECT_EQ_PTR(dlist_getdup(c), dup_str);
    EXPECT_EQ_PTR(dlist_getfree(c), free_str);
    dlist_free(c);
    dlist_free(l);

    l = dlist_init();
    c = dlist_dup(l);
    EXPECT_NOTNULL(c);
    EXPECT_EQ_UINT(dlist_len(c), 0);
    dlist_free(c);
    dlist_free(l);
}

UTEST_CASE(dlist_search)
{
    struct dlist *l = dlist_init();
    struct dlist_node *n;

    dlist_add_tail(l, V(1));
    dlist_add_tail(l, V(2));
    dlist_add_tail(l, V(3));
    n = dlist_search(l, V(2));
    EXPECT_NOTNULL(n);
    EXPECT_EQ_PTR(dlist_nodevalue(n), V(2));
    EXPECT_NULL(dlist_search(l, V(9)));
    dlist_free(l);

    l = dlist_init();
    dlist_setmatch(l, match_str);
    dlist_setfree(l, free_str);
    dlist_add_tail(l, strdup("aa"));
    dlist_add_tail(l, strdup("bb"));
    n = dlist_search(l, "bb");
    EXPECT_NOTNULL(n);
    EXPECT_EQ_STR((char *)dlist_nodevalue(n), "bb");
    EXPECT_NULL(dlist_search(l, "cc"));
    dlist_free(l);
}

UTEST_CASE(dlist_index)
{
    struct dlist *l = dlist_init();
    struct dlist_node *n;

    EXPECT_NULL(dlist_index(l, 0));
    dlist_add_tail(l, V(10));
    dlist_add_tail(l, V(20));
    dlist_add_tail(l, V(30));
    n = dlist_index(l, 0);
    EXPECT_EQ_PTR(n, dlist_first(l));
    EXPECT_EQ_PTR(dlist_nodevalue(n), V(10));
    n = dlist_index(l, 1);
    EXPECT_EQ_PTR(dlist_nodevalue(n), V(20));
    n = dlist_index(l, 2);
    EXPECT_EQ_PTR(n, dlist_last(l));
    EXPECT_EQ_PTR(dlist_nodevalue(n), V(30));
    EXPECT_NULL(dlist_index(l, 3));
    EXPECT_NULL(dlist_index(l, 100));
    dlist_free(l);
}

UTEST_SUITE(dlist)
{
    UTEST_RUNCASE(dlist_init);
    UTEST_RUNCASE(dlist_free);
    UTEST_RUNCASE(dlist_clear);
    UTEST_RUNCASE(dlist_len);
    UTEST_RUNCASE(dlist_first);
    UTEST_RUNCASE(dlist_last);
    UTEST_RUNCASE(dlist_empty);
    UTEST_RUNCASE(dlist_prevnode);
    UTEST_RUNCASE(dlist_nextnode);
    UTEST_RUNCASE(dlist_nodevalue);
    UTEST_RUNCASE(dlist_setdup);
    UTEST_RUNCASE(dlist_setfree);
    UTEST_RUNCASE(dlist_setmatch);
    UTEST_RUNCASE(dlist_getdup);
    UTEST_RUNCASE(dlist_getfree);
    UTEST_RUNCASE(dlist_getmatch);
    UTEST_RUNCASE(dlist_add_head);
    UTEST_RUNCASE(dlist_add_tail);
    UTEST_RUNCASE(dlist_insert);
    UTEST_RUNCASE(dlist_del);
    UTEST_RUNCASE(dlist_dup);
    UTEST_RUNCASE(dlist_search);
    UTEST_RUNCASE(dlist_index);
}
