/* Tests for slist. */

#include <errno.h>
#include <slist.h>
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

UTEST_CASE(slist_len)
{
    struct slist l;

    slist_init(&l);
    EXPECT_EQ_UINT(slist_len(&l), 0);
    EXPECT_EQ_INT(slist_add_tail(&l, ip(1)), 0);
    EXPECT_EQ_UINT(slist_len(&l), 1);
    EXPECT_EQ_INT(slist_add_head(&l, ip(0)), 0);
    EXPECT_EQ_INT(slist_add_tail(&l, ip(2)), 0);
    EXPECT_EQ_UINT(slist_len(&l), 3);
    EXPECT_EQ_INT(slist_del_head(&l, NULL), 0);
    EXPECT_EQ_UINT(slist_len(&l), 2);
    slist_free(&l);
}

UTEST_CASE(slist_empty)
{
    struct slist l;

    slist_init(&l);
    EXPECT_TRUE(slist_empty(&l));
    EXPECT_EQ_INT(slist_add_head(&l, ip(1)), 0);
    EXPECT_FALSE(slist_empty(&l));
    EXPECT_EQ_INT(slist_del_head(&l, NULL), 0);
    EXPECT_TRUE(slist_empty(&l));
    slist_free(&l);
}

UTEST_CASE(slist_front)
{
    struct slist l;

    slist_init(&l);
    EXPECT_NULL(slist_front(&l));
    EXPECT_EQ_INT(slist_add_tail(&l, ip(1)), 0);
    EXPECT_EQ_INT(slist_add_tail(&l, ip(2)), 0);
    EXPECT_EQ_PTR(slist_nodeval(slist_front(&l)), ip(1));
    EXPECT_EQ_PTR(slist_front(&l), slist_at(&l, 0));
    slist_free(&l);

    slist_init(&l);
    EXPECT_EQ_INT(slist_add_head(&l, ip(7)), 0);
    EXPECT_EQ_PTR(slist_front(&l), slist_back(&l));
    EXPECT_NULL(slist_nextnode(slist_front(&l)));
    slist_free(&l);
}

UTEST_CASE(slist_back)
{
    struct slist l;

    slist_init(&l);
    EXPECT_NULL(slist_back(&l));
    EXPECT_EQ_INT(slist_add_tail(&l, ip(1)), 0);
    EXPECT_EQ_INT(slist_add_tail(&l, ip(2)), 0);
    EXPECT_EQ_PTR(slist_nodeval(slist_back(&l)), ip(2));
    EXPECT_EQ_PTR(slist_back(&l), slist_at(&l, 1));
    slist_free(&l);

    slist_init(&l);
    EXPECT_EQ_INT(slist_add_tail(&l, ip(9)), 0);
    EXPECT_EQ_PTR(slist_back(&l), slist_front(&l));
    EXPECT_EQ_PTR(slist_nodeval(slist_back(&l)), ip(9));
    slist_free(&l);
}

UTEST_CASE(slist_nextnode)
{
    struct slist l;

    slist_init(&l);
    EXPECT_EQ_INT(slist_add_tail(&l, ip(1)), 0);
    EXPECT_EQ_INT(slist_add_tail(&l, ip(2)), 0);
    EXPECT_NULL(slist_nextnode(slist_back(&l)));
    EXPECT_EQ_PTR(slist_nodeval(slist_nextnode(slist_front(&l))), ip(2));
    slist_free(&l);

    slist_init(&l);
    EXPECT_EQ_INT(slist_add_tail(&l, ip(1)), 0);
    EXPECT_EQ_INT(slist_add_tail(&l, ip(2)), 0);
    EXPECT_EQ_INT(slist_add_tail(&l, ip(3)), 0);
    EXPECT_EQ_PTR(slist_nextnode(slist_front(&l)), slist_at(&l, 1));
    EXPECT_EQ_PTR(slist_nextnode(slist_at(&l, 1)), slist_back(&l));
    EXPECT_NULL(slist_nextnode(slist_back(&l)));
    slist_free(&l);
}

UTEST_CASE(slist_nodeval)
{
    struct slist l;

    slist_init(&l);
    EXPECT_EQ_INT(slist_add_head(&l, ip(7)), 0);
    EXPECT_EQ_PTR(slist_nodeval(slist_front(&l)), ip(7));
    slist_free(&l);
}

