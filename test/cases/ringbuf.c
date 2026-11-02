/* Tests for ringbuf. */

#include <errno.h>
#include <ringbuf.h>
#include <string.h>
#include <utest.h>

struct rec {
    int id;
    char tag;
    double score;
};

UTEST_CASE(ringbuf_len)
{
    struct ringbuf rb;
    int buf[4];
    int x = 1;

    EXPECT_EQ_INT(ringbuf_init(&rb, buf, 4, sizeof(int)), 0);
    EXPECT_EQ_UINT(ringbuf_len(&rb), 0);
    EXPECT_EQ_INT(ringbuf_push(&rb, &x), 0);
    EXPECT_EQ_UINT(ringbuf_len(&rb), 1);
}

UTEST_CASE(ringbuf_cap)
{
    struct ringbuf rb;
    int buf[4];

    EXPECT_EQ_INT(ringbuf_init(&rb, buf, 4, sizeof(int)), 0);
    EXPECT_EQ_UINT(ringbuf_cap(&rb), 4);
}

UTEST_CASE(ringbuf_elesz)
{
    struct ringbuf rb;
    int buf[2];

    EXPECT_EQ_INT(ringbuf_init(&rb, buf, 2, sizeof(int)), 0);
    EXPECT_EQ_UINT(ringbuf_elesz(&rb), sizeof(int));

    {
        double dbuf[2];
        struct rec rbuf[2];

        EXPECT_EQ_INT(ringbuf_init(&rb, dbuf, 2, sizeof(double)), 0);
        EXPECT_EQ_UINT(ringbuf_elesz(&rb), sizeof(double));
        EXPECT_EQ_INT(ringbuf_init(&rb, rbuf, 2, sizeof(struct rec)), 0);
        EXPECT_EQ_UINT(ringbuf_elesz(&rb), sizeof(struct rec));
        EXPECT_EQ_UINT(ringbuf_cap(&rb), 2);
    }
}

UTEST_CASE(ringbuf_empty)
{
    struct ringbuf rb;
    int buf[2];
    int x = 1;

    EXPECT_EQ_INT(ringbuf_init(&rb, buf, 2, sizeof(int)), 0);
    EXPECT_TRUE(ringbuf_empty(&rb));
    EXPECT_EQ_INT(ringbuf_push(&rb, &x), 0);
    EXPECT_FALSE(ringbuf_empty(&rb));
}

UTEST_CASE(ringbuf_full)
{
    struct ringbuf rb;
    int buf[2];
    int a = 1, b = 2;

    EXPECT_EQ_INT(ringbuf_init(&rb, buf, 2, sizeof(int)), 0);
    EXPECT_FALSE(ringbuf_full(&rb));
    EXPECT_EQ_INT(ringbuf_push(&rb, &a), 0);
    EXPECT_FALSE(ringbuf_full(&rb));
    EXPECT_EQ_INT(ringbuf_push(&rb, &b), 0);
    EXPECT_TRUE(ringbuf_full(&rb));
    EXPECT_EQ_UINT(ringbuf_avail(&rb), 0);

    {
        char storage[1];
        char c = 'a';

        EXPECT_EQ_INT(ringbuf_init(&rb, storage, 1, sizeof(c)), 0);
        EXPECT_FALSE(ringbuf_full(&rb));
        EXPECT_EQ_INT(ringbuf_push(&rb, &c), 0);
        EXPECT_TRUE(ringbuf_full(&rb));
        EXPECT_EQ_UINT(ringbuf_avail(&rb), 0);
    }
}

