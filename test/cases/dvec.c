/* Tests for dvec. */

#include <dvec.h>
#include <errno.h>
#include <string.h>
#include <utest.h>

static int cmp_int(const void *a, const void *b)
{
    int x = *(const int *)a;
    int y = *(const int *)b;

    return (x > y) - (x < y);
}

UTEST_CASE(dvec_raw)
{
    struct dvec v;
    int x = 1;

    EXPECT_EQ_INT(dvec_init_empty(&v, sizeof(int)), 0);
    EXPECT_NULL(dvec_raw(&v));
    EXPECT_EQ_INT(dvec_pushb(&v, &x), 0);
    EXPECT_NOTNULL(dvec_raw(&v));
    EXPECT_EQ_PTR(dvec_raw(&v), v.buf);
    dvec_free(&v);
}

UTEST_CASE(dvec_len)
{
    struct dvec v;
    int x = 1;

    EXPECT_EQ_INT(dvec_init_empty(&v, sizeof(int)), 0);
    EXPECT_EQ_UINT(dvec_len(&v), 0);
    EXPECT_EQ_INT(dvec_pushb(&v, &x), 0);
    EXPECT_EQ_UINT(dvec_len(&v), 1);
    dvec_free(&v);
}

UTEST_CASE(dvec_cap)
{
    struct dvec v;

    EXPECT_EQ_INT(dvec_init(&v, sizeof(int), 4), 0);
    EXPECT_EQ_UINT(dvec_cap(&v), 4);
    EXPECT_GE_UINT(dvec_cap(&v), dvec_len(&v));
    dvec_free(&v);
}

UTEST_CASE(dvec_elesz)
{
    struct dvec v;

    EXPECT_EQ_INT(dvec_init_empty(&v, sizeof(int)), 0);
    EXPECT_EQ_UINT(dvec_elesz(&v), sizeof(int));
    dvec_free(&v);
}

UTEST_CASE(dvec_empty)
{
    struct dvec v;
    int x = 1;

    EXPECT_EQ_INT(dvec_init_empty(&v, sizeof(int)), 0);
    EXPECT_TRUE(dvec_empty(&v));
    EXPECT_EQ_INT(dvec_pushb(&v, &x), 0);
    EXPECT_FALSE(dvec_empty(&v));
    dvec_free(&v);
}

UTEST_CASE(dvec_avail)
{
    struct dvec v;
    int x = 1;

    EXPECT_EQ_INT(dvec_init(&v, sizeof(int), 4), 0);
    EXPECT_EQ_UINT(dvec_avail(&v), 4);
    EXPECT_EQ_INT(dvec_pushb(&v, &x), 0);
    EXPECT_EQ_UINT(dvec_avail(&v), dvec_cap(&v) - dvec_len(&v));
    dvec_free(&v);
}

UTEST_CASE(dvec_init)
{
    struct dvec v;

    EXPECT_EQ_INT(dvec_init(&v, 0, 4), -EINVAL);

    EXPECT_EQ_INT(dvec_init(&v, sizeof(int), 4), 0);
    EXPECT_NOTNULL(dvec_raw(&v));
    EXPECT_EQ_UINT(dvec_len(&v), 0);
    EXPECT_EQ_UINT(dvec_cap(&v), 4);
    EXPECT_EQ_UINT(dvec_elesz(&v), sizeof(int));
    EXPECT_TRUE(dvec_empty(&v));
    dvec_free(&v);

    EXPECT_EQ_INT(dvec_init(&v, sizeof(int), 0), 0);
    EXPECT_NULL(dvec_raw(&v));
    EXPECT_EQ_UINT(dvec_cap(&v), 0);
    dvec_free(&v);
}

UTEST_CASE(dvec_init_empty)
{
    struct dvec v;

    EXPECT_EQ_INT(dvec_init_empty(&v, 0), -EINVAL);

    EXPECT_EQ_INT(dvec_init_empty(&v, sizeof(int)), 0);
    EXPECT_NULL(dvec_raw(&v));
    EXPECT_EQ_UINT(dvec_len(&v), 0);
    EXPECT_EQ_UINT(dvec_cap(&v), 0);
    EXPECT_TRUE(dvec_empty(&v));
    dvec_free(&v);
}

UTEST_CASE(dvec_free)
{
    struct dvec v;
    int x = 1;

    dvec_free(NULL);

    EXPECT_EQ_INT(dvec_init_empty(&v, sizeof(int)), 0);
    EXPECT_EQ_INT(dvec_pushb(&v, &x), 0);
    dvec_free(&v);
    EXPECT_NULL(dvec_raw(&v));
    EXPECT_EQ_UINT(dvec_len(&v), 0);
    EXPECT_EQ_UINT(dvec_cap(&v), 0);
}