UTEST_CASE(slist_setdup)
{
    struct slist l;

    slist_init(&l);
    slist_setdup(&l, int_dup);
    EXPECT_EQ_PTR(slist_getdup(&l), int_dup);
    slist_free(&l);
}

UTEST_CASE(slist_setfree)
{
    struct slist l;

    slist_init(&l);
    slist_setfree(&l, int_free);
    EXPECT_EQ_PTR(slist_getfree(&l), int_free);
    slist_free(&l);
}

UTEST_CASE(slist_setmatch)
{
    struct slist l;

    slist_init(&l);
    slist_setmatch(&l, int_match);
    EXPECT_EQ_PTR(slist_getmatch(&l), int_match);
    slist_free(&l);
}

UTEST_CASE(slist_getdup)
{
    struct slist l;

    slist_init(&l);
    EXPECT_NULL(slist_getdup(&l));
    slist_setdup(&l, int_dup);
    EXPECT_EQ_PTR(slist_getdup(&l), int_dup);
    slist_free(&l);
}

UTEST_CASE(slist_getfree)
{
    struct slist l;

    slist_init(&l);
    EXPECT_NULL(slist_getfree(&l));
    slist_setfree(&l, int_free);
    EXPECT_EQ_PTR(slist_getfree(&l), int_free);
    slist_free(&l);
}

UTEST_CASE(slist_getmatch)
{
    struct slist l;

    slist_init(&l);
    EXPECT_NULL(slist_getmatch(&l));
    slist_setmatch(&l, int_match);
    EXPECT_EQ_PTR(slist_getmatch(&l), int_match);
    slist_free(&l);
}

UTEST_CASE(slist_init)
{
    struct slist l;

    slist_init(&l);
    EXPECT_TRUE(slist_empty(&l));
    EXPECT_NULL(slist_front(&l));
    EXPECT_NULL(slist_back(&l));
    EXPECT_NULL(slist_getdup(&l));
    EXPECT_NULL(slist_getfree(&l));
    EXPECT_NULL(slist_getmatch(&l));
    slist_free(&l);
}

UTEST_CASE(slist_free)
{
    struct slist l;

    slist_free(NULL);
    slist_init(&l);
    EXPECT_EQ_INT(slist_add_tail(&l, ip(1)), 0);
    slist_free(&l);
    EXPECT_TRUE(slist_empty(&l));
    EXPECT_NULL(slist_front(&l));
}

UTEST_CASE(slist_clear)
{
    struct slist l;
    int *a, *b;

    slist_init(&l);
    slist_setfree(&l, int_free);
    a = malloc(sizeof(*a));
    b = malloc(sizeof(*b));
    EXPECT_NOTNULL(a);
    EXPECT_NOTNULL(b);
    *a = 1;
    *b = 2;
    EXPECT_EQ_INT(slist_add_tail(&l, a), 0);
    EXPECT_EQ_INT(slist_add_tail(&l, b), 0);
    slist_clear(&l);
    EXPECT_TRUE(slist_empty(&l));
    EXPECT_NULL(slist_front(&l));
    EXPECT_NULL(slist_back(&l));
    EXPECT_EQ_PTR(slist_getfree(&l), int_free);
    slist_free(&l);

    slist_init(&l);
    slist_setdup(&l, int_dup);
    slist_setfree(&l, int_free);
    slist_setmatch(&l, int_match);
    slist_clear(&l);
    EXPECT_TRUE(slist_empty(&l));
    EXPECT_EQ_PTR(slist_getdup(&l), int_dup);
    EXPECT_EQ_PTR(slist_getfree(&l), int_free);
    EXPECT_EQ_PTR(slist_getmatch(&l), int_match);
    slist_free(&l);
}

