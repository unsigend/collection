/* Tests for dlist. */

#include <dlist.h>
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <utest.h>

static void *int_dup(void *p)
{
    int *n;

    n = malloc(sizeof(*n));
    if (n == NULL)
        return NULL;
    *n = *(int *)p;
    return n;
}

static void int_free(void *p) { free(p); }

static bool int_match(void *p, void *key)
{
    return *(int *)p == *(int *)key;
}

static void *ip(int x) { return (void *)(intptr_t)x; }

UTEST_CASE(dlist_len)
{
    struct dlist l;

    dlist_init(&l);
    EXPECT_EQ_UINT(dlist_len(&l), 0);
    EXPECT_EQ_INT(dlist_add_tail(&l, ip(1)), 0);
    EXPECT_EQ_UINT(dlist_len(&l), 1);
    EXPECT_EQ_INT(dlist_add_tail(&l, ip(2)), 0);
    EXPECT_EQ_INT(dlist_add_head(&l, ip(0)), 0);
    EXPECT_EQ_UINT(dlist_len(&l), 3);
    EXPECT_EQ_INT(dlist_del_head(&l, NULL), 0);
    EXPECT_EQ_UINT(dlist_len(&l), 2);
    dlist_free(&l);
}

UTEST_CASE(dlist_empty)
{
    struct dlist l;

    dlist_init(&l);
    EXPECT_TRUE(dlist_empty(&l));
    EXPECT_EQ_INT(dlist_add_head(&l, ip(1)), 0);
    EXPECT_FALSE(dlist_empty(&l));
    EXPECT_EQ_INT(dlist_del_tail(&l, NULL), 0);
    EXPECT_TRUE(dlist_empty(&l));
    dlist_free(&l);
}

UTEST_CASE(dlist_front)
{
    struct dlist l;

    dlist_init(&l);
    EXPECT_NULL(dlist_front(&l));
    EXPECT_EQ_INT(dlist_add_tail(&l, ip(1)), 0);
    EXPECT_EQ_INT(dlist_add_tail(&l, ip(2)), 0);
    EXPECT_EQ_PTR(dlist_nodeval(dlist_front(&l)), ip(1));
    EXPECT_EQ_PTR(dlist_front(&l), dlist_at(&l, 0));
    dlist_free(&l);

    dlist_init(&l);
    EXPECT_EQ_INT(dlist_add_head(&l, ip(7)), 0);
    EXPECT_EQ_PTR(dlist_front(&l), dlist_back(&l));
    EXPECT_NULL(dlist_prevnode(dlist_front(&l)));
    EXPECT_NULL(dlist_nextnode(dlist_front(&l)));
    dlist_free(&l);
}

UTEST_CASE(dlist_back)
{
    struct dlist l;

    dlist_init(&l);
    EXPECT_NULL(dlist_back(&l));
    EXPECT_EQ_INT(dlist_add_tail(&l, ip(1)), 0);
    EXPECT_EQ_INT(dlist_add_tail(&l, ip(2)), 0);
    EXPECT_EQ_PTR(dlist_nodeval(dlist_back(&l)), ip(2));
    EXPECT_EQ_PTR(dlist_back(&l), dlist_at(&l, 1));
    dlist_free(&l);

    dlist_init(&l);
    EXPECT_EQ_INT(dlist_add_tail(&l, ip(9)), 0);
    EXPECT_EQ_PTR(dlist_back(&l), dlist_front(&l));
    EXPECT_EQ_PTR(dlist_nodeval(dlist_back(&l)), ip(9));
    dlist_free(&l);
}

