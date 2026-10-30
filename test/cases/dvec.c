/* dvec test suite */

#include <dvec.h>
#include <errno.h>
#include <stdlib.h>
#include <utest.h>

static int cmp_int(const void *a, const void *b)
{
    int x = *(const int *)a, y = *(const int *)b;

    return (x > y) - (x < y);
}

UTEST_CASE(dvec_init)
{
    struct dvec v;
    int x = 1;

    EXPECT_EQ_INT(dvec_init(&v, 4, sizeof(int)), 0);
    EXPECT_EQ_UINT(dvec_cap(&v), 4);
    EXPECT_EQ_UINT(dvec_len(&v), 0);
    EXPECT_EQ_UINT(dvec_elesz(&v), sizeof(int));
    EXPECT_NOTNULL(dvec_buf(&v));
    EXPECT_EQ_INT(dvec_pushb(&v, &x), 0);
    EXPECT_EQ_INT(*(int *)dvec_at(&v, 0), 1);
    dvec_free(&v);

    EXPECT_EQ_INT(dvec_init(&v, 0, sizeof(int)), 0);
    EXPECT_EQ_UINT(dvec_cap(&v), 0);
    EXPECT_NULL(dvec_buf(&v));
    dvec_free(&v);

    EXPECT_EQ_INT(dvec_init(&v, 4, 0), -EINVAL);
}

UTEST_CASE(dvec_init_empty)
{
    struct dvec v;
    int x = 7;

    EXPECT_EQ_INT(dvec_init_empty(&v, sizeof(int)), 0);
    EXPECT_EQ_UINT(dvec_len(&v), 0);
    EXPECT_EQ_UINT(dvec_cap(&v), 0);
    EXPECT_NULL(dvec_buf(&v));
    EXPECT_EQ_INT(dvec_pushb(&v, &x), 0);
    EXPECT_EQ_UINT(dvec_len(&v), 1);
    EXPECT_GE_UINT(dvec_cap(&v), 1);
    EXPECT_EQ_INT(*(int *)dvec_at(&v, 0), 7);
    dvec_free(&v);

    EXPECT_EQ_INT(dvec_init_empty(&v, 0), -EINVAL);
}

UTEST_CASE(dvec_free)
{
    struct dvec v;
    int x = 1;

    dvec_init(&v, 2, sizeof(int));
    dvec_pushb(&v, &x);
    dvec_free(&v);
    EXPECT_NULL(dvec_buf(&v));
    EXPECT_EQ_UINT(dvec_len(&v), 0);
    EXPECT_EQ_UINT(dvec_cap(&v), 0);

    dvec_init_empty(&v, sizeof(int));
    dvec_free(&v);
}

UTEST_CASE(dvec_clear)
{
    struct dvec v;
    int a = 1, b = 2;

    dvec_init(&v, 4, sizeof(int));
    dvec_pushb(&v, &a);
    dvec_pushb(&v, &b);
    dvec_clear(&v);
    EXPECT_EQ_UINT(dvec_len(&v), 0);
    EXPECT_EQ_UINT(dvec_cap(&v), 4);
    EXPECT_NOTNULL(dvec_buf(&v));
    dvec_pushb(&v, &b);
    EXPECT_EQ_INT(*(int *)dvec_at(&v, 0), 2);
    dvec_free(&v);
}

UTEST_CASE(dvec_len)
{
    struct dvec v;
    int x = 1;

    dvec_init_empty(&v, sizeof(int));
    EXPECT_EQ_UINT(dvec_len(&v), 0);
    dvec_pushb(&v, &x);
    EXPECT_EQ_UINT(dvec_len(&v), 1);
    dvec_pushb(&v, &x);
    EXPECT_EQ_UINT(dvec_len(&v), 2);
    dvec_free(&v);
}

UTEST_CASE(dvec_cap)
{
    struct dvec v;

    dvec_init(&v, 8, sizeof(int));
    EXPECT_EQ_UINT(dvec_cap(&v), 8);
    dvec_free(&v);
}