UTEST_CASE(slist_add_head)
{
    struct slist l;

    slist_init(&l);
    EXPECT_EQ_INT(slist_add_head(&l, ip(1)), 0);
    EXPECT_EQ_INT(slist_add_head(&l, ip(2)), 0);
    EXPECT_EQ_UINT(slist_len(&l), 2);
    EXPECT_EQ_PTR(slist_nodeval(slist_front(&l)), ip(2));
    EXPECT_EQ_PTR(slist_nodeval(slist_back(&l)), ip(1));
    EXPECT_EQ_PTR(slist_nextnode(slist_front(&l)), slist_back(&l));
    slist_free(&l);

    slist_init(&l);
    EXPECT_EQ_INT(slist_add_head(&l, ip(1)), 0);
    EXPECT_EQ_PTR(slist_front(&l), slist_back(&l));
    EXPECT_EQ_INT(slist_add_head(&l, ip(2)), 0);
    EXPECT_EQ_INT(slist_add_head(&l, ip(3)), 0);
    EXPECT_EQ_PTR(slist_nodeval(slist_at(&l, 0)), ip(3));
    EXPECT_EQ_PTR(slist_nodeval(slist_at(&l, 1)), ip(2));
    EXPECT_EQ_PTR(slist_nodeval(slist_at(&l, 2)), ip(1));
    EXPECT_EQ_PTR(slist_nextnode(slist_at(&l, 0)), slist_at(&l, 1));
    slist_free(&l);
}

UTEST_CASE(slist_add_tail)
{
    struct slist l;

    slist_init(&l);
    EXPECT_EQ_INT(slist_add_tail(&l, ip(1)), 0);
    EXPECT_EQ_INT(slist_add_tail(&l, ip(2)), 0);
    EXPECT_EQ_UINT(slist_len(&l), 2);
    EXPECT_EQ_PTR(slist_nodeval(slist_front(&l)), ip(1));
    EXPECT_EQ_PTR(slist_nodeval(slist_back(&l)), ip(2));
    EXPECT_NULL(slist_nextnode(slist_back(&l)));
    slist_free(&l);

    slist_init(&l);
    EXPECT_EQ_INT(slist_add_head(&l, ip(4)), 0);
    EXPECT_EQ_INT(slist_add_tail(&l, ip(5)), 0);
    EXPECT_EQ_INT(slist_add_tail(&l, ip(6)), 0);
    EXPECT_EQ_PTR(slist_nodeval(slist_front(&l)), ip(4));
    EXPECT_EQ_PTR(slist_nodeval(slist_back(&l)), ip(6));
    EXPECT_EQ_PTR(slist_nodeval(slist_at(&l, 1)), ip(5));
    slist_free(&l);
}

UTEST_CASE(slist_del_head)
{
    struct slist l;
    void *out;

    slist_init(&l);
    EXPECT_EQ_INT(slist_del_head(&l, &out), -ENOENT);
    EXPECT_EQ_INT(slist_add_tail(&l, ip(1)), 0);
    EXPECT_EQ_INT(slist_add_tail(&l, ip(2)), 0);
    EXPECT_EQ_INT(slist_del_head(&l, &out), 0);
    EXPECT_EQ_PTR(out, ip(1));
    EXPECT_EQ_UINT(slist_len(&l), 1);
    EXPECT_EQ_PTR(slist_nodeval(slist_front(&l)), ip(2));
    EXPECT_EQ_INT(slist_del_head(&l, NULL), 0);
    EXPECT_TRUE(slist_empty(&l));
    EXPECT_NULL(slist_back(&l));
    EXPECT_NULL(slist_front(&l));
    EXPECT_EQ_INT(slist_del_head(&l, &out), -ENOENT);
    slist_free(&l);

    slist_init(&l);
    EXPECT_EQ_INT(slist_add_head(&l, ip(4)), 0);
    EXPECT_EQ_INT(slist_del_head(&l, &out), 0);
    EXPECT_EQ_PTR(out, ip(4));
    EXPECT_TRUE(slist_empty(&l));
    EXPECT_NULL(slist_front(&l));
    EXPECT_NULL(slist_back(&l));
    slist_free(&l);
}

UTEST_CASE(slist_insn)
{
    struct slist l;

    slist_init(&l);
    EXPECT_EQ_INT(slist_add_tail(&l, ip(1)), 0);
    EXPECT_EQ_INT(slist_insn(&l, slist_front(&l), ip(2)), 0);
    EXPECT_EQ_INT(slist_insn(&l, slist_back(&l), ip(3)), 0);
    EXPECT_EQ_UINT(slist_len(&l), 3);
    EXPECT_EQ_PTR(slist_nodeval(slist_at(&l, 0)), ip(1));
    EXPECT_EQ_PTR(slist_nodeval(slist_at(&l, 1)), ip(2));
    EXPECT_EQ_PTR(slist_nodeval(slist_at(&l, 2)), ip(3));
    EXPECT_EQ_PTR(slist_back(&l), slist_at(&l, 2));
    slist_free(&l);

    slist_init(&l);
    EXPECT_EQ_INT(slist_add_tail(&l, ip(1)), 0);
    EXPECT_EQ_INT(slist_insn(&l, slist_front(&l), ip(9)), 0);
    EXPECT_EQ_PTR(slist_nodeval(slist_front(&l)), ip(1));
    EXPECT_EQ_PTR(slist_nodeval(slist_back(&l)), ip(9));
    EXPECT_EQ_PTR(slist_back(&l), slist_nextnode(slist_front(&l)));
    EXPECT_EQ_INT(slist_insn(&l, slist_back(&l), ip(8)), 0);
    EXPECT_EQ_PTR(slist_nodeval(slist_back(&l)), ip(8));
    EXPECT_EQ_UINT(slist_len(&l), 3);
    slist_free(&l);
}

