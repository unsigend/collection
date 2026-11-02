/* Tests for deque. */

#include <deque.h>
#include <errno.h>
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

UTEST_CASE(deque_len)
{
    struct deque d;
    int x = 1;

    EXPECT_EQ_INT(deque_init_empty(&d, sizeof(int)), 0);
    EXPECT_EQ_UINT(deque_len(&d), 0);
    EXPECT_EQ_INT(deque_pushb(&d, &x), 0);
    EXPECT_EQ_UINT(deque_len(&d), 1);
    deque_free(&d);
}

UTEST_CASE(deque_cap)
{
    struct deque d;

    EXPECT_EQ_INT(deque_init(&d, sizeof(int), 4), 0);
    EXPECT_EQ_UINT(deque_cap(&d), 4);
    EXPECT_GE_UINT(deque_cap(&d), deque_len(&d));
    deque_free(&d);
}

UTEST_CASE(deque_elesz)
{
    struct deque d;

    EXPECT_EQ_INT(deque_init_empty(&d, sizeof(int)), 0);
    EXPECT_EQ_UINT(deque_elesz(&d), sizeof(int));
    deque_free(&d);

    EXPECT_EQ_INT(deque_init_empty(&d, sizeof(double)), 0);
    EXPECT_EQ_UINT(deque_elesz(&d), sizeof(double));
    deque_free(&d);

    EXPECT_EQ_INT(deque_init(&d, sizeof(struct rec), 2), 0);
    EXPECT_EQ_UINT(deque_elesz(&d), sizeof(struct rec));
    deque_free(&d);
}

UTEST_CASE(deque_empty)
{
    struct deque d;
    int x = 1;

    EXPECT_EQ_INT(deque_init_empty(&d, sizeof(int)), 0);
    EXPECT_TRUE(deque_empty(&d));
    EXPECT_EQ_INT(deque_pushb(&d, &x), 0);
    EXPECT_FALSE(deque_empty(&d));
    deque_free(&d);
}

UTEST_CASE(deque_full)
{
    struct deque d;
    int a = 1, b = 2;

    EXPECT_EQ_INT(deque_init(&d, sizeof(int), 2), 0);
    EXPECT_FALSE(deque_full(&d));
    EXPECT_EQ_INT(deque_pushb(&d, &a), 0);
    EXPECT_EQ_INT(deque_pushb(&d, &b), 0);
    EXPECT_TRUE(deque_full(&d));
    deque_free(&d);
}

UTEST_CASE(deque_avail)
{
    struct deque d;
    int x = 1;

    EXPECT_EQ_INT(deque_init(&d, sizeof(int), 4), 0);
    EXPECT_EQ_UINT(deque_avail(&d), 4);
    EXPECT_EQ_INT(deque_pushb(&d, &x), 0);
    EXPECT_EQ_UINT(deque_avail(&d), deque_cap(&d) - deque_len(&d));
    deque_free(&d);
}

UTEST_CASE(deque_head)
{
    struct deque d;
    int x = 1;

    EXPECT_EQ_INT(deque_init(&d, sizeof(int), 4), 0);
    EXPECT_EQ_UINT(deque_head(&d), 0);
    EXPECT_EQ_INT(deque_pushf(&d, &x), 0);
    EXPECT_EQ_UINT(deque_head(&d), 3);
    deque_free(&d);
}

UTEST_CASE(deque_init)
{
    struct deque d;

    EXPECT_EQ_INT(deque_init(&d, 0, 4), -EINVAL);

    EXPECT_EQ_INT(deque_init(&d, sizeof(int), 4), 0);
    EXPECT_NOTNULL(d.buf);
    EXPECT_EQ_UINT(deque_len(&d), 0);
    EXPECT_EQ_UINT(deque_cap(&d), 4);
    EXPECT_EQ_UINT(deque_elesz(&d), sizeof(int));
    EXPECT_EQ_UINT(deque_head(&d), 0);
    EXPECT_TRUE(deque_empty(&d));
    deque_free(&d);

    EXPECT_EQ_INT(deque_init(&d, sizeof(int), 0), 0);
    EXPECT_NULL(d.buf);
    EXPECT_EQ_UINT(deque_cap(&d), 0);
    EXPECT_TRUE(deque_empty(&d));
    EXPECT_TRUE(deque_full(&d));
    deque_free(&d);

    EXPECT_EQ_INT(deque_init(&d, sizeof(char), 1), 0);
    EXPECT_EQ_UINT(deque_cap(&d), 1);
    EXPECT_EQ_UINT(deque_elesz(&d), sizeof(char));
    EXPECT_FALSE(deque_full(&d));
    deque_free(&d);

    EXPECT_EQ_INT(deque_init(&d, sizeof(struct rec), 3), 0);
    EXPECT_EQ_UINT(deque_cap(&d), 3);
    EXPECT_EQ_UINT(deque_head(&d), 0);
    deque_free(&d);
}