UTEST_CASE(ringbuf_avail)
{
    struct ringbuf rb;
    int buf[4];
    int x = 1;

    EXPECT_EQ_INT(ringbuf_init(&rb, buf, 4, sizeof(int)), 0);
    EXPECT_EQ_UINT(ringbuf_avail(&rb), 4);
    EXPECT_EQ_INT(ringbuf_push(&rb, &x), 0);
    EXPECT_EQ_UINT(ringbuf_avail(&rb), ringbuf_cap(&rb) - ringbuf_len(&rb));
    EXPECT_EQ_INT(ringbuf_push(&rb, &x), 0);
    EXPECT_EQ_INT(ringbuf_push(&rb, &x), 0);
    EXPECT_EQ_INT(ringbuf_push(&rb, &x), 0);
    EXPECT_EQ_UINT(ringbuf_avail(&rb), 0);
    EXPECT_TRUE(ringbuf_full(&rb));
    EXPECT_EQ_INT(ringbuf_pop(&rb, &x), 0);
    EXPECT_EQ_UINT(ringbuf_avail(&rb), 1);
}

UTEST_CASE(ringbuf_init)
{
    struct ringbuf rb;
    int buf[3];

    EXPECT_EQ_INT(ringbuf_init(&rb, buf, 3, 0), -EINVAL);
    EXPECT_EQ_INT(ringbuf_init(&rb, buf, 0, sizeof(int)), -EINVAL);
    EXPECT_EQ_INT(ringbuf_init(&rb, buf, 3, sizeof(int)), 0);
    EXPECT_EQ_PTR(rb.buf, buf);
    EXPECT_EQ_UINT(rb.cap, 3);
    EXPECT_EQ_UINT(rb.elesz, sizeof(int));
    EXPECT_EQ_UINT(rb.len, 0);
    EXPECT_EQ_UINT(rb.head, 0);
    EXPECT_EQ_UINT(rb.tail, 0);

    {
        char one[1];
        struct rec recs[2];
        double ds[3];

        EXPECT_EQ_INT(ringbuf_init(&rb, one, 1, sizeof(char)), 0);
        EXPECT_EQ_UINT(ringbuf_cap(&rb), 1);
        EXPECT_EQ_UINT(ringbuf_elesz(&rb), sizeof(char));
        EXPECT_TRUE(ringbuf_empty(&rb));
        EXPECT_FALSE(ringbuf_full(&rb));

        EXPECT_EQ_INT(ringbuf_init(&rb, recs, 2, sizeof(struct rec)), 0);
        EXPECT_EQ_UINT(ringbuf_elesz(&rb), sizeof(struct rec));
        EXPECT_EQ_PTR(rb.buf, recs);

        EXPECT_EQ_INT(ringbuf_init(&rb, ds, 3, sizeof(double)), 0);
        EXPECT_EQ_UINT(ringbuf_cap(&rb), 3);
        EXPECT_EQ_UINT(ringbuf_elesz(&rb), sizeof(double));
    }
}

UTEST_CASE(ringbuf_clear)
{
    struct ringbuf rb;
    int buf[4];
    int a = 1, b = 2, out;

    EXPECT_EQ_INT(ringbuf_init(&rb, buf, 4, sizeof(int)), 0);
    EXPECT_EQ_INT(ringbuf_push(&rb, &a), 0);
    EXPECT_EQ_INT(ringbuf_push(&rb, &b), 0);
    ringbuf_clear(&rb);
    EXPECT_TRUE(ringbuf_empty(&rb));
    EXPECT_EQ_UINT(rb.head, 0);
    EXPECT_EQ_UINT(rb.tail, 0);
    EXPECT_EQ_UINT(ringbuf_avail(&rb), 4);
    EXPECT_EQ_INT(ringbuf_pop(&rb, &out), -EAGAIN);
    EXPECT_EQ_INT(ringbuf_push(&rb, &a), 0);
    EXPECT_EQ_INT(ringbuf_pop(&rb, &out), 0);
    EXPECT_EQ_INT(out, 1);

    EXPECT_EQ_INT(ringbuf_init(&rb, buf, 4, sizeof(int)), 0);
    ringbuf_clear(&rb);
    EXPECT_TRUE(ringbuf_empty(&rb));
    EXPECT_EQ_UINT(ringbuf_avail(&rb), 4);
    EXPECT_EQ_INT(ringbuf_push(&rb, &a), 0);
    EXPECT_EQ_INT(ringbuf_push(&rb, &b), 0);
    ringbuf_clear(&rb);
    EXPECT_EQ_UINT(ringbuf_len(&rb), 0);
    EXPECT_EQ_INT(ringbuf_pop(&rb, &out), -EAGAIN);
}