UTEST_CASE(dlist_prevnode)
{
    struct dlist l;

    dlist_init(&l);
    EXPECT_EQ_INT(dlist_add_tail(&l, ip(1)), 0);
    EXPECT_EQ_INT(dlist_add_tail(&l, ip(2)), 0);
    EXPECT_NULL(dlist_prevnode(dlist_front(&l)));
    EXPECT_EQ_PTR(dlist_nodeval(dlist_prevnode(dlist_back(&l))), ip(1));
    dlist_free(&l);

    dlist_init(&l);
    EXPECT_EQ_INT(dlist_add_tail(&l, ip(1)), 0);
    EXPECT_EQ_INT(dlist_add_tail(&l, ip(2)), 0);
    EXPECT_EQ_INT(dlist_add_tail(&l, ip(3)), 0);
    EXPECT_EQ_PTR(dlist_prevnode(dlist_at(&l, 1)), dlist_front(&l));
    EXPECT_EQ_PTR(dlist_prevnode(dlist_back(&l)), dlist_at(&l, 1));
    EXPECT_NULL(dlist_prevnode(dlist_at(&l, 0)));
    dlist_free(&l);
}

UTEST_CASE(dlist_nextnode)
{
    struct dlist l;

    dlist_init(&l);
    EXPECT_EQ_INT(dlist_add_tail(&l, ip(1)), 0);
    EXPECT_EQ_INT(dlist_add_tail(&l, ip(2)), 0);
    EXPECT_NULL(dlist_nextnode(dlist_back(&l)));
    EXPECT_EQ_PTR(dlist_nodeval(dlist_nextnode(dlist_front(&l))), ip(2));
    dlist_free(&l);

    dlist_init(&l);
    EXPECT_EQ_INT(dlist_add_tail(&l, ip(1)), 0);
    EXPECT_EQ_INT(dlist_add_tail(&l, ip(2)), 0);
    EXPECT_EQ_INT(dlist_add_tail(&l, ip(3)), 0);
    EXPECT_EQ_PTR(dlist_nextnode(dlist_front(&l)), dlist_at(&l, 1));
    EXPECT_EQ_PTR(dlist_nextnode(dlist_at(&l, 1)), dlist_back(&l));
    EXPECT_NULL(dlist_nextnode(dlist_back(&l)));
    dlist_free(&l);
}

UTEST_CASE(dlist_nodeval)
{
    struct dlist l;

    dlist_init(&l);
    EXPECT_EQ_INT(dlist_add_head(&l, ip(7)), 0);
    EXPECT_EQ_PTR(dlist_nodeval(dlist_front(&l)), ip(7));
    dlist_free(&l);
}

UTEST_CASE(dlist_setdup)
{
    struct dlist l;

    dlist_init(&l);
    dlist_setdup(&l, int_dup);
    EXPECT_EQ_PTR(dlist_getdup(&l), int_dup);
    dlist_free(&l);
}

UTEST_CASE(dlist_setfree)
{
    struct dlist l;

    dlist_init(&l);
    dlist_setfree(&l, int_free);
    EXPECT_EQ_PTR(dlist_getfree(&l), int_free);
    dlist_free(&l);
}

UTEST_CASE(dlist_setmatch)
{
    struct dlist l;

    dlist_init(&l);
    dlist_setmatch(&l, int_match);
    EXPECT_EQ_PTR(dlist_getmatch(&l), int_match);
    dlist_free(&l);
}

UTEST_CASE(dlist_getdup)
{
    struct dlist l;

    dlist_init(&l);
    EXPECT_NULL(dlist_getdup(&l));
    dlist_setdup(&l, int_dup);
    EXPECT_EQ_PTR(dlist_getdup(&l), int_dup);
    dlist_free(&l);
}

UTEST_CASE(dlist_getfree)
{
    struct dlist l;

    dlist_init(&l);
    EXPECT_NULL(dlist_getfree(&l));
    dlist_setfree(&l, int_free);
    EXPECT_EQ_PTR(dlist_getfree(&l), int_free);
    dlist_free(&l);
}

UTEST_CASE(dlist_getmatch)
{
    struct dlist l;

    dlist_init(&l);
    EXPECT_NULL(dlist_getmatch(&l));
    dlist_setmatch(&l, int_match);
    EXPECT_EQ_PTR(dlist_getmatch(&l), int_match);
    dlist_free(&l);
}