UTEST_CASE(dvec_clear)
{
    struct dvec v;
    int x = 7;
    size_t cap;

    EXPECT_EQ_INT(dvec_init(&v, sizeof(int), 4), 0);
    EXPECT_EQ_INT(dvec_pushb(&v, &x), 0);
    cap = dvec_cap(&v);
    dvec_clear(&v);
    EXPECT_EQ_UINT(dvec_len(&v), 0);
    EXPECT_TRUE(dvec_empty(&v));
    EXPECT_EQ_UINT(dvec_cap(&v), cap);
    EXPECT_NOTNULL(dvec_raw(&v));
    dvec_free(&v);
}

UTEST_CASE(dvec_at)
{
    struct dvec v;
    int a = 1, b = 2, c = 3;

    EXPECT_EQ_INT(dvec_init_empty(&v, sizeof(int)), 0);
    EXPECT_NULL(dvec_at(&v, 0));
    EXPECT_EQ_INT(dvec_pushb(&v, &a), 0);
    EXPECT_EQ_INT(dvec_pushb(&v, &b), 0);
    EXPECT_EQ_INT(dvec_pushb(&v, &c), 0);
    EXPECT_EQ_INT(*(int *)dvec_at(&v, 0), 1);
    EXPECT_EQ_INT(*(int *)dvec_at(&v, 1), 2);
    EXPECT_EQ_INT(*(int *)dvec_at(&v, 2), 3);
    EXPECT_NULL(dvec_at(&v, 3));
    dvec_free(&v);
}

UTEST_CASE(dvec_back)
{
    struct dvec v;
    int a = 1, b = 2;

    EXPECT_EQ_INT(dvec_init_empty(&v, sizeof(int)), 0);
    EXPECT_NULL(dvec_back(&v));
    EXPECT_EQ_INT(dvec_pushb(&v, &a), 0);
    EXPECT_EQ_INT(*(int *)dvec_back(&v), 1);
    EXPECT_EQ_INT(dvec_pushb(&v, &b), 0);
    EXPECT_EQ_INT(*(int *)dvec_back(&v), 2);
    dvec_free(&v);
}

UTEST_CASE(dvec_front)
{
    struct dvec v;
    int a = 1, b = 2;

    EXPECT_EQ_INT(dvec_init_empty(&v, sizeof(int)), 0);
    EXPECT_NULL(dvec_front(&v));
    EXPECT_EQ_INT(dvec_pushb(&v, &a), 0);
    EXPECT_EQ_INT(*(int *)dvec_front(&v), 1);
    EXPECT_EQ_INT(dvec_pushb(&v, &b), 0);
    EXPECT_EQ_INT(*(int *)dvec_front(&v), 1);
    dvec_free(&v);
}

UTEST_CASE(dvec_reserve)
{
    struct dvec v;
    int x = 5;

    EXPECT_EQ_INT(dvec_init_empty(&v, sizeof(int)), 0);
    EXPECT_EQ_INT(dvec_reserve(&v, 8), 0);
    EXPECT_GE_UINT(dvec_cap(&v), 8);
    EXPECT_EQ_UINT(dvec_len(&v), 0);
    EXPECT_EQ_INT(dvec_pushb(&v, &x), 0);
    EXPECT_EQ_INT(dvec_reserve(&v, 1), 0);
    EXPECT_GE_UINT(dvec_cap(&v), 8);
    EXPECT_EQ_INT(*(int *)dvec_at(&v, 0), 5);
    dvec_free(&v);
}

UTEST_CASE(dvec_shrink)
{
    struct dvec v;
    int x = 9;

    EXPECT_EQ_INT(dvec_init(&v, sizeof(int), 8), 0);
    dvec_shrink(&v);
    EXPECT_EQ_UINT(dvec_cap(&v), 0);
    EXPECT_NULL(dvec_raw(&v));

    EXPECT_EQ_INT(dvec_init(&v, sizeof(int), 8), 0);
    EXPECT_EQ_INT(dvec_pushb(&v, &x), 0);
    dvec_shrink(&v);
    EXPECT_EQ_UINT(dvec_cap(&v), 1);
    EXPECT_EQ_UINT(dvec_len(&v), 1);
    EXPECT_EQ_INT(*(int *)dvec_at(&v, 0), 9);
    dvec_shrink(&v);
    EXPECT_EQ_UINT(dvec_cap(&v), 1);
    dvec_free(&v);
}