UTEST_CASE(deque_init_empty)
{
    struct deque d;

    EXPECT_EQ_INT(deque_init_empty(&d, 0), -EINVAL);

    EXPECT_EQ_INT(deque_init_empty(&d, sizeof(int)), 0);
    EXPECT_NULL(d.buf);
    EXPECT_EQ_UINT(deque_len(&d), 0);
    EXPECT_EQ_UINT(deque_cap(&d), 0);
    EXPECT_TRUE(deque_empty(&d));
    EXPECT_TRUE(deque_full(&d));
    deque_free(&d);

    EXPECT_EQ_INT(deque_init_empty(&d, sizeof(double)), 0);
    EXPECT_EQ_UINT(deque_elesz(&d), sizeof(double));
    deque_free(&d);

    EXPECT_EQ_INT(deque_init_empty(&d, sizeof(struct rec)), 0);
    EXPECT_EQ_UINT(deque_elesz(&d), sizeof(struct rec));
    deque_free(&d);
}

UTEST_CASE(deque_free)
{
    struct deque d;
    int x = 1;

    deque_free(NULL);

    EXPECT_EQ_INT(deque_init_empty(&d, sizeof(int)), 0);
    EXPECT_EQ_INT(deque_pushb(&d, &x), 0);
    deque_free(&d);
    EXPECT_NULL(d.buf);
    EXPECT_EQ_UINT(deque_len(&d), 0);
    EXPECT_EQ_UINT(deque_cap(&d), 0);
}

UTEST_CASE(deque_clear)
{
    struct deque d;
    int x = 7;
    size_t cap;

    EXPECT_EQ_INT(deque_init(&d, sizeof(int), 4), 0);
    EXPECT_EQ_INT(deque_pushb(&d, &x), 0);
    cap = deque_cap(&d);
    deque_clear(&d);
    EXPECT_EQ_UINT(deque_len(&d), 0);
    EXPECT_TRUE(deque_empty(&d));
    EXPECT_EQ_UINT(deque_cap(&d), cap);
    EXPECT_NOTNULL(d.buf);
    EXPECT_EQ_INT(deque_popf(&d, &x), -ENOENT);
    deque_clear(&d);
    EXPECT_EQ_UINT(deque_len(&d), 0);
    EXPECT_EQ_UINT(deque_cap(&d), cap);
    EXPECT_EQ_INT(deque_pushb(&d, &x), 0);
    deque_clear(&d);
    EXPECT_TRUE(deque_empty(&d));
    EXPECT_EQ_UINT(deque_cap(&d), cap);
    deque_free(&d);
}

UTEST_CASE(deque_at)
{
    struct deque d;
    int a = 1, b = 2, c = 3;

    EXPECT_EQ_INT(deque_init_empty(&d, sizeof(int)), 0);
    EXPECT_NULL(deque_at(&d, 0));
    EXPECT_EQ_INT(deque_pushb(&d, &a), 0);
    EXPECT_EQ_INT(deque_pushb(&d, &b), 0);
    EXPECT_EQ_INT(deque_pushb(&d, &c), 0);
    EXPECT_EQ_INT(*(int *)deque_at(&d, 0), 1);
    EXPECT_EQ_INT(*(int *)deque_at(&d, 1), 2);
    EXPECT_EQ_INT(*(int *)deque_at(&d, 2), 3);
    EXPECT_NULL(deque_at(&d, 3));
    *(int *)deque_at(&d, 1) = 9;
    EXPECT_EQ_INT(*(int *)deque_at(&d, 1), 9);
    EXPECT_EQ_INT(*(int *)deque_at(&d, 0), 1);
    deque_free(&d);

    {
        struct rec a = {1, 'a', 1.5}, b = {2, 'b', 2.5};
        char c = 'q';

        EXPECT_EQ_INT(deque_init(&d, sizeof(c), 1), 0);
        EXPECT_NULL(deque_at(&d, 0));
        EXPECT_EQ_INT(deque_pushb(&d, &c), 0);
        EXPECT_EQ_CHAR(*(char *)deque_at(&d, 0), 'q');
        EXPECT_NULL(deque_at(&d, 1));
        EXPECT_EQ_PTR(deque_at(&d, 0), deque_front(&d));
        deque_free(&d);

        EXPECT_EQ_INT(deque_init_empty(&d, sizeof(a)), 0);
        EXPECT_EQ_INT(deque_pushb(&d, &a), 0);
        EXPECT_EQ_INT(deque_pushb(&d, &b), 0);
        EXPECT_EQ_INT(((struct rec *)deque_at(&d, 0))->id, 1);
        EXPECT_EQ_CHAR(((struct rec *)deque_at(&d, 1))->tag, 'b');
        EXPECT_EQ_DOUBLE(((struct rec *)deque_at(&d, 0))->score, 1.5);
        deque_free(&d);
    }
}

UTEST_CASE(deque_back)
{
    struct deque d;
    int a = 1, b = 2;

    EXPECT_EQ_INT(deque_init_empty(&d, sizeof(int)), 0);
    EXPECT_NULL(deque_back(&d));
    EXPECT_EQ_INT(deque_pushb(&d, &a), 0);
    EXPECT_EQ_INT(*(int *)deque_back(&d), 1);
    EXPECT_EQ_INT(deque_pushb(&d, &b), 0);
    EXPECT_EQ_INT(*(int *)deque_back(&d), 2);
    EXPECT_EQ_PTR(deque_back(&d), deque_at(&d, 1));
    deque_free(&d);

    {
        double x = -1.25;

        EXPECT_EQ_INT(deque_init_empty(&d, sizeof(x)), 0);
        EXPECT_EQ_INT(deque_pushf(&d, &x), 0);
        EXPECT_EQ_PTR(deque_back(&d), deque_front(&d));
        EXPECT_EQ_DOUBLE(*(double *)deque_back(&d), -1.25);
        deque_free(&d);
    }
}