UTEST_CASE(dvec_buf)
{
    struct dvec v;

    dvec_init_empty(&v, sizeof(int));
    EXPECT_NULL(dvec_buf(&v));
    dvec_init(&v, 2, sizeof(int));
    EXPECT_NOTNULL(dvec_buf(&v));
    dvec_free(&v);
}

UTEST_CASE(dvec_elesz)
{
    struct dvec v;

    dvec_init_empty(&v, sizeof(int));
    EXPECT_EQ_UINT(dvec_elesz(&v), sizeof(int));
    dvec_free(&v);
}

UTEST_CASE(dvec_avail)
{
    struct dvec v;
    int x = 1;

    dvec_init(&v, 4, sizeof(int));
    EXPECT_EQ_UINT(dvec_avail(&v), 4);
    dvec_pushb(&v, &x);
    EXPECT_EQ_UINT(dvec_avail(&v), 3);
    dvec_free(&v);
}

UTEST_CASE(dvec_at)
{
    struct dvec v;
    int a = 10, b = 20;

    dvec_init_empty(&v, sizeof(int));
    EXPECT_NULL(dvec_at(&v, 0));
    dvec_pushb(&v, &a);
    dvec_pushb(&v, &b);
    EXPECT_EQ_INT(*(int *)dvec_at(&v, 0), 10);
    EXPECT_EQ_INT(*(int *)dvec_at(&v, 1), 20);
    EXPECT_NULL(dvec_at(&v, 2));
    dvec_free(&v);
}

UTEST_CASE(dvec_front)
{
    struct dvec v;
    int a = 1, b = 2;

    dvec_init_empty(&v, sizeof(int));
    EXPECT_NULL(dvec_front(&v));
    dvec_pushb(&v, &a);
    dvec_pushb(&v, &b);
    EXPECT_EQ_INT(*(int *)dvec_front(&v), 1);
    dvec_free(&v);
}

UTEST_CASE(dvec_back)
{
    struct dvec v;
    int a = 1, b = 2;

    dvec_init_empty(&v, sizeof(int));
    EXPECT_NULL(dvec_back(&v));
    dvec_pushb(&v, &a);
    EXPECT_EQ_INT(*(int *)dvec_back(&v), 1);
    dvec_pushb(&v, &b);
    EXPECT_EQ_INT(*(int *)dvec_back(&v), 2);
    dvec_free(&v);
}

UTEST_CASE(dvec_pushb)
{
    struct dvec v;
    int a = 1, b = 2, c = 3;

    dvec_init_empty(&v, sizeof(int));
    EXPECT_EQ_INT(dvec_pushb(&v, &a), 0);
    EXPECT_EQ_INT(dvec_pushb(&v, &b), 0);
    EXPECT_EQ_INT(dvec_pushb(&v, &c), 0);
    EXPECT_EQ_UINT(dvec_len(&v), 3);
    EXPECT_EQ_INT(*(int *)dvec_at(&v, 0), 1);
    EXPECT_EQ_INT(*(int *)dvec_at(&v, 1), 2);
    EXPECT_EQ_INT(*(int *)dvec_at(&v, 2), 3);
    dvec_free(&v);
}

UTEST_CASE(dvec_pushf)
{
    struct dvec v;
    int a = 1, b = 2, c = 3;

    dvec_init_empty(&v, sizeof(int));
    EXPECT_EQ_INT(dvec_pushf(&v, &a), 0);
    EXPECT_EQ_INT(dvec_pushf(&v, &b), 0);
    EXPECT_EQ_INT(dvec_pushf(&v, &c), 0);
    EXPECT_EQ_INT(*(int *)dvec_at(&v, 0), 3);
    EXPECT_EQ_INT(*(int *)dvec_at(&v, 1), 2);
    EXPECT_EQ_INT(*(int *)dvec_at(&v, 2), 1);
    dvec_free(&v);
}