UTEST_CASE(slist_deln)
{
    struct slist l;
    void *out;

    slist_init(&l);
    EXPECT_EQ_INT(slist_add_tail(&l, ip(1)), 0);
    EXPECT_EQ_INT(slist_deln(&l, slist_front(&l), &out), -ENOENT);
    EXPECT_EQ_INT(slist_add_tail(&l, ip(2)), 0);
    EXPECT_EQ_INT(slist_add_tail(&l, ip(3)), 0);
    EXPECT_EQ_INT(slist_deln(&l, slist_front(&l), &out), 0);
    EXPECT_EQ_PTR(out, ip(2));
    EXPECT_EQ_UINT(slist_len(&l), 2);
    EXPECT_EQ_PTR(slist_nodeval(slist_front(&l)), ip(1));
    EXPECT_EQ_PTR(slist_nodeval(slist_back(&l)), ip(3));
    EXPECT_EQ_INT(slist_deln(&l, slist_front(&l), NULL), 0);
    EXPECT_EQ_PTR(slist_front(&l), slist_back(&l));
    EXPECT_NULL(slist_nextnode(slist_back(&l)));
    slist_free(&l);

    slist_init(&l);
    EXPECT_EQ_INT(slist_add_tail(&l, ip(1)), 0);
    EXPECT_EQ_INT(slist_add_tail(&l, ip(2)), 0);
    EXPECT_EQ_INT(slist_add_tail(&l, ip(3)), 0);
    EXPECT_EQ_INT(slist_deln(&l, slist_at(&l, 1), &out), 0);
    EXPECT_EQ_PTR(out, ip(3));
    EXPECT_EQ_PTR(slist_back(&l), slist_at(&l, 1));
    EXPECT_EQ_PTR(slist_nodeval(slist_back(&l)), ip(2));
    EXPECT_NULL(slist_nextnode(slist_back(&l)));
    slist_free(&l);
}