UTEST_CASE(deque_front)
{
    struct deque d;
    int a = 1, b = 2;

    EXPECT_EQ_INT(deque_init_empty(&d, sizeof(int)), 0);
    EXPECT_NULL(deque_front(&d));
    EXPECT_EQ_INT(deque_pushb(&d, &a), 0);
    EXPECT_EQ_INT(*(int *)deque_front(&d), 1);
    EXPECT_EQ_INT(deque_pushb(&d, &b), 0);
    EXPECT_EQ_INT(*(int *)deque_front(&d), 1);
    *(int *)deque_front(&d) = 6;
    EXPECT_EQ_INT(*(int *)deque_at(&d, 0), 6);
    deque_free(&d);

    {
        struct rec r = {3, 'z', 4.5};

        EXPECT_EQ_INT(deque_init(&d, sizeof(r), 1), 0);
        EXPECT_NULL(deque_front(&d));
        EXPECT_EQ_INT(deque_pushf(&d, &r), 0);
        EXPECT_EQ_INT(((struct rec *)deque_front(&d))->id, 3);
        EXPECT_EQ_CHAR(((struct rec *)deque_front(&d))->tag, 'z');
        EXPECT_EQ_DOUBLE(((struct rec *)deque_front(&d))->score, 4.5);
        deque_free(&d);
    }
}

UTEST_CASE(deque_reserve)
{
    struct deque d;
    int x = 5;

    EXPECT_EQ_INT(deque_init_empty(&d, sizeof(int)), 0);
    EXPECT_EQ_INT(deque_reserve(&d, 8), 0);
    EXPECT_GE_UINT(deque_cap(&d), 8);
    EXPECT_EQ_UINT(deque_len(&d), 0);
    EXPECT_EQ_INT(deque_pushb(&d, &x), 0);
    EXPECT_EQ_INT(deque_reserve(&d, 1), 0);
    EXPECT_GE_UINT(deque_cap(&d), 8);
    EXPECT_EQ_INT(*(int *)deque_at(&d, 0), 5);
    EXPECT_EQ_INT(deque_reserve(&d, deque_cap(&d)), 0);
    EXPECT_EQ_INT(*(int *)deque_at(&d, 0), 5);
    deque_free(&d);

    EXPECT_EQ_INT(deque_init(&d, sizeof(char), 1), 0);
    EXPECT_EQ_INT(deque_reserve(&d, 1), 0);
    EXPECT_EQ_UINT(deque_cap(&d), 1);
    EXPECT_EQ_INT(deque_reserve(&d, 4), 0);
    EXPECT_GE_UINT(deque_cap(&d), 4);
    deque_free(&d);
}

UTEST_CASE(deque_shrink)
{
    struct deque d;
    int x = 9;

    EXPECT_EQ_INT(deque_init(&d, sizeof(int), 8), 0);
    deque_shrink(&d);
    EXPECT_EQ_UINT(deque_cap(&d), 0);
    EXPECT_NULL(d.buf);

    EXPECT_EQ_INT(deque_init(&d, sizeof(int), 8), 0);
    EXPECT_EQ_INT(deque_pushb(&d, &x), 0);
    deque_shrink(&d);
    EXPECT_EQ_UINT(deque_cap(&d), 1);
    EXPECT_EQ_UINT(deque_len(&d), 1);
    EXPECT_EQ_INT(*(int *)deque_at(&d, 0), 9);
    deque_shrink(&d);
    EXPECT_EQ_UINT(deque_cap(&d), 1);
    deque_clear(&d);
    deque_shrink(&d);
    EXPECT_EQ_UINT(deque_len(&d), 0);
    EXPECT_EQ_UINT(deque_cap(&d), 0);
    EXPECT_NULL(d.buf);
    deque_free(&d);
}

UTEST_CASE(deque_pushb)
{
    struct deque d;
    int a = 1, b = 2, c = 3;

    EXPECT_EQ_INT(deque_init_empty(&d, sizeof(int)), 0);
    EXPECT_EQ_INT(deque_pushb(&d, &a), 0);
    EXPECT_EQ_INT(deque_pushb(&d, &b), 0);
    EXPECT_EQ_INT(deque_pushb(&d, &c), 0);
    EXPECT_EQ_UINT(deque_len(&d), 3);
    EXPECT_GE_UINT(deque_cap(&d), 3);
    EXPECT_EQ_INT(*(int *)deque_at(&d, 0), 1);
    EXPECT_EQ_INT(*(int *)deque_at(&d, 1), 2);
    EXPECT_EQ_INT(*(int *)deque_at(&d, 2), 3);
    deque_free(&d);

    {
        struct rec a = {1, 'a', 1.5}, b = {2, 'b', -2.0};
        char bytes[4] = {'w', 'x', 'y', 'z'};
        int i;

        EXPECT_EQ_INT(deque_init(&d, sizeof(a), 1), 0);
        EXPECT_EQ_INT(deque_pushb(&d, &a), 0);
        EXPECT_TRUE(deque_full(&d));
        EXPECT_EQ_INT(deque_pushb(&d, &b), 0);
        EXPECT_EQ_INT(((struct rec *)deque_at(&d, 0))->id, 1);
        EXPECT_EQ_DOUBLE(((struct rec *)deque_at(&d, 1))->score, -2.0);
        EXPECT_EQ_CHAR(((struct rec *)deque_back(&d))->tag, 'b');
        deque_free(&d);

        EXPECT_EQ_INT(deque_init_empty(&d, sizeof(char)), 0);
        for (i = 0; i < 4; i++)
            EXPECT_EQ_INT(deque_pushb(&d, &bytes[i]), 0);
        for (i = 0; i < 4; i++)
            EXPECT_EQ_CHAR(*(char *)deque_at(&d, (size_t)i), bytes[i]);
        deque_free(&d);
    }
}