UTEST_CASE(dlist_init)
{
    struct dlist l;

    dlist_init(&l);
    EXPECT_TRUE(dlist_empty(&l));
    EXPECT_NULL(dlist_front(&l));
    EXPECT_NULL(dlist_back(&l));
    EXPECT_NULL(dlist_getdup(&l));
    EXPECT_NULL(dlist_getfree(&l));
    EXPECT_NULL(dlist_getmatch(&l));
    dlist_free(&l);
}

UTEST_CASE(dlist_free)
{
    struct dlist l;

    dlist_free(NULL);
    dlist_init(&l);
    EXPECT_EQ_INT(dlist_add_tail(&l, ip(1)), 0);
    dlist_free(&l);
    EXPECT_TRUE(dlist_empty(&l));
    EXPECT_NULL(dlist_front(&l));
}

UTEST_CASE(dlist_clear)
{
    struct dlist l;
    int *a, *b;

    dlist_init(&l);
    dlist_setfree(&l, int_free);
    a = malloc(sizeof(*a));
    b = malloc(sizeof(*b));
    EXPECT_NOTNULL(a);
    EXPECT_NOTNULL(b);
    *a = 1;
    *b = 2;
    EXPECT_EQ_INT(dlist_add_tail(&l, a), 0);
    EXPECT_EQ_INT(dlist_add_tail(&l, b), 0);
    dlist_clear(&l);
    EXPECT_TRUE(dlist_empty(&l));
    EXPECT_NULL(dlist_front(&l));
    EXPECT_NULL(dlist_back(&l));
    EXPECT_EQ_PTR(dlist_getfree(&l), int_free);
    dlist_free(&l);

    dlist_init(&l);
    dlist_setdup(&l, int_dup);
    dlist_setfree(&l, int_free);
    dlist_setmatch(&l, int_match);
    dlist_clear(&l);
    EXPECT_TRUE(dlist_empty(&l));
    EXPECT_NULL(dlist_front(&l));
    EXPECT_EQ_PTR(dlist_getdup(&l), int_dup);
    EXPECT_EQ_PTR(dlist_getfree(&l), int_free);
    EXPECT_EQ_PTR(dlist_getmatch(&l), int_match);
    dlist_free(&l);
}

UTEST_CASE(dlist_add_head)
{
    struct dlist l;

    dlist_init(&l);
    EXPECT_EQ_INT(dlist_add_head(&l, ip(1)), 0);
    EXPECT_EQ_INT(dlist_add_head(&l, ip(2)), 0);
    EXPECT_EQ_UINT(dlist_len(&l), 2);
    EXPECT_EQ_PTR(dlist_nodeval(dlist_front(&l)), ip(2));
    EXPECT_EQ_PTR(dlist_nodeval(dlist_back(&l)), ip(1));
    EXPECT_EQ_PTR(dlist_nextnode(dlist_front(&l)), dlist_back(&l));
    EXPECT_EQ_PTR(dlist_prevnode(dlist_back(&l)), dlist_front(&l));
    dlist_free(&l);

    dlist_init(&l);
    EXPECT_EQ_INT(dlist_add_head(&l, ip(1)), 0);
    EXPECT_EQ_PTR(dlist_front(&l), dlist_back(&l));
    EXPECT_EQ_INT(dlist_add_head(&l, ip(2)), 0);
    EXPECT_EQ_INT(dlist_add_head(&l, ip(3)), 0);
    EXPECT_EQ_PTR(dlist_nodeval(dlist_at(&l, 0)), ip(3));
    EXPECT_EQ_PTR(dlist_nodeval(dlist_at(&l, 1)), ip(2));
    EXPECT_EQ_PTR(dlist_nodeval(dlist_at(&l, 2)), ip(1));
    EXPECT_EQ_PTR(dlist_nextnode(dlist_at(&l, 0)), dlist_at(&l, 1));
    EXPECT_EQ_PTR(dlist_prevnode(dlist_at(&l, 2)), dlist_at(&l, 1));
    dlist_free(&l);
}