UTEST_CASE(ringbuf_push)
{
    struct ringbuf rb;
    int buf[3];
    int a = 1, b = 2, c = 3, d = 4;

    EXPECT_EQ_INT(ringbuf_init(&rb, buf, 3, sizeof(int)), 0);
    EXPECT_EQ_INT(ringbuf_push(&rb, &a), 0);
    EXPECT_EQ_INT(ringbuf_push(&rb, &b), 0);
    EXPECT_EQ_INT(ringbuf_push(&rb, &c), 0);
    EXPECT_TRUE(ringbuf_full(&rb));
    EXPECT_EQ_INT(ringbuf_push(&rb, &d), -ENOSPC);
    EXPECT_EQ_UINT(ringbuf_len(&rb), 3);

    {
        struct rec storage[2];
        struct rec a = {1, 'a', 1.5}, b = {2, 'b', -2.5}, c = {3, 'c', 0.0};
        double ds[1];
        double x = 3.25, y = 4.5;

        EXPECT_EQ_INT(ringbuf_init(&rb, storage, 2, sizeof(a)), 0);
        EXPECT_EQ_INT(ringbuf_push(&rb, &a), 0);
        EXPECT_EQ_INT(ringbuf_push(&rb, &b), 0);
        EXPECT_EQ_INT(ringbuf_push(&rb, &c), -ENOSPC);
        EXPECT_EQ_UINT(ringbuf_len(&rb), 2);

        EXPECT_EQ_INT(ringbuf_init(&rb, ds, 1, sizeof(double)), 0);
        EXPECT_EQ_INT(ringbuf_push(&rb, &x), 0);
        EXPECT_TRUE(ringbuf_full(&rb));
        EXPECT_EQ_INT(ringbuf_push(&rb, &y), -ENOSPC);
        EXPECT_EQ_UINT(ringbuf_len(&rb), 1);
    }
}