UTEST_CASE(dvec_pushb)
{
    struct dvec v;
    int a = 1, b = 2, c = 3;

    EXPECT_EQ_INT(dvec_init_empty(&v, sizeof(int)), 0);
    EXPECT_EQ_INT(dvec_pushb(&v, &a), 0);
    EXPECT_EQ_INT(dvec_pushb(&v, &b), 0);
    EXPECT_EQ_INT(dvec_pushb(&v, &c), 0);
    EXPECT_EQ_UINT(dvec_len(&v), 3);
    EXPECT_GE_UINT(dvec_cap(&v), 3);
    EXPECT_EQ_INT(*(int *)dvec_at(&v, 0), 1);
    EXPECT_EQ_INT(*(int *)dvec_at(&v, 1), 2);
    EXPECT_EQ_INT(*(int *)dvec_at(&v, 2), 3);
    dvec_free(&v);
}

UTEST_CASE(dvec_pushf)
{
    struct dvec v;
    int a = 1, b = 2, c = 3;

    EXPECT_EQ_INT(dvec_init_empty(&v, sizeof(int)), 0);
    EXPECT_EQ_INT(dvec_pushf(&v, &a), 0);
    EXPECT_EQ_INT(dvec_pushf(&v, &b), 0);
    EXPECT_EQ_INT(dvec_pushf(&v, &c), 0);
    EXPECT_EQ_UINT(dvec_len(&v), 3);
    EXPECT_EQ_INT(*(int *)dvec_at(&v, 0), 3);
    EXPECT_EQ_INT(*(int *)dvec_at(&v, 1), 2);
    EXPECT_EQ_INT(*(int *)dvec_at(&v, 2), 1);
    dvec_free(&v);
}

UTEST_CASE(dvec_popb)
{
    struct dvec v;
    int a = 1, b = 2, out = 0;

    EXPECT_EQ_INT(dvec_init_empty(&v, sizeof(int)), 0);
    EXPECT_EQ_INT(dvec_popb(&v, &out), -ENOENT);
    EXPECT_EQ_INT(dvec_pushb(&v, &a), 0);
    EXPECT_EQ_INT(dvec_pushb(&v, &b), 0);
    EXPECT_EQ_INT(dvec_popb(&v, &out), 0);
    EXPECT_EQ_INT(out, 2);
    EXPECT_EQ_UINT(dvec_len(&v), 1);
    EXPECT_EQ_INT(dvec_popb(&v, NULL), 0);
    EXPECT_TRUE(dvec_empty(&v));
    dvec_free(&v);
}

UTEST_CASE(dvec_popf)
{
    struct dvec v;
    int a = 1, b = 2, c = 3, out = 0;

    EXPECT_EQ_INT(dvec_init_empty(&v, sizeof(int)), 0);
    EXPECT_EQ_INT(dvec_popf(&v, &out), -ENOENT);
    EXPECT_EQ_INT(dvec_pushb(&v, &a), 0);
    EXPECT_EQ_INT(dvec_pushb(&v, &b), 0);
    EXPECT_EQ_INT(dvec_pushb(&v, &c), 0);
    EXPECT_EQ_INT(dvec_popf(&v, &out), 0);
    EXPECT_EQ_INT(out, 1);
    EXPECT_EQ_UINT(dvec_len(&v), 2);
    EXPECT_EQ_INT(*(int *)dvec_at(&v, 0), 2);
    EXPECT_EQ_INT(*(int *)dvec_at(&v, 1), 3);
    EXPECT_EQ_INT(dvec_popf(&v, NULL), 0);
    EXPECT_EQ_INT(*(int *)dvec_front(&v), 3);
    dvec_free(&v);
}

UTEST_CASE(dvec_ins)
{
    struct dvec v;
    int a = 1, b = 3, mid = 2;

    EXPECT_EQ_INT(dvec_init_empty(&v, sizeof(int)), 0);
    EXPECT_EQ_INT(dvec_ins(&v, 1, &a), -ERANGE);
    EXPECT_EQ_INT(dvec_ins(&v, 0, &a), 0);
    EXPECT_EQ_INT(dvec_ins(&v, 1, &b), 0);
    EXPECT_EQ_INT(dvec_ins(&v, 1, &mid), 0);
    EXPECT_EQ_UINT(dvec_len(&v), 3);
    EXPECT_EQ_INT(*(int *)dvec_at(&v, 0), 1);
    EXPECT_EQ_INT(*(int *)dvec_at(&v, 1), 2);
    EXPECT_EQ_INT(*(int *)dvec_at(&v, 2), 3);
    dvec_free(&v);
}