UTEST_CASE(deque_pushf)
{
    struct deque d;
    int a = 1, b = 2, c = 3;

    EXPECT_EQ_INT(deque_init_empty(&d, sizeof(int)), 0);
    EXPECT_EQ_INT(deque_pushf(&d, &a), 0);
    EXPECT_EQ_INT(deque_pushf(&d, &b), 0);
    EXPECT_EQ_INT(deque_pushf(&d, &c), 0);
    EXPECT_EQ_UINT(deque_len(&d), 3);
    EXPECT_EQ_INT(*(int *)deque_at(&d, 0), 3);
    EXPECT_EQ_INT(*(int *)deque_at(&d, 1), 2);
    EXPECT_EQ_INT(*(int *)deque_at(&d, 2), 1);
    deque_free(&d);

    {
        double x = 1.5, y = -0.25;
        char c = 'm', e = 'n';

        EXPECT_EQ_INT(deque_init(&d, sizeof(c), 1), 0);
        EXPECT_EQ_INT(deque_pushf(&d, &c), 0);
        EXPECT_EQ_INT(deque_pushf(&d, &e), 0);
        EXPECT_EQ_CHAR(*(char *)deque_front(&d), 'n');
        EXPECT_EQ_CHAR(*(char *)deque_back(&d), 'm');
        deque_free(&d);

        EXPECT_EQ_INT(deque_init_empty(&d, sizeof(double)), 0);
        EXPECT_EQ_INT(deque_pushf(&d, &x), 0);
        EXPECT_EQ_INT(deque_pushf(&d, &y), 0);
        EXPECT_EQ_DOUBLE(*(double *)deque_at(&d, 0), -0.25);
        EXPECT_EQ_DOUBLE(*(double *)deque_at(&d, 1), 1.5);
        deque_free(&d);
    }
}

UTEST_CASE(deque_popb)
{
    struct deque d;
    int a = 1, b = 2, out = 0;

    EXPECT_EQ_INT(deque_init_empty(&d, sizeof(int)), 0);
    EXPECT_EQ_INT(deque_popb(&d, &out), -ENOENT);
    EXPECT_EQ_INT(deque_pushb(&d, &a), 0);
    EXPECT_EQ_INT(deque_pushb(&d, &b), 0);
    EXPECT_EQ_INT(deque_popb(&d, &out), 0);
    EXPECT_EQ_INT(out, 2);
    EXPECT_EQ_UINT(deque_len(&d), 1);
    EXPECT_EQ_INT(deque_popb(&d, NULL), 0);
    EXPECT_TRUE(deque_empty(&d));
    EXPECT_EQ_INT(deque_popb(&d, &out), -ENOENT);
    deque_free(&d);

    {
        struct rec a = {1, 'a', 1.0}, b = {2, 'b', 2.0}, got;

        EXPECT_EQ_INT(deque_init_empty(&d, sizeof(a)), 0);
        EXPECT_EQ_INT(deque_pushb(&d, &a), 0);
        EXPECT_EQ_INT(deque_pushf(&d, &b), 0);
        EXPECT_EQ_INT(deque_popb(&d, &got), 0);
        EXPECT_EQ_INT(got.id, 1);
        EXPECT_EQ_CHAR(got.tag, 'a');
        EXPECT_EQ_INT(deque_popb(&d, &got), 0);
        EXPECT_EQ_INT(got.id, 2);
        EXPECT_EQ_DOUBLE(got.score, 2.0);
        EXPECT_TRUE(deque_empty(&d));
        deque_free(&d);
    }
}

UTEST_CASE(deque_popf)
{
    struct deque d;
    int a = 1, b = 2, c = 3, out = 0;

    EXPECT_EQ_INT(deque_init_empty(&d, sizeof(int)), 0);
    EXPECT_EQ_INT(deque_popf(&d, &out), -ENOENT);
    EXPECT_EQ_INT(deque_pushb(&d, &a), 0);
    EXPECT_EQ_INT(deque_pushb(&d, &b), 0);
    EXPECT_EQ_INT(deque_pushb(&d, &c), 0);
    EXPECT_EQ_INT(deque_popf(&d, &out), 0);
    EXPECT_EQ_INT(out, 1);
    EXPECT_EQ_UINT(deque_len(&d), 2);
    EXPECT_EQ_INT(*(int *)deque_at(&d, 0), 2);
    EXPECT_EQ_INT(*(int *)deque_at(&d, 1), 3);
    EXPECT_EQ_INT(deque_popf(&d, NULL), 0);
    EXPECT_EQ_INT(*(int *)deque_front(&d), 3);
    EXPECT_EQ_INT(deque_popf(&d, &out), 0);
    EXPECT_EQ_INT(out, 3);
    EXPECT_NULL(deque_front(&d));
    EXPECT_EQ_INT(deque_popf(&d, &out), -ENOENT);
    deque_free(&d);

    {
        double x = 0.5, y = -2.5, got;

        EXPECT_EQ_INT(deque_init(&d, sizeof(double), 1), 0);
        EXPECT_EQ_INT(deque_pushb(&d, &x), 0);
        EXPECT_EQ_INT(deque_pushb(&d, &y), 0);
        EXPECT_EQ_INT(deque_popf(&d, &got), 0);
        EXPECT_EQ_DOUBLE(got, 0.5);
        EXPECT_EQ_INT(deque_popf(&d, &got), 0);
        EXPECT_EQ_DOUBLE(got, -2.5);
        deque_free(&d);
    }
}