UTEST_CASE(dlist_add_tail)
{
    struct dlist l;

    dlist_init(&l);
    EXPECT_EQ_INT(dlist_add_tail(&l, ip(1)), 0);
    EXPECT_EQ_INT(dlist_add_tail(&l, ip(2)), 0);
    EXPECT_EQ_UINT(dlist_len(&l), 2);
    EXPECT_EQ_PTR(dlist_nodeval(dlist_front(&l)), ip(1));
    EXPECT_EQ_PTR(dlist_nodeval(dlist_back(&l)), ip(2));
    EXPECT_EQ_PTR(dlist_nextnode(dlist_front(&l)), dlist_back(&l));
    EXPECT_NULL(dlist_nextnode(dlist_back(&l)));
    dlist_free(&l);

    dlist_init(&l);
    EXPECT_EQ_INT(dlist_add_head(&l, ip(4)), 0);
    EXPECT_EQ_INT(dlist_add_tail(&l, ip(5)), 0);
    EXPECT_EQ_INT(dlist_add_tail(&l, ip(6)), 0);
    EXPECT_EQ_PTR(dlist_nodeval(dlist_front(&l)), ip(4));
    EXPECT_EQ_PTR(dlist_nodeval(dlist_back(&l)), ip(6));
    EXPECT_EQ_PTR(dlist_nodeval(dlist_at(&l, 1)), ip(5));
    dlist_free(&l);
}

UTEST_CASE(dlist_del_head)
{
    struct dlist l;
    void *out;

    dlist_init(&l);
    EXPECT_EQ_INT(dlist_del_head(&l, &out), -ENOENT);
    EXPECT_EQ_INT(dlist_add_tail(&l, ip(1)), 0);
    EXPECT_EQ_INT(dlist_add_tail(&l, ip(2)), 0);
    EXPECT_EQ_INT(dlist_del_head(&l, &out), 0);
    EXPECT_EQ_PTR(out, ip(1));
    EXPECT_EQ_UINT(dlist_len(&l), 1);
    EXPECT_EQ_PTR(dlist_nodeval(dlist_front(&l)), ip(2));
    EXPECT_EQ_INT(dlist_del_head(&l, NULL), 0);
    EXPECT_TRUE(dlist_empty(&l));
    EXPECT_NULL(dlist_back(&l));
    EXPECT_NULL(dlist_front(&l));
    EXPECT_EQ_INT(dlist_del_head(&l, &out), -ENOENT);
    dlist_free(&l);

    dlist_init(&l);
    EXPECT_EQ_INT(dlist_add_head(&l, ip(4)), 0);
    EXPECT_EQ_INT(dlist_del_head(&l, &out), 0);
    EXPECT_EQ_PTR(out, ip(4));
    EXPECT_TRUE(dlist_empty(&l));
    EXPECT_NULL(dlist_front(&l));
    EXPECT_NULL(dlist_back(&l));
    dlist_free(&l);
}

UTEST_CASE(dlist_del_tail)
{
    struct dlist l;
    void *out;

    dlist_init(&l);
    EXPECT_EQ_INT(dlist_del_tail(&l, &out), -ENOENT);
    EXPECT_EQ_INT(dlist_add_tail(&l, ip(1)), 0);
    EXPECT_EQ_INT(dlist_add_tail(&l, ip(2)), 0);
    EXPECT_EQ_INT(dlist_del_tail(&l, &out), 0);
    EXPECT_EQ_PTR(out, ip(2));
    EXPECT_EQ_UINT(dlist_len(&l), 1);
    EXPECT_EQ_PTR(dlist_nodeval(dlist_back(&l)), ip(1));
    EXPECT_EQ_INT(dlist_del_tail(&l, NULL), 0);
    EXPECT_TRUE(dlist_empty(&l));
    EXPECT_NULL(dlist_front(&l));
    EXPECT_NULL(dlist_back(&l));
    EXPECT_EQ_INT(dlist_del_tail(&l, &out), -ENOENT);
    dlist_free(&l);

    dlist_init(&l);
    EXPECT_EQ_INT(dlist_add_tail(&l, ip(8)), 0);
    EXPECT_EQ_INT(dlist_del_tail(&l, &out), 0);
    EXPECT_EQ_PTR(out, ip(8));
    EXPECT_EQ_UINT(dlist_len(&l), 0);
    dlist_free(&l);
}