UTEST_CASE(dvec_del)
{
    struct dvec v;
    int a = 1, b = 2, c = 3, out = 0;

    EXPECT_EQ_INT(dvec_init_empty(&v, sizeof(int)), 0);
    EXPECT_EQ_INT(dvec_del(&v, 0, &out), -ERANGE);
    EXPECT_EQ_INT(dvec_pushb(&v, &a), 0);
    EXPECT_EQ_INT(dvec_pushb(&v, &b), 0);
    EXPECT_EQ_INT(dvec_pushb(&v, &c), 0);
    EXPECT_EQ_INT(dvec_del(&v, 1, &out), 0);
    EXPECT_EQ_INT(out, 2);
    EXPECT_EQ_UINT(dvec_len(&v), 2);
    EXPECT_EQ_INT(*(int *)dvec_at(&v, 0), 1);
    EXPECT_EQ_INT(*(int *)dvec_at(&v, 1), 3);
    EXPECT_EQ_INT(dvec_del(&v, 0, NULL), 0);
    EXPECT_EQ_INT(*(int *)dvec_at(&v, 0), 3);
    EXPECT_EQ_INT(dvec_del(&v, 1, &out), -ERANGE);
    dvec_free(&v);
}

UTEST_CASE(dvec_dup)
{
    struct dvec v, d;
    int a = 1, b = 2;

    EXPECT_EQ_INT(dvec_init_empty(&v, sizeof(int)), 0);
    EXPECT_EQ_INT(dvec_dup(&v, &v), -EINVAL);
    EXPECT_EQ_INT(dvec_pushb(&v, &a), 0);
    EXPECT_EQ_INT(dvec_pushb(&v, &b), 0);
    EXPECT_EQ_INT(dvec_dup(&v, &d), 0);
    EXPECT_EQ_UINT(dvec_len(&d), 2);
    EXPECT_EQ_UINT(dvec_elesz(&d), sizeof(int));
    EXPECT_EQ_INT(*(int *)dvec_at(&d, 0), 1);
    EXPECT_EQ_INT(*(int *)dvec_at(&d, 1), 2);
    EXPECT_NE_PTR(dvec_raw(&d), dvec_raw(&v));
    a = 9;
    EXPECT_EQ_INT(dvec_pushb(&v, &a), 0);
    EXPECT_EQ_UINT(dvec_len(&d), 2);
    dvec_free(&d);
    dvec_free(&v);
}

UTEST_CASE(dvec_sort)
{
    struct dvec v;
    int a = 3, b = 1, c = 2;

    EXPECT_EQ_INT(dvec_init_empty(&v, sizeof(int)), 0);
    dvec_sort(&v, cmp_int);
    EXPECT_TRUE(dvec_empty(&v));
    EXPECT_EQ_INT(dvec_pushb(&v, &a), 0);
    dvec_sort(&v, cmp_int);
    EXPECT_EQ_INT(*(int *)dvec_at(&v, 0), 3);
    EXPECT_EQ_INT(dvec_pushb(&v, &b), 0);
    EXPECT_EQ_INT(dvec_pushb(&v, &c), 0);
    dvec_sort(&v, cmp_int);
    EXPECT_EQ_INT(*(int *)dvec_at(&v, 0), 1);
    EXPECT_EQ_INT(*(int *)dvec_at(&v, 1), 2);
    EXPECT_EQ_INT(*(int *)dvec_at(&v, 2), 3);
    dvec_free(&v);
}

UTEST_SUITE(dvec)
{
    UTEST_RUNCASE(dvec_raw);
    UTEST_RUNCASE(dvec_len);
    UTEST_RUNCASE(dvec_cap);
    UTEST_RUNCASE(dvec_elesz);
    UTEST_RUNCASE(dvec_empty);
    UTEST_RUNCASE(dvec_avail);
    UTEST_RUNCASE(dvec_init);
    UTEST_RUNCASE(dvec_init_empty);
    UTEST_RUNCASE(dvec_free);
    UTEST_RUNCASE(dvec_clear);
    UTEST_RUNCASE(dvec_at);
    UTEST_RUNCASE(dvec_back);
    UTEST_RUNCASE(dvec_front);
    UTEST_RUNCASE(dvec_reserve);
    UTEST_RUNCASE(dvec_shrink);
    UTEST_RUNCASE(dvec_pushb);
    UTEST_RUNCASE(dvec_pushf);
    UTEST_RUNCASE(dvec_popb);
    UTEST_RUNCASE(dvec_popf);
    UTEST_RUNCASE(dvec_ins);
    UTEST_RUNCASE(dvec_del);
    UTEST_RUNCASE(dvec_dup);
    UTEST_RUNCASE(dvec_sort);
}