UTEST_CASE(dvec_popb)
{
    struct dvec v;
    int a = 1, b = 2, out;

    dvec_init_empty(&v, sizeof(int));
    EXPECT_EQ_INT(dvec_popb(&v, &out), -EINVAL);
    dvec_pushb(&v, &a);
    dvec_pushb(&v, &b);
    EXPECT_EQ_INT(dvec_popb(&v, &out), 0);
    EXPECT_EQ_INT(out, 2);
    EXPECT_EQ_INT(dvec_popb(&v, &out), 0);
    EXPECT_EQ_INT(out, 1);
    EXPECT_EQ_UINT(dvec_len(&v), 0);
    dvec_free(&v);
}

UTEST_CASE(dvec_popf)
{
    struct dvec v;
    int a = 1, b = 2, c = 3, out;

    dvec_init_empty(&v, sizeof(int));
    EXPECT_EQ_INT(dvec_popf(&v, &out), -EINVAL);
    dvec_pushb(&v, &a);
    dvec_pushb(&v, &b);
    dvec_pushb(&v, &c);
    EXPECT_EQ_INT(dvec_popf(&v, &out), 0);
    EXPECT_EQ_INT(out, 1);
    EXPECT_EQ_INT(*(int *)dvec_at(&v, 0), 2);
    EXPECT_EQ_INT(*(int *)dvec_at(&v, 1), 3);
    EXPECT_EQ_INT(dvec_popf(&v, &out), 0);
    EXPECT_EQ_INT(out, 2);
    EXPECT_EQ_INT(dvec_popf(&v, &out), 0);
    EXPECT_EQ_INT(out, 3);
    EXPECT_EQ_UINT(dvec_len(&v), 0);
    dvec_free(&v);
}

UTEST_CASE(dvec_ins)
{
    struct dvec v;
    int a = 1, b = 3, c = 2;

    dvec_init_empty(&v, sizeof(int));
    EXPECT_EQ_INT(dvec_ins(&v, 1, &a), -EINVAL);
    EXPECT_EQ_INT(dvec_ins(&v, 0, &a), 0);
    EXPECT_EQ_INT(dvec_ins(&v, 1, &b), 0);
    EXPECT_EQ_INT(dvec_ins(&v, 1, &c), 0);
    EXPECT_EQ_INT(*(int *)dvec_at(&v, 0), 1);
    EXPECT_EQ_INT(*(int *)dvec_at(&v, 1), 2);
    EXPECT_EQ_INT(*(int *)dvec_at(&v, 2), 3);
    dvec_free(&v);
}

UTEST_CASE(dvec_del)
{
    struct dvec v;
    int a = 1, b = 2, c = 3;

    dvec_init_empty(&v, sizeof(int));
    EXPECT_EQ_INT(dvec_del(&v, 0), -EINVAL);
    dvec_pushb(&v, &a);
    dvec_pushb(&v, &b);
    dvec_pushb(&v, &c);
    EXPECT_EQ_INT(dvec_del(&v, 1), 0);
    EXPECT_EQ_UINT(dvec_len(&v), 2);
    EXPECT_EQ_INT(*(int *)dvec_at(&v, 0), 1);
    EXPECT_EQ_INT(*(int *)dvec_at(&v, 1), 3);
    EXPECT_EQ_INT(dvec_del(&v, 1), 0);
    EXPECT_EQ_INT(*(int *)dvec_at(&v, 0), 1);
    EXPECT_EQ_INT(dvec_del(&v, 0), 0);
    EXPECT_EQ_UINT(dvec_len(&v), 0);
    dvec_free(&v);
}