UTEST_CASE(dlist_ins)
{
    struct dlist l;

    dlist_init(&l);
    EXPECT_EQ_INT(dlist_add_tail(&l, ip(1)), 0);
    EXPECT_EQ_INT(dlist_add_tail(&l, ip(3)), 0);
    EXPECT_EQ_INT(dlist_ins(&l, dlist_front(&l), ip(2), 1), 0);
    EXPECT_EQ_UINT(dlist_len(&l), 3);
    EXPECT_EQ_PTR(dlist_nodeval(dlist_at(&l, 0)), ip(1));
    EXPECT_EQ_PTR(dlist_nodeval(dlist_at(&l, 1)), ip(2));
    EXPECT_EQ_PTR(dlist_nodeval(dlist_at(&l, 2)), ip(3));
    EXPECT_EQ_INT(dlist_ins(&l, dlist_front(&l), ip(0), 0), 0);
    EXPECT_EQ_PTR(dlist_nodeval(dlist_front(&l)), ip(0));
    EXPECT_EQ_INT(dlist_ins(&l, dlist_back(&l), ip(4), 1), 0);
    EXPECT_EQ_PTR(dlist_nodeval(dlist_back(&l)), ip(4));
    EXPECT_EQ_PTR(dlist_prevnode(dlist_back(&l)), dlist_at(&l, dlist_len(&l) - 2));
    EXPECT_EQ_PTR(dlist_nextnode(dlist_at(&l, 0)), dlist_at(&l, 1));
    dlist_free(&l);

    dlist_init(&l);
    EXPECT_EQ_INT(dlist_add_tail(&l, ip(1)), 0);
    EXPECT_EQ_INT(dlist_ins(&l, dlist_front(&l), ip(0), 0), 0);
    EXPECT_EQ_INT(dlist_ins(&l, dlist_back(&l), ip(2), 1), 0);
    EXPECT_EQ_PTR(dlist_nodeval(dlist_front(&l)), ip(0));
    EXPECT_EQ_PTR(dlist_nodeval(dlist_at(&l, 1)), ip(1));
    EXPECT_EQ_PTR(dlist_nodeval(dlist_back(&l)), ip(2));
    EXPECT_EQ_PTR(dlist_prevnode(dlist_at(&l, 1)), dlist_front(&l));
    EXPECT_EQ_PTR(dlist_nextnode(dlist_at(&l, 1)), dlist_back(&l));
    dlist_free(&l);
}

UTEST_CASE(dlist_del)
{
    struct dlist l;
    void *out;
    struct dlist_node *mid;

    dlist_init(&l);
    EXPECT_EQ_INT(dlist_add_tail(&l, ip(1)), 0);
    EXPECT_EQ_INT(dlist_add_tail(&l, ip(2)), 0);
    EXPECT_EQ_INT(dlist_add_tail(&l, ip(3)), 0);
    mid = dlist_at(&l, 1);
    EXPECT_EQ_INT(dlist_del(&l, mid, &out), 0);
    EXPECT_EQ_PTR(out, ip(2));
    EXPECT_EQ_UINT(dlist_len(&l), 2);
    EXPECT_EQ_PTR(dlist_nodeval(dlist_front(&l)), ip(1));
    EXPECT_EQ_PTR(dlist_nodeval(dlist_back(&l)), ip(3));
    EXPECT_EQ_INT(dlist_del(&l, dlist_front(&l), NULL), 0);
    EXPECT_EQ_PTR(dlist_nodeval(dlist_front(&l)), ip(3));
    EXPECT_EQ_INT(dlist_del(&l, dlist_front(&l), NULL), 0);
    EXPECT_TRUE(dlist_empty(&l));
    EXPECT_NULL(dlist_front(&l));
    EXPECT_NULL(dlist_back(&l));
    dlist_free(&l);

    dlist_init(&l);
    EXPECT_EQ_INT(dlist_add_tail(&l, ip(1)), 0);
    EXPECT_EQ_INT(dlist_add_tail(&l, ip(2)), 0);
    EXPECT_EQ_INT(dlist_add_tail(&l, ip(3)), 0);
    EXPECT_EQ_INT(dlist_del(&l, dlist_back(&l), &out), 0);
    EXPECT_EQ_PTR(out, ip(3));
    EXPECT_EQ_PTR(dlist_nodeval(dlist_back(&l)), ip(2));
    EXPECT_NULL(dlist_nextnode(dlist_back(&l)));
    EXPECT_EQ_INT(dlist_del(&l, dlist_front(&l), &out), 0);
    EXPECT_EQ_PTR(out, ip(1));
    EXPECT_EQ_PTR(dlist_front(&l), dlist_back(&l));
    dlist_free(&l);
}

