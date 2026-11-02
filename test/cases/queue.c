/* Tests for queue. */

#include <errno.h>
#include <queue.h>
#include <utest.h>

struct rec {
    int id;
    char tag;
    double score;
};

UTEST_CASE(queue_len)
{
    struct queue q;
    int x = 1;

    EXPECT_EQ_INT(queue_init_empty(&q, sizeof(int)), 0);
    EXPECT_EQ_UINT(queue_len(&q), 0);
    EXPECT_EQ_INT(queue_push(&q, &x), 0);
    EXPECT_EQ_UINT(queue_len(&q), 1);
    EXPECT_EQ_INT(queue_pop(&q, &x), 0);
    EXPECT_EQ_UINT(queue_len(&q), 0);
    queue_free(&q);

    EXPECT_EQ_INT(queue_init(&q, sizeof(char), 1), 0);
    EXPECT_EQ_UINT(queue_len(&q), 0);
    queue_free(&q);
}

UTEST_CASE(queue_elesz)
{
    struct queue q;

    EXPECT_EQ_INT(queue_init_empty(&q, sizeof(int)), 0);
    EXPECT_EQ_UINT(queue_elesz(&q), sizeof(int));
    queue_free(&q);

    EXPECT_EQ_INT(queue_init_empty(&q, sizeof(double)), 0);
    EXPECT_EQ_UINT(queue_elesz(&q), sizeof(double));
    queue_free(&q);

    EXPECT_EQ_INT(queue_init_empty(&q, sizeof(struct rec)), 0);
    EXPECT_EQ_UINT(queue_elesz(&q), sizeof(struct rec));
    queue_free(&q);
}

UTEST_CASE(queue_empty)
{
    struct queue q;
    int x = 1;

    EXPECT_EQ_INT(queue_init_empty(&q, sizeof(int)), 0);
    EXPECT_TRUE(queue_empty(&q));
    EXPECT_EQ_INT(queue_push(&q, &x), 0);
    EXPECT_FALSE(queue_empty(&q));
    EXPECT_EQ_INT(queue_pop(&q, &x), 0);
    EXPECT_TRUE(queue_empty(&q));
    queue_free(&q);
}

UTEST_CASE(queue_full)
{
    struct queue q;
    int a = 1, b = 2;

    EXPECT_EQ_INT(queue_init(&q, sizeof(int), 2), 0);
    EXPECT_FALSE(queue_full(&q));
    EXPECT_EQ_INT(queue_push(&q, &a), 0);
    EXPECT_FALSE(queue_full(&q));
    EXPECT_EQ_INT(queue_push(&q, &b), 0);
    EXPECT_TRUE(queue_full(&q));
    EXPECT_EQ_UINT(queue_avail(&q), 0);
    EXPECT_EQ_INT(queue_pop(&q, &a), 0);
    EXPECT_FALSE(queue_full(&q));
    queue_free(&q);

    {
        char c = 'z', d = 'y';

        EXPECT_EQ_INT(queue_init(&q, sizeof(c), 1), 0);
        EXPECT_TRUE(queue_empty(&q));
        EXPECT_FALSE(queue_full(&q));
        EXPECT_EQ_INT(queue_push(&q, &c), 0);
        EXPECT_TRUE(queue_full(&q));
        EXPECT_EQ_UINT(queue_avail(&q), 0);
        EXPECT_EQ_INT(queue_push(&q, &d), 0);
        EXPECT_EQ_UINT(queue_len(&q), 2);
        EXPECT_EQ_CHAR(*(char *)queue_front(&q), 'z');
        EXPECT_EQ_CHAR(*(char *)queue_back(&q), 'y');
        queue_free(&q);
    }
}