UTEST_CASE(ringbuf_pop)
{
    struct ringbuf rb;
    int buf[3];
    int a = 1, b = 2, c = 3, out = 0;

    EXPECT_EQ_INT(ringbuf_init(&rb, buf, 3, sizeof(int)), 0);
    EXPECT_EQ_INT(ringbuf_pop(&rb, &out), -EAGAIN);
    EXPECT_EQ_INT(ringbuf_push(&rb, &a), 0);
    EXPECT_EQ_INT(ringbuf_push(&rb, &b), 0);
    EXPECT_EQ_INT(ringbuf_push(&rb, &c), 0);
    EXPECT_EQ_INT(ringbuf_pop(&rb, &out), 0);
    EXPECT_EQ_INT(out, 1);
    EXPECT_EQ_INT(ringbuf_pop(&rb, &out), 0);
    EXPECT_EQ_INT(out, 2);
    EXPECT_EQ_INT(ringbuf_pop(&rb, &out), 0);
    EXPECT_EQ_INT(out, 3);
    EXPECT_TRUE(ringbuf_empty(&rb));
    EXPECT_EQ_INT(ringbuf_pop(&rb, &out), -EAGAIN);

    {
        struct rec storage[2];
        struct rec a = {1, 'a', 1.25}, b = {2, 'b', 8.5}, got;
        char one[1];
        char c = 'q', d = 'r', ch;

        EXPECT_EQ_INT(ringbuf_init(&rb, storage, 2, sizeof(a)), 0);
        EXPECT_EQ_INT(ringbuf_push(&rb, &a), 0);
        EXPECT_EQ_INT(ringbuf_push(&rb, &b), 0);
        EXPECT_EQ_INT(ringbuf_pop(&rb, &got), 0);
        EXPECT_EQ_INT(got.id, 1);
        EXPECT_EQ_CHAR(got.tag, 'a');
        EXPECT_EQ_DOUBLE(got.score, 1.25);
        EXPECT_EQ_INT(ringbuf_push(&rb, &a), 0);
        EXPECT_EQ_INT(ringbuf_pop(&rb, &got), 0);
        EXPECT_EQ_INT(got.id, 2);
        EXPECT_EQ_INT(ringbuf_pop(&rb, &got), 0);
        EXPECT_EQ_CHAR(got.tag, 'a');
        EXPECT_TRUE(ringbuf_empty(&rb));

        EXPECT_EQ_INT(ringbuf_init(&rb, one, 1, sizeof(char)), 0);
        EXPECT_EQ_INT(ringbuf_pop(&rb, &ch), -EAGAIN);
        EXPECT_EQ_INT(ringbuf_push(&rb, &c), 0);
        EXPECT_EQ_INT(ringbuf_pop(&rb, &ch), 0);
        EXPECT_EQ_CHAR(ch, 'q');
        EXPECT_EQ_INT(ringbuf_push(&rb, &d), 0);
        EXPECT_EQ_INT(ringbuf_pop(&rb, &ch), 0);
        EXPECT_EQ_CHAR(ch, 'r');
        EXPECT_EQ_INT(ringbuf_pop(&rb, &ch), -EAGAIN);
    }
}

UTEST_CASE(ringbuf_peek)
{
    struct ringbuf rb;
    int buf[2];
    int a = 1, b = 2, out = 0;

    EXPECT_EQ_INT(ringbuf_init(&rb, buf, 2, sizeof(int)), 0);
    EXPECT_EQ_INT(ringbuf_peek(&rb, &out), -EAGAIN);
    EXPECT_EQ_INT(ringbuf_push(&rb, &a), 0);
    EXPECT_EQ_INT(ringbuf_push(&rb, &b), 0);
    EXPECT_EQ_INT(ringbuf_peek(&rb, &out), 0);
    EXPECT_EQ_INT(out, 1);
    EXPECT_EQ_UINT(ringbuf_len(&rb), 2);
    EXPECT_EQ_INT(ringbuf_peek(&rb, &out), 0);
    EXPECT_EQ_INT(out, 1);
    EXPECT_EQ_INT(ringbuf_pop(&rb, &out), 0);
    EXPECT_EQ_INT(out, 1);
    EXPECT_EQ_INT(ringbuf_peek(&rb, &out), 0);
    EXPECT_EQ_INT(out, 2);

    {
        struct rec storage[1];
        struct rec a = {9, 'z', -0.5}, got;
        double ds[2];
        double x = 1.5, y = 2.5, g;

        EXPECT_EQ_INT(ringbuf_init(&rb, storage, 1, sizeof(a)), 0);
        EXPECT_EQ_INT(ringbuf_peek(&rb, &got), -EAGAIN);
        EXPECT_EQ_INT(ringbuf_push(&rb, &a), 0);
        EXPECT_EQ_INT(ringbuf_peek(&rb, &got), 0);
        EXPECT_EQ_INT(got.id, 9);
        EXPECT_EQ_CHAR(got.tag, 'z');
        EXPECT_EQ_DOUBLE(got.score, -0.5);
        EXPECT_EQ_UINT(ringbuf_len(&rb), 1);

        EXPECT_EQ_INT(ringbuf_init(&rb, ds, 2, sizeof(double)), 0);
        EXPECT_EQ_INT(ringbuf_push(&rb, &x), 0);
        EXPECT_EQ_INT(ringbuf_push(&rb, &y), 0);
        EXPECT_EQ_INT(ringbuf_pop(&rb, &g), 0);
        EXPECT_EQ_INT(ringbuf_push(&rb, &x), 0);
        EXPECT_EQ_INT(ringbuf_peek(&rb, &g), 0);
        EXPECT_EQ_DOUBLE(g, 2.5);
        EXPECT_EQ_INT(ringbuf_peek(&rb, &g), 0);
        EXPECT_EQ_DOUBLE(g, 2.5);
        EXPECT_EQ_UINT(ringbuf_len(&rb), 2);
    }
}