UTEST_CASE(dvec_dup)
{
    struct dvec v, d;
    int a = 1, b = 2;

    dvec_init_empty(&v, sizeof(int));
    EXPECT_EQ_INT(dvec_dup(&v, &d), 0);
    EXPECT_EQ_UINT(dvec_len(&d), 0);
    EXPECT_NULL(dvec_buf(&d));
    dvec_free(&d);

    dvec_pushb(&v, &a);
    dvec_pushb(&v, &b);
    EXPECT_EQ_INT(dvec_dup(&v, &d), 0);
    EXPECT_EQ_UINT(dvec_len(&d), 2);
    EXPECT_EQ_INT(*(int *)dvec_at(&d, 0), 1);
    EXPECT_EQ_INT(*(int *)dvec_at(&d, 1), 2);
    EXPECT_NE_PTR(dvec_buf(&d), dvec_buf(&v));
    dvec_free(&d);
    dvec_free(&v);
}

UTEST_CASE(dvec_shrink)
{
    struct dvec v;
    int a = 1, b = 2;

    dvec_init(&v, 8, sizeof(int));
    dvec_pushb(&v, &a);
    dvec_pushb(&v, &b);
    dvec_shrink(&v);
    EXPECT_EQ_UINT(dvec_cap(&v), 2);
    EXPECT_EQ_UINT(dvec_len(&v), 2);
    EXPECT_EQ_INT(*(int *)dvec_at(&v, 1), 2);
    dvec_clear(&v);
    dvec_shrink(&v);
    EXPECT_EQ_UINT(dvec_cap(&v), 0);
    EXPECT_NULL(dvec_buf(&v));
    dvec_free(&v);
}

UTEST_CASE(dvec_reserve)
{
    struct dvec v;
    size_t cap;

    dvec_init_empty(&v, sizeof(int));
    EXPECT_EQ_INT(dvec_reserve(&v, 4), 0);
    EXPECT_EQ_UINT(dvec_cap(&v), 4);
    EXPECT_EQ_UINT(dvec_len(&v), 0);
    EXPECT_NOTNULL(dvec_buf(&v));
    cap = dvec_cap(&v);
    EXPECT_EQ_INT(dvec_reserve(&v, 2), 0);
    EXPECT_EQ_UINT(dvec_cap(&v), cap);
    dvec_free(&v);
}

UTEST_CASE(dvec_sort)
{
    struct dvec v;
    int a = 3, b = 1, c = 2;

    dvec_init_empty(&v, sizeof(int));
    dvec_pushb(&v, &a);
    dvec_sort(&v, cmp_int);
    EXPECT_EQ_INT(*(int *)dvec_at(&v, 0), 3);
    dvec_pushb(&v, &b);
    dvec_pushb(&v, &c);
    dvec_sort(&v, cmp_int);
    EXPECT_EQ_INT(*(int *)dvec_at(&v, 0), 1);
    EXPECT_EQ_INT(*(int *)dvec_at(&v, 1), 2);
    EXPECT_EQ_INT(*(int *)dvec_at(&v, 2), 3);
    dvec_free(&v);
}

UTEST_SUITE(dvec)
{
    UTEST_RUNCASE(dvec_init);
    UTEST_RUNCASE(dvec_init_empty);
    UTEST_RUNCASE(dvec_free);
    UTEST_RUNCASE(dvec_clear);
    UTEST_RUNCASE(dvec_len);
    UTEST_RUNCASE(dvec_cap);
    UTEST_RUNCASE(dvec_buf);
    UTEST_RUNCASE(dvec_elesz);
    UTEST_RUNCASE(dvec_avail);
    UTEST_RUNCASE(dvec_at);
    UTEST_RUNCASE(dvec_front);
    UTEST_RUNCASE(dvec_back);
    UTEST_RUNCASE(dvec_pushb);
    UTEST_RUNCASE(dvec_pushf);
    UTEST_RUNCASE(dvec_popb);
    UTEST_RUNCASE(dvec_popf);
    UTEST_RUNCASE(dvec_ins);
    UTEST_RUNCASE(dvec_del);
    UTEST_RUNCASE(dvec_dup);
    UTEST_RUNCASE(dvec_shrink);
    UTEST_RUNCASE(dvec_reserve);
    UTEST_RUNCASE(dvec_sort);
}