UTEST_CASE(queue_avail)
{
    struct queue q;
    int x = 1;

    EXPECT_EQ_INT(queue_init(&q, sizeof(int), 4), 0);
    EXPECT_EQ_UINT(queue_avail(&q), 4);
    EXPECT_EQ_INT(queue_push(&q, &x), 0);
    EXPECT_EQ_UINT(queue_avail(&q), 3);
    EXPECT_EQ_INT(queue_push(&q, &x), 0);
    EXPECT_EQ_INT(queue_push(&q, &x), 0);
    EXPECT_EQ_INT(queue_push(&q, &x), 0);
    EXPECT_EQ_UINT(queue_avail(&q), 0);
    EXPECT_TRUE(queue_full(&q));
    EXPECT_EQ_INT(queue_pop(&q, &x), 0);
    EXPECT_EQ_UINT(queue_avail(&q), 1);
    queue_free(&q);
}

UTEST_CASE(queue_init)
{
    struct queue q;

    EXPECT_EQ_INT(queue_init(&q, 0, 4), -EINVAL);

    EXPECT_EQ_INT(queue_init(&q, sizeof(int), 4), 0);
    EXPECT_EQ_UINT(queue_len(&q), 0);
    EXPECT_EQ_UINT(queue_elesz(&q), sizeof(int));
    EXPECT_TRUE(queue_empty(&q));
    EXPECT_EQ_UINT(queue_avail(&q), 4);
    queue_free(&q);

    EXPECT_EQ_INT(queue_init(&q, sizeof(int), 0), 0);
    EXPECT_TRUE(queue_empty(&q));
    EXPECT_TRUE(queue_full(&q));
    EXPECT_EQ_UINT(queue_avail(&q), 0);
    queue_free(&q);

    EXPECT_EQ_INT(queue_init(&q, sizeof(char), 1), 0);
    EXPECT_EQ_UINT(queue_elesz(&q), sizeof(char));
    EXPECT_EQ_UINT(queue_len(&q), 0);
    EXPECT_EQ_UINT(queue_avail(&q), 1);
    queue_free(&q);

    EXPECT_EQ_INT(queue_init(&q, sizeof(struct rec), 2), 0);
    EXPECT_EQ_UINT(queue_elesz(&q), sizeof(struct rec));
    EXPECT_TRUE(queue_empty(&q));
    EXPECT_FALSE(queue_full(&q));
    queue_free(&q);
}

UTEST_CASE(queue_init_empty)
{
    struct queue q;

    EXPECT_EQ_INT(queue_init_empty(&q, 0), -EINVAL);

    EXPECT_EQ_INT(queue_init_empty(&q, sizeof(int)), 0);
    EXPECT_EQ_UINT(queue_len(&q), 0);
    EXPECT_TRUE(queue_empty(&q));
    EXPECT_TRUE(queue_full(&q));
    queue_free(&q);

    EXPECT_EQ_INT(queue_init_empty(&q, sizeof(double)), 0);
    EXPECT_EQ_UINT(queue_elesz(&q), sizeof(double));
    EXPECT_EQ_UINT(queue_len(&q), 0);
    queue_free(&q);

    EXPECT_EQ_INT(queue_init_empty(&q, sizeof(struct rec)), 0);
    EXPECT_EQ_UINT(queue_elesz(&q), sizeof(struct rec));
    queue_free(&q);
}

UTEST_CASE(queue_free)
{
    struct queue q;
    int x = 1;

    queue_free(NULL);

    EXPECT_EQ_INT(queue_init_empty(&q, sizeof(int)), 0);
    EXPECT_EQ_INT(queue_push(&q, &x), 0);
    queue_free(&q);
    EXPECT_EQ_UINT(queue_len(&q), 0);
}

