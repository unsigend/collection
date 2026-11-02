/* Tests for deque. */

#include <deque.h>
#include <errno.h>
#include <utest.h>

static int cmp_int(const void *a, const void *b)
{
    int x = *(const int *)a;
    int y = *(const int *)b;

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
    deque_free(&d);
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
    deque_free(&d);
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
    deque_free(&d);
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
    deque_free(&d);
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
    deque_free(&d);
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
    deque_free(&d);
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
    deque_free(&d);
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
    deque_free(&d);
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