UTEST_CASE(deque_ins)
{
    struct deque d;
    int a = 1, b = 3, mid = 2;

    EXPECT_EQ_INT(deque_init_empty(&d, sizeof(int)), 0);
    EXPECT_EQ_INT(deque_ins(&d, 1, &a), -ERANGE);
    EXPECT_EQ_INT(deque_ins(&d, 0, &a), 0);
    EXPECT_EQ_INT(deque_ins(&d, 1, &b), 0);
    EXPECT_EQ_INT(deque_ins(&d, 1, &mid), 0);
    EXPECT_EQ_UINT(deque_len(&d), 3);
    EXPECT_EQ_INT(*(int *)deque_at(&d, 0), 1);
    EXPECT_EQ_INT(*(int *)deque_at(&d, 1), 2);
    EXPECT_EQ_INT(*(int *)deque_at(&d, 2), 3);
    EXPECT_EQ_INT(deque_ins(&d, 0, &b), 0);
    EXPECT_EQ_INT(*(int *)deque_front(&d), 3);
    EXPECT_EQ_INT(deque_ins(&d, deque_len(&d), &a), 0);
    EXPECT_EQ_INT(*(int *)deque_back(&d), 1);
    EXPECT_EQ_INT(deque_ins(&d, deque_len(&d) + 1, &a), -ERANGE);
    deque_free(&d);

    {
        struct rec a = {1, 'a', 1.0}, b = {9, 'z', 9.0};

        EXPECT_EQ_INT(deque_init(&d, sizeof(a), 1), 0);
        EXPECT_EQ_INT(deque_ins(&d, 0, &a), 0);
        EXPECT_EQ_INT(deque_ins(&d, 1, &b), 0);
        EXPECT_EQ_INT(((struct rec *)deque_at(&d, 1))->id, 9);
        EXPECT_EQ_CHAR(((struct rec *)deque_back(&d))->tag, 'z');
        deque_free(&d);
    }
}

UTEST_CASE(deque_del)
{
    struct deque d;
    int a = 1, b = 2, c = 3, out = 0;

    EXPECT_EQ_INT(deque_init_empty(&d, sizeof(int)), 0);
    EXPECT_EQ_INT(deque_del(&d, 0, &out), -ERANGE);
    EXPECT_EQ_INT(deque_pushb(&d, &a), 0);
    EXPECT_EQ_INT(deque_pushb(&d, &b), 0);
    EXPECT_EQ_INT(deque_pushb(&d, &c), 0);
    EXPECT_EQ_INT(deque_del(&d, 1, &out), 0);
    EXPECT_EQ_INT(out, 2);
    EXPECT_EQ_UINT(deque_len(&d), 2);
    EXPECT_EQ_INT(*(int *)deque_at(&d, 0), 1);
    EXPECT_EQ_INT(*(int *)deque_at(&d, 1), 3);
    EXPECT_EQ_INT(deque_del(&d, 0, NULL), 0);
    EXPECT_EQ_INT(*(int *)deque_at(&d, 0), 3);
    EXPECT_EQ_INT(deque_del(&d, 1, &out), -ERANGE);
    EXPECT_EQ_INT(deque_del(&d, 0, &out), 0);
    EXPECT_EQ_INT(out, 3);
    EXPECT_TRUE(deque_empty(&d));
    EXPECT_EQ_INT(deque_del(&d, 0, &out), -ERANGE);
    deque_free(&d);

    {
        char a = 'a', b = 'b', c = 'c', out = 0;

        EXPECT_EQ_INT(deque_init_empty(&d, sizeof(char)), 0);
        EXPECT_EQ_INT(deque_pushb(&d, &a), 0);
        EXPECT_EQ_INT(deque_pushb(&d, &b), 0);
        EXPECT_EQ_INT(deque_pushb(&d, &c), 0);
        EXPECT_EQ_INT(deque_del(&d, 2, &out), 0);
        EXPECT_EQ_CHAR(out, 'c');
        EXPECT_EQ_INT(deque_del(&d, 0, &out), 0);
        EXPECT_EQ_CHAR(out, 'a');
        EXPECT_EQ_CHAR(*(char *)deque_front(&d), 'b');
        EXPECT_EQ_UINT(deque_len(&d), 1);
        deque_free(&d);
    }
}