UTEST_CASE(queue_clear)
{
    struct queue q;
    int x = 7;
    size_t avail;

    EXPECT_EQ_INT(queue_init(&q, sizeof(int), 4), 0);
    EXPECT_EQ_INT(queue_push(&q, &x), 0);
    avail = queue_avail(&q);
    queue_clear(&q);
    EXPECT_TRUE(queue_empty(&q));
    EXPECT_EQ_UINT(queue_avail(&q), avail + 1);
    EXPECT_EQ_INT(queue_pop(&q, &x), -ENOENT);
    queue_clear(&q);
    EXPECT_TRUE(queue_empty(&q));
    EXPECT_EQ_INT(queue_push(&q, &x), 0);
    EXPECT_EQ_UINT(queue_len(&q), 1);
    queue_clear(&q);
    EXPECT_EQ_UINT(queue_len(&q), 0);
    EXPECT_EQ_UINT(queue_avail(&q), avail + 1);
    queue_free(&q);
}

UTEST_CASE(queue_front)
{
    struct queue q;
    int a = 1, b = 2;

    EXPECT_EQ_INT(queue_init_empty(&q, sizeof(int)), 0);
    EXPECT_NULL(queue_front(&q));
    EXPECT_EQ_INT(queue_push(&q, &a), 0);
    EXPECT_EQ_INT(*(int *)queue_front(&q), 1);
    EXPECT_EQ_INT(queue_push(&q, &b), 0);
    EXPECT_EQ_INT(*(int *)queue_front(&q), 1);
    *(int *)queue_front(&q) = 9;
    EXPECT_EQ_INT(queue_pop(&q, &a), 0);
    EXPECT_EQ_INT(a, 9);
    EXPECT_EQ_INT(*(int *)queue_front(&q), 2);
    queue_free(&q);

    {
        struct rec r = {4, 'q', 0.5}, out;
        char c = 'm';

        EXPECT_EQ_INT(queue_init(&q, sizeof(c), 1), 0);
        EXPECT_EQ_INT(queue_push(&q, &c), 0);
        EXPECT_EQ_CHAR(*(char *)queue_front(&q), 'm');
        EXPECT_EQ_PTR(queue_front(&q), queue_back(&q));
        queue_free(&q);

        EXPECT_EQ_INT(queue_init_empty(&q, sizeof(r)), 0);
        EXPECT_EQ_INT(queue_push(&q, &r), 0);
        EXPECT_EQ_INT(((struct rec *)queue_front(&q))->id, 4);
        EXPECT_EQ_CHAR(((struct rec *)queue_front(&q))->tag, 'q');
        EXPECT_EQ_DOUBLE(((struct rec *)queue_front(&q))->score, 0.5);
        EXPECT_EQ_INT(queue_pop(&q, &out), 0);
        EXPECT_EQ_INT(out.id, 4);
        EXPECT_NULL(queue_front(&q));
        queue_free(&q);
    }
}

UTEST_CASE(queue_back)
{
    struct queue q;
    int a = 1, b = 2;

    EXPECT_EQ_INT(queue_init_empty(&q, sizeof(int)), 0);
    EXPECT_NULL(queue_back(&q));
    EXPECT_EQ_INT(queue_push(&q, &a), 0);
    EXPECT_EQ_INT(*(int *)queue_back(&q), 1);
    EXPECT_EQ_INT(queue_push(&q, &b), 0);
    EXPECT_EQ_INT(*(int *)queue_back(&q), 2);
    *(int *)queue_back(&q) = 8;
    EXPECT_EQ_INT(queue_pop(&q, &a), 0);
    EXPECT_EQ_INT(*(int *)queue_back(&q), 8);
    EXPECT_EQ_PTR(queue_front(&q), queue_back(&q));
    queue_free(&q);

    {
        double x = -1.5, y = 2.25;

        EXPECT_EQ_INT(queue_init_empty(&q, sizeof(double)), 0);
        EXPECT_EQ_INT(queue_push(&q, &x), 0);
        EXPECT_EQ_DOUBLE(*(double *)queue_back(&q), -1.5);
        EXPECT_EQ_INT(queue_push(&q, &y), 0);
        EXPECT_EQ_DOUBLE(*(double *)queue_back(&q), 2.25);
        EXPECT_EQ_DOUBLE(*(double *)queue_front(&q), -1.5);
        queue_free(&q);
    }
}