UTEST_CASE(slist_sdel)
{
    struct slist l;
    void *out;
    int *a, *b, *c, key;

    slist_init(&l);
    EXPECT_EQ_INT(slist_sdel(&l, ip(1), &out), -ENOENT);
    EXPECT_EQ_INT(slist_add_tail(&l, ip(1)), 0);
    EXPECT_EQ_INT(slist_add_tail(&l, ip(2)), 0);
    EXPECT_EQ_INT(slist_add_tail(&l, ip(3)), 0);
    EXPECT_EQ_INT(slist_sdel(&l, ip(2), &out), 0);
    EXPECT_EQ_PTR(out, ip(2));
    EXPECT_EQ_UINT(slist_len(&l), 2);
    EXPECT_EQ_PTR(slist_nodeval(slist_front(&l)), ip(1));
    EXPECT_EQ_PTR(slist_nodeval(slist_back(&l)), ip(3));
    EXPECT_EQ_INT(slist_sdel(&l, ip(1), NULL), 0);
    EXPECT_EQ_PTR(slist_nodeval(slist_front(&l)), ip(3));
    EXPECT_EQ_INT(slist_sdel(&l, ip(9), &out), -ENOENT);
    EXPECT_EQ_INT(slist_sdel(&l, ip(3), &out), 0);
    EXPECT_EQ_PTR(out, ip(3));
    EXPECT_TRUE(slist_empty(&l));
    EXPECT_NULL(slist_back(&l));
    EXPECT_EQ_INT(slist_sdel(&l, ip(3), &out), -ENOENT);
    slist_free(&l);

    slist_init(&l);
    EXPECT_EQ_INT(slist_add_tail(&l, ip(7)), 0);
    EXPECT_EQ_INT(slist_sdel(&l, ip(7), &out), 0);
    EXPECT_EQ_PTR(out, ip(7));
    EXPECT_TRUE(slist_empty(&l));
    slist_free(&l);

    slist_init(&l);
    EXPECT_EQ_INT(slist_add_tail(&l, ip(1)), 0);
    EXPECT_EQ_INT(slist_add_tail(&l, ip(2)), 0);
    EXPECT_EQ_INT(slist_add_tail(&l, ip(2)), 0);
    EXPECT_EQ_INT(slist_sdel(&l, ip(2), &out), 0);
    EXPECT_EQ_PTR(out, ip(2));
    EXPECT_EQ_UINT(slist_len(&l), 2);
    EXPECT_EQ_PTR(slist_nodeval(slist_back(&l)), ip(2));
    slist_free(&l);

    slist_init(&l);
    slist_setmatch(&l, int_match);
    slist_setfree(&l, int_free);
    a = malloc(sizeof(*a));
    b = malloc(sizeof(*b));
    c = malloc(sizeof(*c));
    EXPECT_NOTNULL(a);
    EXPECT_NOTNULL(b);
    EXPECT_NOTNULL(c);
    *a = 1;
    *b = 2;
    *c = 3;
    EXPECT_EQ_INT(slist_add_tail(&l, a), 0);
    EXPECT_EQ_INT(slist_add_tail(&l, b), 0);
    EXPECT_EQ_INT(slist_add_tail(&l, c), 0);
    key = 2;
    EXPECT_EQ_INT(slist_sdel(&l, &key, &out), 0);
    EXPECT_EQ_INT(*(int *)out, 2);
    free(out);
    EXPECT_EQ_UINT(slist_len(&l), 2);
    slist_free(&l);
}

UTEST_CASE(slist_dup)
{
    struct slist l, d;
    int *a, *b;

    slist_init(&l);
    EXPECT_EQ_INT(slist_dup(&l, &l), -EINVAL);
    EXPECT_EQ_INT(slist_add_tail(&l, ip(1)), 0);
    EXPECT_EQ_INT(slist_add_tail(&l, ip(2)), 0);
    EXPECT_EQ_INT(slist_dup(&l, &d), 0);
    EXPECT_EQ_UINT(slist_len(&d), 2);
    EXPECT_EQ_PTR(slist_nodeval(slist_front(&d)), ip(1));
    EXPECT_EQ_PTR(slist_nodeval(slist_back(&d)), ip(2));
    EXPECT_NE_PTR(slist_front(&d), slist_front(&l));
    slist_free(&d);
    slist_free(&l);

    slist_init(&l);
    slist_setdup(&l, int_dup);
    slist_setfree(&l, int_free);
    a = malloc(sizeof(*a));
    b = malloc(sizeof(*b));
    EXPECT_NOTNULL(a);
    EXPECT_NOTNULL(b);
    *a = 4;
    *b = 5;
    EXPECT_EQ_INT(slist_add_tail(&l, a), 0);
    EXPECT_EQ_INT(slist_add_tail(&l, b), 0);
    EXPECT_EQ_INT(slist_dup(&l, &d), 0);
    EXPECT_EQ_PTR(slist_getfree(&d), int_free);
    EXPECT_EQ_INT(*(int *)slist_nodeval(slist_front(&d)), 4);
    EXPECT_NE_PTR(slist_nodeval(slist_front(&d)), a);
    EXPECT_EQ_PTR(slist_getdup(&d), int_dup);
    EXPECT_EQ_INT(*(int *)slist_nodeval(slist_back(&d)), 5);
    slist_free(&d);
    slist_free(&l);

    slist_init(&l);
    slist_setdup(&l, int_dup);
    slist_setmatch(&l, int_match);
    EXPECT_EQ_INT(slist_dup(&l, &d), 0);
    EXPECT_TRUE(slist_empty(&d));
    EXPECT_EQ_PTR(slist_getdup(&d), int_dup);
    EXPECT_EQ_PTR(slist_getmatch(&d), int_match);
    EXPECT_NULL(slist_front(&d));
    slist_free(&d);
    slist_free(&l);
}