UTEST_CASE(deque_dup)
{
    struct deque d, c;
    int a = 1, b = 2;

    EXPECT_EQ_INT(deque_init_empty(&d, sizeof(int)), 0);
    EXPECT_EQ_INT(deque_dup(&d, &d), -EINVAL);
    EXPECT_EQ_INT(deque_pushb(&d, &a), 0);
    EXPECT_EQ_INT(deque_pushb(&d, &b), 0);
    EXPECT_EQ_INT(deque_dup(&d, &c), 0);
    EXPECT_EQ_UINT(deque_len(&c), 2);
    EXPECT_EQ_UINT(deque_elesz(&c), sizeof(int));
    EXPECT_EQ_INT(*(int *)deque_at(&c, 0), 1);
    EXPECT_EQ_INT(*(int *)deque_at(&c, 1), 2);
    EXPECT_NE_PTR(c.buf, d.buf);
    a = 9;
    EXPECT_EQ_INT(deque_pushb(&d, &a), 0);
    EXPECT_EQ_UINT(deque_len(&c), 2);
    deque_free(&c);
    deque_free(&d);

    EXPECT_EQ_INT(deque_init_empty(&d, sizeof(double)), 0);
    EXPECT_EQ_INT(deque_dup(&d, &c), 0);
    EXPECT_TRUE(deque_empty(&c));
    EXPECT_EQ_UINT(deque_elesz(&c), sizeof(double));
    {
        double x = 4.0;

        EXPECT_EQ_INT(deque_pushb(&d, &x), 0);
    }
    EXPECT_TRUE(deque_empty(&c));
    deque_free(&c);
    deque_free(&d);

    {
        struct rec r = {5, 'k', 3.25}, changed = {0, 'x', 0.0};

        EXPECT_EQ_INT(deque_init_empty(&d, sizeof(r)), 0);
        EXPECT_EQ_INT(deque_pushb(&d, &r), 0);
        EXPECT_EQ_INT(deque_dup(&d, &c), 0);
        ((struct rec *)deque_at(&d, 0))->id = 99;
        EXPECT_EQ_INT(((struct rec *)deque_at(&c, 0))->id, 5);
        EXPECT_EQ_CHAR(((struct rec *)deque_at(&c, 0))->tag, 'k');
        EXPECT_EQ_DOUBLE(((struct rec *)deque_at(&c, 0))->score, 3.25);
        EXPECT_EQ_INT(deque_pushb(&c, &changed), 0);
        EXPECT_EQ_UINT(deque_len(&d), 1);
        deque_free(&c);
        deque_free(&d);
    }
}

UTEST_CASE(deque_sort)
{
    struct deque d;
    int a = 3, b = 1, c = 2;

    EXPECT_EQ_INT(deque_init_empty(&d, sizeof(int)), 0);
    deque_sort(&d, cmp_int);
    EXPECT_TRUE(deque_empty(&d));
    EXPECT_EQ_INT(deque_pushb(&d, &a), 0);
    deque_sort(&d, cmp_int);
    EXPECT_EQ_INT(*(int *)deque_at(&d, 0), 3);
    EXPECT_EQ_INT(deque_pushb(&d, &b), 0);
    EXPECT_EQ_INT(deque_pushb(&d, &c), 0);
    deque_sort(&d, cmp_int);
    EXPECT_EQ_INT(*(int *)deque_at(&d, 0), 1);
    EXPECT_EQ_INT(*(int *)deque_at(&d, 1), 2);
    EXPECT_EQ_INT(*(int *)deque_at(&d, 2), 3);
    a = 2;
    b = 2;
    c = 2;
    EXPECT_EQ_INT(deque_pushb(&d, &a), 0);
    EXPECT_EQ_INT(deque_pushb(&d, &b), 0);
    EXPECT_EQ_INT(deque_pushb(&d, &c), 0);
    deque_sort(&d, cmp_int);
    EXPECT_EQ_INT(*(int *)deque_at(&d, 0), 1);
    EXPECT_EQ_INT(*(int *)deque_at(&d, 5), 3);
    deque_free(&d);

    {
        char x = 'c', y = 'a', z = 'b';
        struct rec ra = {3, 'c', 1.0}, rb = {1, 'a', 2.0}, rc = {1, 'b', 3.0};
        int rev[4] = {4, 3, 2, 1};
        int i;

        EXPECT_EQ_INT(deque_init_empty(&d, sizeof(char)), 0);
        EXPECT_EQ_INT(deque_pushb(&d, &x), 0);
        EXPECT_EQ_INT(deque_pushb(&d, &y), 0);
        EXPECT_EQ_INT(deque_pushb(&d, &z), 0);
        deque_sort(&d, cmp_char);
        EXPECT_EQ_CHAR(*(char *)deque_at(&d, 0), 'a');
        EXPECT_EQ_CHAR(*(char *)deque_at(&d, 1), 'b');
        EXPECT_EQ_CHAR(*(char *)deque_at(&d, 2), 'c');
        deque_free(&d);

        EXPECT_EQ_INT(deque_init_empty(&d, sizeof(int)), 0);
        for (i = 0; i < 4; i++)
            EXPECT_EQ_INT(deque_pushb(&d, &rev[i]), 0);
        deque_sort(&d, cmp_int);
        for (i = 0; i < 4; i++)
            EXPECT_EQ_INT(*(int *)deque_at(&d, (size_t)i), i + 1);
        deque_free(&d);

        EXPECT_EQ_INT(deque_init_empty(&d, sizeof(ra)), 0);
        EXPECT_EQ_INT(deque_pushb(&d, &ra), 0);
        EXPECT_EQ_INT(deque_pushb(&d, &rb), 0);
        EXPECT_EQ_INT(deque_pushb(&d, &rc), 0);
        deque_sort(&d, cmp_rec);
        EXPECT_EQ_INT(((struct rec *)deque_at(&d, 0))->id, 1);
        EXPECT_EQ_INT(((struct rec *)deque_at(&d, 2))->id, 3);
        EXPECT_EQ_CHAR(((struct rec *)deque_at(&d, 2))->tag, 'c');
        deque_free(&d);
    }
}