UTEST_CASE(queue_push)
{
    struct queue q;
    int a = 1, b = 2, c = 3, out = 0;

    EXPECT_EQ_INT(queue_init_empty(&q, sizeof(int)), 0);
    EXPECT_EQ_INT(queue_push(&q, &a), 0);
    EXPECT_EQ_INT(queue_push(&q, &b), 0);
    EXPECT_EQ_INT(queue_push(&q, &c), 0);
    EXPECT_EQ_UINT(queue_len(&q), 3);
    EXPECT_EQ_INT(queue_pop(&q, &out), 0);
    EXPECT_EQ_INT(out, 1);
    EXPECT_EQ_INT(queue_pop(&q, &out), 0);
    EXPECT_EQ_INT(out, 2);
    EXPECT_EQ_INT(queue_pop(&q, &out), 0);
    EXPECT_EQ_INT(out, 3);
    queue_free(&q);

    {
        struct rec a = {1, 'a', 1.5}, b = {2, 'b', -2.5}, got;
        char bytes[5] = {'w', 'x', 'y', 'z', 'q'};
        char ch;
        int i;

        EXPECT_EQ_INT(queue_init(&q, sizeof(a), 1), 0);
        EXPECT_EQ_INT(queue_push(&q, &a), 0);
        EXPECT_TRUE(queue_full(&q));
        EXPECT_EQ_INT(queue_push(&q, &b), 0);
        EXPECT_EQ_UINT(queue_len(&q), 2);
        EXPECT_EQ_INT(queue_pop(&q, &got), 0);
        EXPECT_EQ_INT(got.id, 1);
        EXPECT_EQ_CHAR(got.tag, 'a');
        EXPECT_EQ_DOUBLE(got.score, 1.5);
        EXPECT_EQ_INT(queue_pop(&q, &got), 0);
        EXPECT_EQ_INT(got.id, 2);
        EXPECT_EQ_DOUBLE(got.score, -2.5);
        queue_free(&q);

        EXPECT_EQ_INT(queue_init_empty(&q, sizeof(char)), 0);
        for (i = 0; i < 5; i++)
            EXPECT_EQ_INT(queue_push(&q, &bytes[i]), 0);
        EXPECT_EQ_UINT(queue_len(&q), 5);
        for (i = 0; i < 5; i++) {
            EXPECT_EQ_INT(queue_pop(&q, &ch), 0);
            EXPECT_EQ_CHAR(ch, bytes[i]);
        }
        EXPECT_TRUE(queue_empty(&q));
        queue_free(&q);
    }
}

UTEST_CASE(queue_pop)
{
    struct queue q;
    int a = 1, b = 2, out = 0;

    EXPECT_EQ_INT(queue_init_empty(&q, sizeof(int)), 0);
    EXPECT_EQ_INT(queue_pop(&q, &out), -ENOENT);
    EXPECT_EQ_INT(queue_push(&q, &a), 0);
    EXPECT_EQ_INT(queue_push(&q, &b), 0);
    EXPECT_EQ_INT(queue_pop(&q, &out), 0);
    EXPECT_EQ_INT(out, 1);
    EXPECT_EQ_UINT(queue_len(&q), 1);
    EXPECT_EQ_INT(*(int *)queue_front(&q), 2);
    EXPECT_EQ_INT(queue_pop(&q, NULL), 0);
    EXPECT_TRUE(queue_empty(&q));
    EXPECT_EQ_INT(queue_pop(&q, &out), -ENOENT);
    queue_free(&q);

    {
        double vals[3] = {0.0, -0.5, 8.25};
        double got;
        int i;

        EXPECT_EQ_INT(queue_init(&q, sizeof(double), 1), 0);
        EXPECT_EQ_INT(queue_pop(&q, &got), -ENOENT);
        for (i = 0; i < 3; i++)
            EXPECT_EQ_INT(queue_push(&q, &vals[i]), 0);
        for (i = 0; i < 3; i++) {
            EXPECT_EQ_INT(queue_pop(&q, &got), 0);
            EXPECT_EQ_DOUBLE(got, vals[i]);
        }
        EXPECT_TRUE(queue_empty(&q));
        EXPECT_EQ_INT(queue_pop(&q, &got), -ENOENT);
        queue_free(&q);
    }
}