UTEST_CASE(slist_search)
{
    struct slist l;
    int *a, *b, key;
    struct slist_node *n;

    slist_init(&l);
    EXPECT_EQ_INT(slist_add_tail(&l, ip(1)), 0);
    EXPECT_EQ_INT(slist_add_tail(&l, ip(2)), 0);
    n = slist_search(&l, ip(2));
    EXPECT_NOTNULL(n);
    EXPECT_EQ_PTR(slist_nodeval(n), ip(2));
    EXPECT_NULL(slist_search(&l, ip(9)));
    EXPECT_EQ_PTR(slist_search(&l, ip(1)), slist_front(&l));
    slist_free(&l);

    slist_init(&l);
    EXPECT_NULL(slist_search(&l, ip(1)));
    slist_free(&l);

    slist_init(&l);
    EXPECT_EQ_INT(slist_add_tail(&l, ip(2)), 0);
    EXPECT_EQ_INT(slist_add_tail(&l, ip(2)), 0);
    n = slist_search(&l, ip(2));
    EXPECT_EQ_PTR(n, slist_front(&l));
    EXPECT_NE_PTR(n, slist_back(&l));
    slist_free(&l);

    slist_init(&l);
    slist_setmatch(&l, int_match);
    slist_setfree(&l, int_free);
    a = malloc(sizeof(*a));
    b = malloc(sizeof(*b));
    EXPECT_NOTNULL(a);
    EXPECT_NOTNULL(b);
    *a = 1;
    *b = 2;
    EXPECT_EQ_INT(slist_add_tail(&l, a), 0);
    EXPECT_EQ_INT(slist_add_tail(&l, b), 0);
    key = 1;
    n = slist_search(&l, &key);
    EXPECT_NOTNULL(n);
    EXPECT_EQ_INT(*(int *)slist_nodeval(n), 1);
    key = 9;
    EXPECT_NULL(slist_search(&l, &key));
    slist_free(&l);
}

UTEST_CASE(slist_at)
{
    struct slist l;

    slist_init(&l);
    EXPECT_NULL(slist_at(&l, 0));
    EXPECT_EQ_INT(slist_add_tail(&l, ip(1)), 0);
    EXPECT_EQ_INT(slist_add_tail(&l, ip(2)), 0);
    EXPECT_EQ_INT(slist_add_tail(&l, ip(3)), 0);
    EXPECT_EQ_PTR(slist_nodeval(slist_at(&l, 0)), ip(1));
    EXPECT_EQ_PTR(slist_nodeval(slist_at(&l, 1)), ip(2));
    EXPECT_EQ_PTR(slist_nodeval(slist_at(&l, 2)), ip(3));
    EXPECT_NULL(slist_at(&l, 3));
    EXPECT_EQ_PTR(slist_at(&l, 2), slist_back(&l));
    EXPECT_EQ_PTR(slist_nextnode(slist_at(&l, 0)), slist_at(&l, 1));
    slist_free(&l);

    slist_init(&l);
    EXPECT_EQ_INT(slist_add_head(&l, ip(4)), 0);
    EXPECT_EQ_PTR(slist_at(&l, 0), slist_front(&l));
    EXPECT_NULL(slist_at(&l, 1));
    slist_free(&l);
}

UTEST_SUITE(slist)
{
    UTEST_RUNCASE(slist_len);
    UTEST_RUNCASE(slist_empty);
    UTEST_RUNCASE(slist_front);
    UTEST_RUNCASE(slist_back);
    UTEST_RUNCASE(slist_nextnode);
    UTEST_RUNCASE(slist_nodeval);
    UTEST_RUNCASE(slist_setdup);
    UTEST_RUNCASE(slist_setfree);
    UTEST_RUNCASE(slist_setmatch);
    UTEST_RUNCASE(slist_getdup);
    UTEST_RUNCASE(slist_getfree);
    UTEST_RUNCASE(slist_getmatch);
    UTEST_RUNCASE(slist_init);
    UTEST_RUNCASE(slist_free);
    UTEST_RUNCASE(slist_clear);
    UTEST_RUNCASE(slist_add_head);
    UTEST_RUNCASE(slist_add_tail);
    UTEST_RUNCASE(slist_del_head);
    UTEST_RUNCASE(slist_insn);
    UTEST_RUNCASE(slist_deln);
    UTEST_RUNCASE(slist_sdel);
    UTEST_RUNCASE(slist_dup);
    UTEST_RUNCASE(slist_search);
    UTEST_RUNCASE(slist_at);
}
