/* ringbuf test suite */

#include <errno.h>
#include <ringbuf.h>
#include <string.h>
#include <utest.h>

UTEST_CASE(ringbuf_init)
{
    struct ringbuf rb;
    char buf[4];

    EXPECT_EQ_INT(ringbuf_init(&rb, buf, 4, 1), 0);
    EXPECT_EQ_PTR(ringbuf_buf(&rb), buf);
    EXPECT_EQ_UINT(ringbuf_cap(&rb), 4);
    EXPECT_EQ_UINT(ringbuf_len(&rb), 0);
    EXPECT_EQ_UINT(ringbuf_elesz(&rb), 1);
    EXPECT_TRUE(ringbuf_empty(&rb));
    EXPECT_FALSE(ringbuf_full(&rb));

    EXPECT_EQ_INT(ringbuf_init(&rb, buf, 0, 1), -EINVAL);
    EXPECT_EQ_INT(ringbuf_init(&rb, buf, 4, 0), -EINVAL);
}

UTEST_CASE(ringbuf_clear)
{
    struct ringbuf rb;
    int buf[3], v, out;

    ringbuf_init(&rb, buf, 3, sizeof(int));
    v = 1;
    ringbuf_push(&rb, &v);
    v = 2;
    ringbuf_push(&rb, &v);
    ringbuf_clear(&rb);
    EXPECT_EQ_UINT(ringbuf_len(&rb), 0);
    EXPECT_TRUE(ringbuf_empty(&rb));
    EXPECT_EQ_UINT(ringbuf_avail(&rb), 3);
    EXPECT_EQ_PTR(ringbuf_buf(&rb), buf);
    EXPECT_EQ_UINT(ringbuf_cap(&rb), 3);
    EXPECT_EQ_UINT(ringbuf_elesz(&rb), sizeof(int));

    v = 9;
    EXPECT_EQ_INT(ringbuf_push(&rb, &v), 0);
    EXPECT_EQ_INT(ringbuf_pop(&rb, &out), 0);
    EXPECT_EQ_INT(out, 9);
}

UTEST_CASE(ringbuf_len)
{
    struct ringbuf rb;
    char buf[3], c;

    ringbuf_init(&rb, buf, 3, 1);
    EXPECT_EQ_UINT(ringbuf_len(&rb), 0);
    c = 'a';
    ringbuf_push(&rb, &c);
    EXPECT_EQ_UINT(ringbuf_len(&rb), 1);
    c = 'b';
    ringbuf_push(&rb, &c);
    EXPECT_EQ_UINT(ringbuf_len(&rb), 2);
    ringbuf_pop(&rb, &c);
    EXPECT_EQ_UINT(ringbuf_len(&rb), 1);
}

UTEST_CASE(ringbuf_cap)
{
    struct ringbuf rb;
    char buf[8];

    ringbuf_init(&rb, buf, 8, 1);
    EXPECT_EQ_UINT(ringbuf_cap(&rb), 8);
    ringbuf_push(&rb, "x");
    EXPECT_EQ_UINT(ringbuf_cap(&rb), 8);
}

UTEST_CASE(ringbuf_buf)
{
    struct ringbuf rb;
    unsigned char buf[2];

    ringbuf_init(&rb, buf, 2, 1);
    EXPECT_EQ_PTR(ringbuf_buf(&rb), buf);
}

UTEST_CASE(ringbuf_elesz)
{
    struct ringbuf rb;
    int buf[4];

    ringbuf_init(&rb, buf, 4, sizeof(int));
    EXPECT_EQ_UINT(ringbuf_elesz(&rb), sizeof(int));
}

UTEST_CASE(ringbuf_full)
{
    struct ringbuf rb;
    char buf[2], c;

    ringbuf_init(&rb, buf, 2, 1);
    EXPECT_FALSE(ringbuf_full(&rb));
    c = 'a';
    ringbuf_push(&rb, &c);
    EXPECT_FALSE(ringbuf_full(&rb));
    c = 'b';
    ringbuf_push(&rb, &c);
    EXPECT_TRUE(ringbuf_full(&rb));
}

UTEST_CASE(ringbuf_empty)
{
    struct ringbuf rb;
    char buf[2], c;

    ringbuf_init(&rb, buf, 2, 1);
    EXPECT_TRUE(ringbuf_empty(&rb));
    c = 'a';
    ringbuf_push(&rb, &c);
    EXPECT_FALSE(ringbuf_empty(&rb));
    ringbuf_pop(&rb, &c);
    EXPECT_TRUE(ringbuf_empty(&rb));
}

UTEST_CASE(ringbuf_avail)
{
    struct ringbuf rb;
    char buf[4], c;

    ringbuf_init(&rb, buf, 4, 1);
    EXPECT_EQ_UINT(ringbuf_avail(&rb), 4);
    c = 'a';
    ringbuf_push(&rb, &c);
    EXPECT_EQ_UINT(ringbuf_avail(&rb), 3);
    c = 'b';
    ringbuf_push(&rb, &c);
    c = 'c';
    ringbuf_push(&rb, &c);
    c = 'd';
    ringbuf_push(&rb, &c);
    EXPECT_EQ_UINT(ringbuf_avail(&rb), 0);
    ringbuf_pop(&rb, &c);
    EXPECT_EQ_UINT(ringbuf_avail(&rb), 1);
}