UTEST_CASE(ringbuf_drop)
{
    struct ringbuf rb;
    int buf[4];
    int a = 1, b = 2, c = 3, d = 4, out = 0;

    EXPECT_EQ_INT(ringbuf_init(&rb, buf, 4, sizeof(int)), 0);
    ringbuf_drop(&rb, 0);
    ringbuf_drop(&rb, 3);
    EXPECT_TRUE(ringbuf_empty(&rb));

    EXPECT_EQ_INT(ringbuf_push(&rb, &a), 0);
    EXPECT_EQ_INT(ringbuf_push(&rb, &b), 0);
    EXPECT_EQ_INT(ringbuf_push(&rb, &c), 0);
    EXPECT_EQ_INT(ringbuf_push(&rb, &d), 0);
    ringbuf_drop(&rb, 0);
    EXPECT_EQ_UINT(ringbuf_len(&rb), 4);
    ringbuf_drop(&rb, 2);
    EXPECT_EQ_UINT(ringbuf_len(&rb), 2);
    EXPECT_EQ_INT(ringbuf_peek(&rb, &out), 0);
    EXPECT_EQ_INT(out, 3);
    ringbuf_drop(&rb, 8);
    EXPECT_TRUE(ringbuf_empty(&rb));
    EXPECT_EQ_INT(ringbuf_pop(&rb, &out), -EAGAIN);

    {
        char storage[3];
        char a = 'a', b = 'b', c = 'c', out = 0;

        EXPECT_EQ_INT(ringbuf_init(&rb, storage, 3, sizeof(char)), 0);
        EXPECT_EQ_INT(ringbuf_push(&rb, &a), 0);
        EXPECT_EQ_INT(ringbuf_push(&rb, &b), 0);
        EXPECT_EQ_INT(ringbuf_push(&rb, &c), 0);
        ringbuf_drop(&rb, 1);
        EXPECT_EQ_UINT(ringbuf_len(&rb), 2);
        EXPECT_EQ_INT(ringbuf_peek(&rb, &out), 0);
        EXPECT_EQ_CHAR(out, 'b');
        ringbuf_drop(&rb, ringbuf_len(&rb));
        EXPECT_TRUE(ringbuf_empty(&rb));
        EXPECT_EQ_UINT(ringbuf_avail(&rb), 3);
        ringbuf_drop(&rb, 1);
        EXPECT_TRUE(ringbuf_empty(&rb));
    }
}

UTEST_CASE(ringbuf_wrap)
{
    struct ringbuf rb;
    int buf[3];
    int a = 1, b = 2, c = 3, d = 4, e = 5, out = 0;

    EXPECT_EQ_INT(ringbuf_init(&rb, buf, 3, sizeof(int)), 0);
    EXPECT_EQ_INT(ringbuf_push(&rb, &a), 0);
    EXPECT_EQ_INT(ringbuf_push(&rb, &b), 0);
    EXPECT_EQ_INT(ringbuf_push(&rb, &c), 0);
    EXPECT_EQ_INT(ringbuf_pop(&rb, &out), 0);
    EXPECT_EQ_INT(out, 1);
    EXPECT_EQ_INT(ringbuf_pop(&rb, &out), 0);
    EXPECT_EQ_INT(out, 2);
    EXPECT_EQ_INT(ringbuf_push(&rb, &d), 0);
    EXPECT_EQ_INT(ringbuf_push(&rb, &e), 0);
    EXPECT_TRUE(ringbuf_full(&rb));
    EXPECT_EQ_INT(ringbuf_push(&rb, &a), -ENOSPC);
    EXPECT_EQ_INT(ringbuf_pop(&rb, &out), 0);
    EXPECT_EQ_INT(out, 3);
    EXPECT_EQ_INT(ringbuf_pop(&rb, &out), 0);
    EXPECT_EQ_INT(out, 4);
    EXPECT_EQ_INT(ringbuf_pop(&rb, &out), 0);
    EXPECT_EQ_INT(out, 5);
    EXPECT_TRUE(ringbuf_empty(&rb));
}