UTEST_CASE(deque_wrap)
{
    struct deque d;
    int a = 1, b = 2, c = 3, e = 4, f = 5, out = 0;

    EXPECT_EQ_INT(deque_init(&d, sizeof(int), 3), 0);
    EXPECT_EQ_INT(deque_pushb(&d, &a), 0);
    EXPECT_EQ_INT(deque_pushb(&d, &b), 0);
    EXPECT_EQ_INT(deque_pushb(&d, &c), 0);
    EXPECT_EQ_INT(deque_popf(&d, &out), 0);
    EXPECT_EQ_INT(out, 1);
    EXPECT_EQ_INT(deque_popf(&d, &out), 0);
    EXPECT_EQ_INT(out, 2);
    EXPECT_EQ_INT(deque_pushb(&d, &e), 0);
    EXPECT_EQ_INT(deque_pushb(&d, &f), 0);
    EXPECT_TRUE(deque_full(&d));
    EXPECT_NE_UINT(deque_head(&d), 0);
    EXPECT_EQ_INT(*(int *)deque_at(&d, 0), 3);
    EXPECT_EQ_INT(*(int *)deque_at(&d, 1), 4);
    EXPECT_EQ_INT(*(int *)deque_at(&d, 2), 5);
    EXPECT_EQ_INT(*(int *)deque_front(&d), 3);
    EXPECT_EQ_INT(*(int *)deque_back(&d), 5);
    EXPECT_EQ_INT(deque_popb(&d, &out), 0);
    EXPECT_EQ_INT(out, 5);
    EXPECT_EQ_INT(deque_popf(&d, &out), 0);
    EXPECT_EQ_INT(out, 3);
    EXPECT_EQ_INT(*(int *)deque_front(&d), 4);
    deque_free(&d);
}

UTEST_CASE(deque_reserve_wrap)
{
    struct deque d;
    int a = 1, b = 2, c = 3, e = 4, out = 0;

    EXPECT_EQ_INT(deque_init(&d, sizeof(int), 3), 0);
    EXPECT_EQ_INT(deque_pushb(&d, &a), 0);
    EXPECT_EQ_INT(deque_pushb(&d, &b), 0);
    EXPECT_EQ_INT(deque_pushb(&d, &c), 0);
    EXPECT_EQ_INT(deque_popf(&d, &out), 0);
    EXPECT_EQ_INT(deque_pushb(&d, &e), 0);
    EXPECT_EQ_INT(deque_reserve(&d, 8), 0);
    EXPECT_GE_UINT(deque_cap(&d), 8);
    EXPECT_EQ_UINT(deque_head(&d), 0);
    EXPECT_EQ_INT(*(int *)deque_at(&d, 0), 2);
    EXPECT_EQ_INT(*(int *)deque_at(&d, 1), 3);
    EXPECT_EQ_INT(*(int *)deque_at(&d, 2), 4);
    deque_free(&d);
}

UTEST_CASE(deque_shrink_wrap)
{
    struct deque d;
    int a = 1, b = 2, c = 3, e = 4, out = 0;

    EXPECT_EQ_INT(deque_init(&d, sizeof(int), 4), 0);
    EXPECT_EQ_INT(deque_pushb(&d, &a), 0);
    EXPECT_EQ_INT(deque_pushb(&d, &b), 0);
    EXPECT_EQ_INT(deque_pushb(&d, &c), 0);
    EXPECT_EQ_INT(deque_popf(&d, &out), 0);
    EXPECT_EQ_INT(deque_pushb(&d, &e), 0);
    deque_shrink(&d);
    EXPECT_EQ_UINT(deque_cap(&d), 3);
    EXPECT_EQ_UINT(deque_head(&d), 0);
    EXPECT_EQ_INT(*(int *)deque_at(&d, 0), 2);
    EXPECT_EQ_INT(*(int *)deque_at(&d, 1), 3);
    EXPECT_EQ_INT(*(int *)deque_at(&d, 2), 4);
    deque_free(&d);
}

UTEST_CASE(deque_ins_wrap)
{
    struct deque d;
    int a = 1, b = 2, c = 3, e = 4, mid = 9, out = 0;

    EXPECT_EQ_INT(deque_init(&d, sizeof(int), 3), 0);
    EXPECT_EQ_INT(deque_pushb(&d, &a), 0);
    EXPECT_EQ_INT(deque_pushb(&d, &b), 0);
    EXPECT_EQ_INT(deque_pushb(&d, &c), 0);
    EXPECT_EQ_INT(deque_popf(&d, &out), 0);
    EXPECT_EQ_INT(deque_pushb(&d, &e), 0);
    EXPECT_EQ_INT(deque_ins(&d, 1, &mid), 0);
    EXPECT_EQ_UINT(deque_len(&d), 4);
    EXPECT_EQ_INT(*(int *)deque_at(&d, 0), 2);
    EXPECT_EQ_INT(*(int *)deque_at(&d, 1), 9);
    EXPECT_EQ_INT(*(int *)deque_at(&d, 2), 3);
    EXPECT_EQ_INT(*(int *)deque_at(&d, 3), 4);
    deque_free(&d);
}