UTEST_CASE(ringbuf_push)
{
    struct ringbuf rb;
    int buf[3], v, out;

    ringbuf_init(&rb, buf, 3, sizeof(int));
    v = 10;
    EXPECT_EQ_INT(ringbuf_push(&rb, &v), 0);
    v = 20;
    EXPECT_EQ_INT(ringbuf_push(&rb, &v), 0);
    v = 30;
    EXPECT_EQ_INT(ringbuf_push(&rb, &v), 0);
    EXPECT_TRUE(ringbuf_full(&rb));
    v = 40;
    EXPECT_EQ_INT(ringbuf_push(&rb, &v), -ENOSPC);

    ringbuf_pop(&rb, &out);
    EXPECT_EQ_INT(out, 10);
    v = 40;
    EXPECT_EQ_INT(ringbuf_push(&rb, &v), 0);
    ringbuf_pop(&rb, &out);
    EXPECT_EQ_INT(out, 20);
    ringbuf_pop(&rb, &out);
    EXPECT_EQ_INT(out, 30);
    ringbuf_pop(&rb, &out);
    EXPECT_EQ_INT(out, 40);
}

UTEST_CASE(ringbuf_pop)
{
    struct ringbuf rb;
    char buf[3], c, out;

    ringbuf_init(&rb, buf, 3, 1);
    EXPECT_EQ_INT(ringbuf_pop(&rb, &out), -EAGAIN);

    c = 'x';
    ringbuf_push(&rb, &c);
    c = 'y';
    ringbuf_push(&rb, &c);
    EXPECT_EQ_INT(ringbuf_pop(&rb, &out), 0);
    EXPECT_EQ_CHAR(out, 'x');
    EXPECT_EQ_INT(ringbuf_pop(&rb, &out), 0);
    EXPECT_EQ_CHAR(out, 'y');
    EXPECT_TRUE(ringbuf_empty(&rb));
}

UTEST_CASE(ringbuf_peek)
{
    struct ringbuf rb;
    int buf[2], v, out;

    ringbuf_init(&rb, buf, 2, sizeof(int));
    EXPECT_EQ_INT(ringbuf_peek(&rb, &out), -EAGAIN);

    v = 7;
    ringbuf_push(&rb, &v);
    v = 8;
    ringbuf_push(&rb, &v);
    EXPECT_EQ_INT(ringbuf_peek(&rb, &out), 0);
    EXPECT_EQ_INT(out, 7);
    EXPECT_EQ_UINT(ringbuf_len(&rb), 2);
    EXPECT_EQ_INT(ringbuf_peek(&rb, &out), 0);
    EXPECT_EQ_INT(out, 7);
    ringbuf_pop(&rb, &out);
    EXPECT_EQ_INT(out, 7);
    EXPECT_EQ_INT(ringbuf_peek(&rb, &out), 0);
    EXPECT_EQ_INT(out, 8);
}

UTEST_CASE(ringbuf_drop)
{
    struct ringbuf rb;
    char buf[4], c, out;

    ringbuf_init(&rb, buf, 4, 1);
    c = 'a';
    ringbuf_push(&rb, &c);
    c = 'b';
    ringbuf_push(&rb, &c);
    c = 'c';
    ringbuf_push(&rb, &c);
    ringbuf_drop(&rb, 0);
    EXPECT_EQ_UINT(ringbuf_len(&rb), 3);
    ringbuf_drop(&rb, 2);
    EXPECT_EQ_UINT(ringbuf_len(&rb), 1);
    EXPECT_EQ_INT(ringbuf_peek(&rb, &out), 0);
    EXPECT_EQ_CHAR(out, 'c');

    ringbuf_drop(&rb, 100);
    EXPECT_TRUE(ringbuf_empty(&rb));
    EXPECT_EQ_UINT(ringbuf_avail(&rb), 4);
    ringbuf_drop(&rb, 1);
    EXPECT_TRUE(ringbuf_empty(&rb));

    c = 'd';
    ringbuf_push(&rb, &c);
    EXPECT_EQ_INT(ringbuf_pop(&rb, &out), 0);
    EXPECT_EQ_CHAR(out, 'd');
}

UTEST_SUITE(ringbuf)
{
    UTEST_RUNCASE(ringbuf_init);
    UTEST_RUNCASE(ringbuf_clear);
    UTEST_RUNCASE(ringbuf_len);
    UTEST_RUNCASE(ringbuf_cap);
    UTEST_RUNCASE(ringbuf_buf);
    UTEST_RUNCASE(ringbuf_elesz);
    UTEST_RUNCASE(ringbuf_full);
    UTEST_RUNCASE(ringbuf_empty);
    UTEST_RUNCASE(ringbuf_avail);
    UTEST_RUNCASE(ringbuf_push);
    UTEST_RUNCASE(ringbuf_pop);
    UTEST_RUNCASE(ringbuf_peek);
    UTEST_RUNCASE(ringbuf_drop);
}