UTEST_CASE(dlist_dup)
{
    struct dlist l, d;
    int *a, *b;

    dlist_init(&l);
    EXPECT_EQ_INT(dlist_dup(&l, &l), -EINVAL);
    EXPECT_EQ_INT(dlist_add_tail(&l, ip(1)), 0);
    EXPECT_EQ_INT(dlist_add_tail(&l, ip(2)), 0);
    EXPECT_EQ_INT(dlist_dup(&l, &d), 0);
    EXPECT_EQ_UINT(dlist_len(&d), 2);
    EXPECT_EQ_PTR(dlist_nodeval(dlist_front(&d)), ip(1));
    EXPECT_EQ_PTR(dlist_nodeval(dlist_back(&d)), ip(2));
    EXPECT_NE_PTR(dlist_front(&d), dlist_front(&l));
    dlist_free(&d);
    dlist_free(&l);

    dlist_init(&l);
    dlist_setdup(&l, int_dup);
    dlist_setfree(&l, int_free);
    a = malloc(sizeof(*a));
    b = malloc(sizeof(*b));
    EXPECT_NOTNULL(a);
    EXPECT_NOTNULL(b);
    *a = 4;
    *b = 5;
    EXPECT_EQ_INT(dlist_add_tail(&l, a), 0);
    EXPECT_EQ_INT(dlist_add_tail(&l, b), 0);
    EXPECT_EQ_INT(dlist_dup(&l, &d), 0);
    EXPECT_EQ_PTR(dlist_getfree(&d), int_free);
    EXPECT_EQ_INT(*(int *)dlist_nodeval(dlist_front(&d)), 4);
    EXPECT_NE_PTR(dlist_nodeval(dlist_front(&d)), a);
    EXPECT_EQ_PTR(dlist_getdup(&d), int_dup);
    EXPECT_EQ_PTR(dlist_getmatch(&d), NULL);
    EXPECT_EQ_INT(*(int *)dlist_nodeval(dlist_back(&d)), 5);
    dlist_free(&d);
    dlist_free(&l);

    dlist_init(&l);
    dlist_setdup(&l, int_dup);
    dlist_setmatch(&l, int_match);
    EXPECT_EQ_INT(dlist_dup(&l, &d), 0);
    EXPECT_TRUE(dlist_empty(&d));
    EXPECT_EQ_PTR(dlist_getdup(&d), int_dup);
    EXPECT_EQ_PTR(dlist_getmatch(&d), int_match);
    EXPECT_NULL(dlist_front(&d));
    dlist_free(&d);
    dlist_free(&l);
}