UTEST_CASE(deque_del_wrap)
{
    struct deque d;
    int a = 1, b = 2, c = 3, e = 4, out = 0;

    EXPECT_EQ_INT(deque_init(&d, sizeof(int), 3), 0);
    EXPECT_EQ_INT(deque_pushb(&d, &a), 0);
    EXPECT_EQ_INT(deque_pushb(&d, &b), 0);
    EXPECT_EQ_INT(deque_pushb(&d, &c), 0);
    EXPECT_EQ_INT(deque_popf(&d, &out), 0);
    EXPECT_EQ_INT(deque_pushb(&d, &e), 0);
    EXPECT_EQ_INT(deque_del(&d, 1, &out), 0);
    EXPECT_EQ_INT(out, 3);
    EXPECT_EQ_UINT(deque_len(&d), 2);
    EXPECT_EQ_INT(*(int *)deque_at(&d, 0), 2);
    EXPECT_EQ_INT(*(int *)deque_at(&d, 1), 4);
    deque_free(&d);
}

UTEST_CASE(deque_dup_wrap)
{
    struct deque d, c;
    int a = 1, b = 2, e = 3, out = 0;

    EXPECT_EQ_INT(deque_init(&d, sizeof(int), 3), 0);
    EXPECT_EQ_INT(deque_pushb(&d, &a), 0);
    EXPECT_EQ_INT(deque_pushb(&d, &b), 0);
    EXPECT_EQ_INT(deque_popf(&d, &out), 0);
    EXPECT_EQ_INT(deque_pushb(&d, &e), 0);
    EXPECT_EQ_INT(deque_dup(&d, &c), 0);
    EXPECT_EQ_UINT(deque_len(&c), 2);
    EXPECT_EQ_UINT(deque_head(&c), 0);
    EXPECT_EQ_INT(*(int *)deque_at(&c, 0), 2);
    EXPECT_EQ_INT(*(int *)deque_at(&c, 1), 3);
    deque_free(&c);
    deque_free(&d);
}

UTEST_CASE(deque_sort_wrap)
{
    struct deque d;
    int a = 3, b = 1, c = 4, e = 2, out = 0;

    EXPECT_EQ_INT(deque_init(&d, sizeof(int), 3), 0);
    EXPECT_EQ_INT(deque_pushb(&d, &a), 0);
    EXPECT_EQ_INT(deque_pushb(&d, &b), 0);
    EXPECT_EQ_INT(deque_pushb(&d, &c), 0);
    EXPECT_EQ_INT(deque_popf(&d, &out), 0);
    EXPECT_EQ_INT(deque_pushb(&d, &e), 0);
    deque_sort(&d, cmp_int);
    EXPECT_EQ_INT(*(int *)deque_at(&d, 0), 1);
    EXPECT_EQ_INT(*(int *)deque_at(&d, 1), 2);
    EXPECT_EQ_INT(*(int *)deque_at(&d, 2), 4);
    deque_free(&d);
}

UTEST_CASE(deque_grow_full)
{
    struct deque d;
    int a = 1, b = 2, c = 3;

    EXPECT_EQ_INT(deque_init(&d, sizeof(int), 2), 0);
    EXPECT_EQ_INT(deque_pushb(&d, &a), 0);
    EXPECT_EQ_INT(deque_pushb(&d, &b), 0);
    EXPECT_TRUE(deque_full(&d));
    EXPECT_EQ_INT(deque_pushb(&d, &c), 0);
    EXPECT_EQ_UINT(deque_len(&d), 3);
    EXPECT_GE_UINT(deque_cap(&d), 3);
    EXPECT_EQ_INT(*(int *)deque_at(&d, 0), 1);
    EXPECT_EQ_INT(*(int *)deque_at(&d, 2), 3);
    deque_free(&d);
}

UTEST_SUITE(deque)
{
    UTEST_RUNCASE(deque_len);
    UTEST_RUNCASE(deque_cap);
    UTEST_RUNCASE(deque_elesz);
    UTEST_RUNCASE(deque_empty);
    UTEST_RUNCASE(deque_full);
    UTEST_RUNCASE(deque_avail);
    UTEST_RUNCASE(deque_head);
    UTEST_RUNCASE(deque_init);
    UTEST_RUNCASE(deque_init_empty);
    UTEST_RUNCASE(deque_free);
    UTEST_RUNCASE(deque_clear);
    UTEST_RUNCASE(deque_at);
    UTEST_RUNCASE(deque_back);
    UTEST_RUNCASE(deque_front);
    UTEST_RUNCASE(deque_reserve);
    UTEST_RUNCASE(deque_shrink);
    UTEST_RUNCASE(deque_pushb);
    UTEST_RUNCASE(deque_pushf);
    UTEST_RUNCASE(deque_popb);
    UTEST_RUNCASE(deque_popf);
    UTEST_RUNCASE(deque_ins);
    UTEST_RUNCASE(deque_del);
    UTEST_RUNCASE(deque_dup);
    UTEST_RUNCASE(deque_sort);
    UTEST_RUNCASE(deque_wrap);
    UTEST_RUNCASE(deque_reserve_wrap);
    UTEST_RUNCASE(deque_shrink_wrap);
    UTEST_RUNCASE(deque_ins_wrap);
    UTEST_RUNCASE(deque_del_wrap);
    UTEST_RUNCASE(deque_dup_wrap);
    UTEST_RUNCASE(deque_sort_wrap);
    UTEST_RUNCASE(deque_grow_full);
}