UTEST_CASE(ringbuf_drop_wrap)
{
    struct ringbuf rb;
    int buf[3];
    int a = 1, b = 2, c = 3, d = 4, out = 0;

    EXPECT_EQ_INT(ringbuf_init(&rb, buf, 3, sizeof(int)), 0);
    EXPECT_EQ_INT(ringbuf_push(&rb, &a), 0);
    EXPECT_EQ_INT(ringbuf_push(&rb, &b), 0);
    EXPECT_EQ_INT(ringbuf_push(&rb, &c), 0);
    EXPECT_EQ_INT(ringbuf_pop(&rb, &out), 0);
    EXPECT_EQ_INT(out, 1);
    EXPECT_EQ_INT(ringbuf_push(&rb, &d), 0);
    ringbuf_drop(&rb, 2);
    EXPECT_EQ_UINT(ringbuf_len(&rb), 1);
    EXPECT_EQ_INT(ringbuf_pop(&rb, &out), 0);
    EXPECT_EQ_INT(out, 4);
    EXPECT_TRUE(ringbuf_empty(&rb));
}

UTEST_CASE(ringbuf_byte)
{
    struct ringbuf rb;
    unsigned char buf[2];
    unsigned char a = 0x11, b = 0x22, c = 0x33, out = 0;

    EXPECT_EQ_INT(ringbuf_init(&rb, buf, 2, 1), 0);
    EXPECT_EQ_INT(ringbuf_push(&rb, &a), 0);
    EXPECT_EQ_INT(ringbuf_push(&rb, &b), 0);
    EXPECT_EQ_INT(ringbuf_push(&rb, &c), -ENOSPC);
    EXPECT_EQ_INT(ringbuf_peek(&rb, &out), 0);
    EXPECT_EQ_UINT(out, 0x11);
    EXPECT_EQ_INT(ringbuf_pop(&rb, &out), 0);
    EXPECT_EQ_UINT(out, 0x11);
    EXPECT_EQ_INT(ringbuf_push(&rb, &c), 0);
    EXPECT_EQ_INT(ringbuf_pop(&rb, &out), 0);
    EXPECT_EQ_UINT(out, 0x22);
    EXPECT_EQ_INT(ringbuf_pop(&rb, &out), 0);
    EXPECT_EQ_UINT(out, 0x33);
}

UTEST_SUITE(ringbuf)
{
    UTEST_RUNCASE(ringbuf_len);
    UTEST_RUNCASE(ringbuf_cap);
    UTEST_RUNCASE(ringbuf_elesz);
    UTEST_RUNCASE(ringbuf_empty);
    UTEST_RUNCASE(ringbuf_full);
    UTEST_RUNCASE(ringbuf_avail);
    UTEST_RUNCASE(ringbuf_init);
    UTEST_RUNCASE(ringbuf_clear);
    UTEST_RUNCASE(ringbuf_push);
    UTEST_RUNCASE(ringbuf_pop);
    UTEST_RUNCASE(ringbuf_peek);
    UTEST_RUNCASE(ringbuf_drop);
    UTEST_RUNCASE(ringbuf_wrap);
    UTEST_RUNCASE(ringbuf_drop_wrap);
    UTEST_RUNCASE(ringbuf_byte);
}
