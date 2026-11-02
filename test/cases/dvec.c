/* Tests for dvec. */

#include <dvec.h>
#include <errno.h>
#include <string.h>
#include <utest.h>

struct rec {
    int id;
    char tag;
    double score;
};

static int cmp_int(const void *a, const void *b)
{
    int x = *(const int *)a;
    int y = *(const int *)b;

    return (x > y) - (x < y);
}

static int cmp_char(const void *a, const void *b)
{
    unsigned char x = *(const unsigned char *)a;
    unsigned char y = *(const unsigned char *)b;

    return (x > y) - (x < y);
}

static int cmp_rec(const void *a, const void *b)
{
    int x = ((const struct rec *)a)->id;
    int y = ((const struct rec *)b)->id;

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

    EXPECT_EQ_INT(dvec_init_empty(&v, sizeof(double)), 0);
    EXPECT_EQ_UINT(dvec_elesz(&v), sizeof(double));
    dvec_free(&v);

    EXPECT_EQ_INT(dvec_init(&v, sizeof(struct rec), 2), 0);
    EXPECT_EQ_UINT(dvec_elesz(&v), sizeof(struct rec));
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
    EXPECT_TRUE(dvec_empty(&v));
    dvec_free(&v);

    EXPECT_EQ_INT(dvec_init(&v, sizeof(char), 1), 0);
    EXPECT_EQ_UINT(dvec_cap(&v), 1);
    EXPECT_EQ_UINT(dvec_elesz(&v), sizeof(char));
    EXPECT_EQ_UINT(dvec_avail(&v), 1);
    dvec_free(&v);

    EXPECT_EQ_INT(dvec_init(&v, sizeof(struct rec), 3), 0);
    EXPECT_EQ_UINT(dvec_cap(&v), 3);
    EXPECT_TRUE(dvec_empty(&v));
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

    EXPECT_EQ_INT(dvec_init_empty(&v, sizeof(double)), 0);
    EXPECT_EQ_UINT(dvec_elesz(&v), sizeof(double));
    dvec_free(&v);

    EXPECT_EQ_INT(dvec_init_empty(&v, sizeof(struct rec)), 0);
    EXPECT_EQ_UINT(dvec_elesz(&v), sizeof(struct rec));
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
    dvec_clear(&v);
    EXPECT_EQ_UINT(dvec_len(&v), 0);
    EXPECT_EQ_UINT(dvec_cap(&v), cap);
    EXPECT_EQ_INT(dvec_pushb(&v, &x), 0);
    dvec_clear(&v);
    EXPECT_TRUE(dvec_empty(&v));
    EXPECT_EQ_INT(dvec_popb(&v, &x), -ENOENT);
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
    *(int *)dvec_at(&v, 1) = 9;
    EXPECT_EQ_INT(*(int *)dvec_at(&v, 1), 9);
    EXPECT_EQ_INT(*(int *)dvec_at(&v, 0), 1);
    dvec_free(&v);

    {
        struct rec a = {1, 'a', 1.5}, b = {2, 'b', 2.5};
        char c = 'q';

        EXPECT_EQ_INT(dvec_init(&v, sizeof(c), 1), 0);
        EXPECT_NULL(dvec_at(&v, 0));
        EXPECT_EQ_INT(dvec_pushb(&v, &c), 0);
        EXPECT_EQ_CHAR(*(char *)dvec_at(&v, 0), 'q');
        EXPECT_NULL(dvec_at(&v, 1));
        EXPECT_EQ_PTR(dvec_at(&v, 0), dvec_front(&v));
        dvec_free(&v);

        EXPECT_EQ_INT(dvec_init_empty(&v, sizeof(a)), 0);
        EXPECT_EQ_INT(dvec_pushb(&v, &a), 0);
        EXPECT_EQ_INT(dvec_pushb(&v, &b), 0);
        EXPECT_EQ_INT(((struct rec *)dvec_at(&v, 0))->id, 1);
        EXPECT_EQ_CHAR(((struct rec *)dvec_at(&v, 1))->tag, 'b');
        EXPECT_EQ_DOUBLE(((struct rec *)dvec_at(&v, 0))->score, 1.5);
        dvec_free(&v);
    }
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
    EXPECT_EQ_PTR(dvec_back(&v), dvec_at(&v, 1));
    dvec_free(&v);

    {
        double x = -1.25;

        EXPECT_EQ_INT(dvec_init_empty(&v, sizeof(x)), 0);
        EXPECT_EQ_INT(dvec_pushf(&v, &x), 0);
        EXPECT_EQ_PTR(dvec_back(&v), dvec_front(&v));
        EXPECT_EQ_DOUBLE(*(double *)dvec_back(&v), -1.25);
        dvec_free(&v);
    }
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
    *(int *)dvec_front(&v) = 6;
    EXPECT_EQ_INT(*(int *)dvec_at(&v, 0), 6);
    dvec_free(&v);

    {
        struct rec r = {3, 'z', 4.5};

        EXPECT_EQ_INT(dvec_init(&v, sizeof(r), 1), 0);
        EXPECT_NULL(dvec_front(&v));
        EXPECT_EQ_INT(dvec_pushf(&v, &r), 0);
        EXPECT_EQ_INT(((struct rec *)dvec_front(&v))->id, 3);
        EXPECT_EQ_CHAR(((struct rec *)dvec_front(&v))->tag, 'z');
        EXPECT_EQ_DOUBLE(((struct rec *)dvec_front(&v))->score, 4.5);
        dvec_free(&v);
    }
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
    EXPECT_EQ_INT(dvec_reserve(&v, dvec_cap(&v)), 0);
    EXPECT_EQ_INT(*(int *)dvec_at(&v, 0), 5);
    dvec_free(&v);

    EXPECT_EQ_INT(dvec_init(&v, sizeof(char), 1), 0);
    EXPECT_EQ_INT(dvec_reserve(&v, 1), 0);
    EXPECT_EQ_UINT(dvec_cap(&v), 1);
    EXPECT_EQ_INT(dvec_reserve(&v, 4), 0);
    EXPECT_GE_UINT(dvec_cap(&v), 4);
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
    dvec_clear(&v);
    dvec_shrink(&v);
    EXPECT_EQ_UINT(dvec_len(&v), 0);
    EXPECT_EQ_UINT(dvec_cap(&v), 0);
    EXPECT_NULL(dvec_raw(&v));
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

    {
        struct rec a = {1, 'a', 1.5}, b = {2, 'b', -2.0};
        char bytes[4] = {'w', 'x', 'y', 'z'};
        int i;

        EXPECT_EQ_INT(dvec_init(&v, sizeof(a), 1), 0);
        EXPECT_EQ_INT(dvec_pushb(&v, &a), 0);
        EXPECT_EQ_UINT(dvec_avail(&v), 0);
        EXPECT_EQ_INT(dvec_pushb(&v, &b), 0);
        EXPECT_EQ_INT(((struct rec *)dvec_at(&v, 0))->id, 1);
        EXPECT_EQ_DOUBLE(((struct rec *)dvec_at(&v, 1))->score, -2.0);
        EXPECT_EQ_CHAR(((struct rec *)dvec_back(&v))->tag, 'b');
        dvec_free(&v);

        EXPECT_EQ_INT(dvec_init_empty(&v, sizeof(char)), 0);
        for (i = 0; i < 4; i++)
            EXPECT_EQ_INT(dvec_pushb(&v, &bytes[i]), 0);
        for (i = 0; i < 4; i++)
            EXPECT_EQ_CHAR(*(char *)dvec_at(&v, (size_t)i), bytes[i]);
        dvec_free(&v);
    }
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

    {
        double x = 1.5, y = -0.25;
        char c = 'm', e = 'n';

        EXPECT_EQ_INT(dvec_init(&v, sizeof(c), 1), 0);
        EXPECT_EQ_INT(dvec_pushf(&v, &c), 0);
        EXPECT_EQ_INT(dvec_pushf(&v, &e), 0);
        EXPECT_EQ_CHAR(*(char *)dvec_front(&v), 'n');
        EXPECT_EQ_CHAR(*(char *)dvec_back(&v), 'm');
        dvec_free(&v);

        EXPECT_EQ_INT(dvec_init_empty(&v, sizeof(double)), 0);
        EXPECT_EQ_INT(dvec_pushf(&v, &x), 0);
        EXPECT_EQ_INT(dvec_pushf(&v, &y), 0);
        EXPECT_EQ_DOUBLE(*(double *)dvec_at(&v, 0), -0.25);
        EXPECT_EQ_DOUBLE(*(double *)dvec_at(&v, 1), 1.5);
        dvec_free(&v);
    }
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
    EXPECT_EQ_INT(dvec_popb(&v, &out), -ENOENT);
    dvec_free(&v);

    {
        struct rec a = {1, 'a', 1.0}, b = {2, 'b', 2.0}, got;

        EXPECT_EQ_INT(dvec_init_empty(&v, sizeof(a)), 0);
        EXPECT_EQ_INT(dvec_pushb(&v, &a), 0);
        EXPECT_EQ_INT(dvec_pushf(&v, &b), 0);
        EXPECT_EQ_INT(dvec_popb(&v, &got), 0);
        EXPECT_EQ_INT(got.id, 1);
        EXPECT_EQ_CHAR(got.tag, 'a');
        EXPECT_EQ_INT(dvec_popb(&v, &got), 0);
        EXPECT_EQ_INT(got.id, 2);
        EXPECT_EQ_DOUBLE(got.score, 2.0);
        EXPECT_TRUE(dvec_empty(&v));
        dvec_free(&v);
    }
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
    EXPECT_EQ_INT(dvec_popf(&v, &out), 0);
    EXPECT_EQ_INT(out, 3);
    EXPECT_NULL(dvec_front(&v));
    EXPECT_EQ_INT(dvec_popf(&v, &out), -ENOENT);
    dvec_free(&v);

    {
        double x = 0.5, y = -2.5, got;

        EXPECT_EQ_INT(dvec_init(&v, sizeof(double), 1), 0);
        EXPECT_EQ_INT(dvec_pushb(&v, &x), 0);
        EXPECT_EQ_INT(dvec_pushb(&v, &y), 0);
        EXPECT_EQ_INT(dvec_popf(&v, &got), 0);
        EXPECT_EQ_DOUBLE(got, 0.5);
        EXPECT_EQ_INT(dvec_popf(&v, &got), 0);
        EXPECT_EQ_DOUBLE(got, -2.5);
        dvec_free(&v);
    }
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
    EXPECT_EQ_INT(dvec_ins(&v, 0, &b), 0);
    EXPECT_EQ_INT(*(int *)dvec_front(&v), 3);
    EXPECT_EQ_INT(dvec_ins(&v, dvec_len(&v), &a), 0);
    EXPECT_EQ_INT(*(int *)dvec_back(&v), 1);
    EXPECT_EQ_INT(dvec_ins(&v, dvec_len(&v) + 1, &a), -ERANGE);
    dvec_free(&v);

    {
        struct rec a = {1, 'a', 1.0}, b = {9, 'z', 9.0};

        EXPECT_EQ_INT(dvec_init(&v, sizeof(a), 1), 0);
        EXPECT_EQ_INT(dvec_ins(&v, 0, &a), 0);
        EXPECT_EQ_INT(dvec_ins(&v, 1, &b), 0);
        EXPECT_EQ_INT(((struct rec *)dvec_at(&v, 1))->id, 9);
        EXPECT_EQ_CHAR(((struct rec *)dvec_back(&v))->tag, 'z');
        dvec_free(&v);
    }
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
    EXPECT_EQ_INT(dvec_del(&v, 0, &out), 0);
    EXPECT_EQ_INT(out, 3);
    EXPECT_TRUE(dvec_empty(&v));
    EXPECT_EQ_INT(dvec_del(&v, 0, &out), -ERANGE);
    dvec_free(&v);

    {
        char a = 'a', b = 'b', c = 'c', out = 0;

        EXPECT_EQ_INT(dvec_init_empty(&v, sizeof(char)), 0);
        EXPECT_EQ_INT(dvec_pushb(&v, &a), 0);
        EXPECT_EQ_INT(dvec_pushb(&v, &b), 0);
        EXPECT_EQ_INT(dvec_pushb(&v, &c), 0);
        EXPECT_EQ_INT(dvec_del(&v, 2, &out), 0);
        EXPECT_EQ_CHAR(out, 'c');
        EXPECT_EQ_INT(dvec_del(&v, 0, &out), 0);
        EXPECT_EQ_CHAR(out, 'a');
        EXPECT_EQ_CHAR(*(char *)dvec_front(&v), 'b');
        EXPECT_EQ_UINT(dvec_len(&v), 1);
        dvec_free(&v);
    }
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

    EXPECT_EQ_INT(dvec_init_empty(&v, sizeof(double)), 0);
    EXPECT_EQ_INT(dvec_dup(&v, &d), 0);
    EXPECT_TRUE(dvec_empty(&d));
    EXPECT_EQ_UINT(dvec_elesz(&d), sizeof(double));
    {
        double x = 4.0;

        EXPECT_EQ_INT(dvec_pushb(&v, &x), 0);
    }
    EXPECT_TRUE(dvec_empty(&d));
    dvec_free(&d);
    dvec_free(&v);

    {
        struct rec r = {5, 'k', 3.25}, changed = {0, 'x', 0.0};

        EXPECT_EQ_INT(dvec_init_empty(&v, sizeof(r)), 0);
        EXPECT_EQ_INT(dvec_pushb(&v, &r), 0);
        EXPECT_EQ_INT(dvec_dup(&v, &d), 0);
        ((struct rec *)dvec_at(&v, 0))->id = 99;
        EXPECT_EQ_INT(((struct rec *)dvec_at(&d, 0))->id, 5);
        EXPECT_EQ_CHAR(((struct rec *)dvec_at(&d, 0))->tag, 'k');
        EXPECT_EQ_DOUBLE(((struct rec *)dvec_at(&d, 0))->score, 3.25);
        EXPECT_EQ_INT(dvec_pushb(&d, &changed), 0);
        EXPECT_EQ_UINT(dvec_len(&v), 1);
        dvec_free(&d);
        dvec_free(&v);
    }
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
    a = 2;
    b = 2;
    c = 2;
    EXPECT_EQ_INT(dvec_pushb(&v, &a), 0);
    EXPECT_EQ_INT(dvec_pushb(&v, &b), 0);
    EXPECT_EQ_INT(dvec_pushb(&v, &c), 0);
    dvec_sort(&v, cmp_int);
    EXPECT_EQ_INT(*(int *)dvec_at(&v, 0), 1);
    EXPECT_EQ_INT(*(int *)dvec_at(&v, 5), 3);
    dvec_free(&v);

    {
        char x = 'c', y = 'a', z = 'b';
        struct rec ra = {3, 'c', 1.0}, rb = {1, 'a', 2.0}, rc = {1, 'b', 3.0};
        int rev[4] = {4, 3, 2, 1};
        int i;

        EXPECT_EQ_INT(dvec_init_empty(&v, sizeof(char)), 0);
        EXPECT_EQ_INT(dvec_pushb(&v, &x), 0);
        EXPECT_EQ_INT(dvec_pushb(&v, &y), 0);
        EXPECT_EQ_INT(dvec_pushb(&v, &z), 0);
        dvec_sort(&v, cmp_char);
        EXPECT_EQ_CHAR(*(char *)dvec_at(&v, 0), 'a');
        EXPECT_EQ_CHAR(*(char *)dvec_at(&v, 1), 'b');
        EXPECT_EQ_CHAR(*(char *)dvec_at(&v, 2), 'c');
        dvec_free(&v);

        EXPECT_EQ_INT(dvec_init_empty(&v, sizeof(int)), 0);
        for (i = 0; i < 4; i++)
            EXPECT_EQ_INT(dvec_pushb(&v, &rev[i]), 0);
        dvec_sort(&v, cmp_int);
        for (i = 0; i < 4; i++)
            EXPECT_EQ_INT(*(int *)dvec_at(&v, (size_t)i), i + 1);
        dvec_free(&v);

        EXPECT_EQ_INT(dvec_init_empty(&v, sizeof(ra)), 0);
        EXPECT_EQ_INT(dvec_pushb(&v, &ra), 0);
        EXPECT_EQ_INT(dvec_pushb(&v, &rb), 0);
        EXPECT_EQ_INT(dvec_pushb(&v, &rc), 0);
        dvec_sort(&v, cmp_rec);
        EXPECT_EQ_INT(((struct rec *)dvec_at(&v, 0))->id, 1);
        EXPECT_EQ_INT(((struct rec *)dvec_at(&v, 2))->id, 3);
        EXPECT_EQ_CHAR(((struct rec *)dvec_at(&v, 2))->tag, 'c');
        dvec_free(&v);
    }
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