UTEST_CASE(dlist_search)
{
    struct dlist l;
    int *a, *b, key;
    struct dlist_node *n;

    dlist_init(&l);
    EXPECT_EQ_INT(dlist_add_tail(&l, ip(1)), 0);
    EXPECT_EQ_INT(dlist_add_tail(&l, ip(2)), 0);
    n = dlist_search(&l, ip(2));
    EXPECT_NOTNULL(n);
    EXPECT_EQ_PTR(dlist_nodeval(n), ip(2));
    EXPECT_NULL(dlist_search(&l, ip(9)));
    EXPECT_EQ_PTR(dlist_search(&l, ip(1)), dlist_front(&l));
    dlist_free(&l);

    dlist_init(&l);
    EXPECT_NULL(dlist_search(&l, ip(1)));
    dlist_free(&l);

    dlist_init(&l);
    EXPECT_EQ_INT(dlist_add_tail(&l, ip(2)), 0);
    EXPECT_EQ_INT(dlist_add_tail(&l, ip(2)), 0);
    n = dlist_search(&l, ip(2));
    EXPECT_EQ_PTR(n, dlist_front(&l));
    EXPECT_NE_PTR(n, dlist_back(&l));
    dlist_free(&l);

    dlist_init(&l);
    dlist_setmatch(&l, int_match);
    dlist_setfree(&l, int_free);
    a = malloc(sizeof(*a));
    b = malloc(sizeof(*b));
    EXPECT_NOTNULL(a);
    EXPECT_NOTNULL(b);
    *a = 1;
    *b = 2;
    EXPECT_EQ_INT(dlist_add_tail(&l, a), 0);
    EXPECT_EQ_INT(dlist_add_tail(&l, b), 0);
    key = 1;
    n = dlist_search(&l, &key);
    EXPECT_NOTNULL(n);
    EXPECT_EQ_INT(*(int *)dlist_nodeval(n), 1);
    key = 9;
    EXPECT_NULL(dlist_search(&l, &key));
    dlist_free(&l);
}

UTEST_CASE(dlist_at)
{
    struct dlist l;

    dlist_init(&l);
    EXPECT_NULL(dlist_at(&l, 0));
    EXPECT_EQ_INT(dlist_add_tail(&l, ip(1)), 0);
    EXPECT_EQ_INT(dlist_add_tail(&l, ip(2)), 0);
    EXPECT_EQ_INT(dlist_add_tail(&l, ip(3)), 0);
    EXPECT_EQ_PTR(dlist_nodeval(dlist_at(&l, 0)), ip(1));
    EXPECT_EQ_PTR(dlist_nodeval(dlist_at(&l, 1)), ip(2));
    EXPECT_EQ_PTR(dlist_nodeval(dlist_at(&l, 2)), ip(3));
    EXPECT_NULL(dlist_at(&l, 3));
    EXPECT_EQ_PTR(dlist_at(&l, 2), dlist_back(&l));
    EXPECT_EQ_PTR(dlist_nextnode(dlist_at(&l, 0)), dlist_at(&l, 1));
    EXPECT_EQ_PTR(dlist_prevnode(dlist_at(&l, 2)), dlist_at(&l, 1));
    dlist_free(&l);

    dlist_init(&l);
    EXPECT_EQ_INT(dlist_add_head(&l, ip(4)), 0);
    EXPECT_EQ_PTR(dlist_at(&l, 0), dlist_front(&l));
    EXPECT_NULL(dlist_at(&l, 1));
    dlist_free(&l);
}

UTEST_SUITE(dlist)
{
    UTEST_RUNCASE(dlist_len);
    UTEST_RUNCASE(dlist_empty);
    UTEST_RUNCASE(dlist_front);
    UTEST_RUNCASE(dlist_back);
    UTEST_RUNCASE(dlist_prevnode);
    UTEST_RUNCASE(dlist_nextnode);
    UTEST_RUNCASE(dlist_nodeval);
    UTEST_RUNCASE(dlist_setdup);
    UTEST_RUNCASE(dlist_setfree);
    UTEST_RUNCASE(dlist_setmatch);
    UTEST_RUNCASE(dlist_getdup);
    UTEST_RUNCASE(dlist_getfree);
    UTEST_RUNCASE(dlist_getmatch);
    UTEST_RUNCASE(dlist_init);
    UTEST_RUNCASE(dlist_free);
    UTEST_RUNCASE(dlist_clear);
    UTEST_RUNCASE(dlist_add_head);
    UTEST_RUNCASE(dlist_add_tail);
    UTEST_RUNCASE(dlist_del_head);
    UTEST_RUNCASE(dlist_del_tail);
    UTEST_RUNCASE(dlist_ins);
    UTEST_RUNCASE(dlist_del);
    UTEST_RUNCASE(dlist_dup);
    UTEST_RUNCASE(dlist_search);
    UTEST_RUNCASE(dlist_at);
}