UTEST_CASE(queue_grow)
{
    struct queue q;
    int a = 1, b = 2, c = 3, out = 0;

    EXPECT_EQ_INT(queue_init(&q, sizeof(int), 2), 0);
    EXPECT_EQ_INT(queue_push(&q, &a), 0);
    EXPECT_EQ_INT(queue_push(&q, &b), 0);
    EXPECT_TRUE(queue_full(&q));
    EXPECT_EQ_INT(queue_push(&q, &c), 0);
    EXPECT_EQ_UINT(queue_len(&q), 3);
    EXPECT_FALSE(queue_full(&q));
    EXPECT_EQ_INT(queue_pop(&q, &out), 0);
    EXPECT_EQ_INT(out, 1);
    EXPECT_EQ_INT(queue_pop(&q, &out), 0);
    EXPECT_EQ_INT(out, 2);
    EXPECT_EQ_INT(queue_pop(&q, &out), 0);
    EXPECT_EQ_INT(out, 3);
    queue_free(&q);
}

UTEST_CASE(queue_wrap)
{
    struct queue q;
    int a = 1, b = 2, c = 3, e = 4, f = 5, out = 0;

    EXPECT_EQ_INT(queue_init(&q, sizeof(int), 3), 0);
    EXPECT_EQ_INT(queue_push(&q, &a), 0);
    EXPECT_EQ_INT(queue_push(&q, &b), 0);
    EXPECT_EQ_INT(queue_push(&q, &c), 0);
    EXPECT_EQ_INT(queue_pop(&q, &out), 0);
    EXPECT_EQ_INT(out, 1);
    EXPECT_EQ_INT(queue_pop(&q, &out), 0);
    EXPECT_EQ_INT(out, 2);
    EXPECT_EQ_INT(queue_push(&q, &e), 0);
    EXPECT_EQ_INT(queue_push(&q, &f), 0);
    EXPECT_TRUE(queue_full(&q));
    EXPECT_EQ_INT(*(int *)queue_front(&q), 3);
    EXPECT_EQ_INT(*(int *)queue_back(&q), 5);
    EXPECT_EQ_INT(queue_pop(&q, &out), 0);
    EXPECT_EQ_INT(out, 3);
    EXPECT_EQ_INT(queue_pop(&q, &out), 0);
    EXPECT_EQ_INT(out, 4);
    EXPECT_EQ_INT(queue_pop(&q, &out), 0);
    EXPECT_EQ_INT(out, 5);
    queue_free(&q);
}

UTEST_SUITE(queue)
{
    UTEST_RUNCASE(queue_len);
    UTEST_RUNCASE(queue_elesz);
    UTEST_RUNCASE(queue_empty);
    UTEST_RUNCASE(queue_full);
    UTEST_RUNCASE(queue_avail);
    UTEST_RUNCASE(queue_init);
    UTEST_RUNCASE(queue_init_empty);
    UTEST_RUNCASE(queue_free);
    UTEST_RUNCASE(queue_clear);
    UTEST_RUNCASE(queue_front);
    UTEST_RUNCASE(queue_back);
    UTEST_RUNCASE(queue_push);
    UTEST_RUNCASE(queue_pop);
    UTEST_RUNCASE(queue_grow);
    UTEST_RUNCASE(queue_wrap);
}
