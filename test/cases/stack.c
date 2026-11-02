/* Tests for stack. */

#include <errno.h>
#include <stack.h>
#include <utest.h>

struct rec {
    int id;
    char tag;
    double score;
};

UTEST_CASE(stack_len)
{
    struct stack s;
    int x = 1;

    EXPECT_EQ_INT(stack_init_empty(&s, sizeof(int)), 0);
    EXPECT_EQ_UINT(stack_len(&s), 0);
    EXPECT_EQ_INT(stack_push(&s, &x), 0);
    EXPECT_EQ_UINT(stack_len(&s), 1);
    EXPECT_EQ_INT(stack_pop(&s, &x), 0);
    EXPECT_EQ_UINT(stack_len(&s), 0);
    stack_free(&s);
}

UTEST_CASE(stack_elesz)
{
    struct stack s;

    EXPECT_EQ_INT(stack_init_empty(&s, sizeof(int)), 0);
    EXPECT_EQ_UINT(stack_elesz(&s), sizeof(int));
    stack_free(&s);

    EXPECT_EQ_INT(stack_init_empty(&s, sizeof(double)), 0);
    EXPECT_EQ_UINT(stack_elesz(&s), sizeof(double));
    stack_free(&s);

    EXPECT_EQ_INT(stack_init(&s, sizeof(struct rec), 2), 0);
    EXPECT_EQ_UINT(stack_elesz(&s), sizeof(struct rec));
    stack_free(&s);
}

UTEST_CASE(stack_empty)
{
    struct stack s;
    int x = 1;

    EXPECT_EQ_INT(stack_init_empty(&s, sizeof(int)), 0);
    EXPECT_TRUE(stack_empty(&s));
    EXPECT_EQ_INT(stack_push(&s, &x), 0);
    EXPECT_FALSE(stack_empty(&s));
    EXPECT_EQ_INT(stack_pop(&s, &x), 0);
    EXPECT_TRUE(stack_empty(&s));
    stack_free(&s);
}

UTEST_CASE(stack_full)
{
    struct stack s;
    int a = 1, b = 2;

    EXPECT_EQ_INT(stack_init(&s, sizeof(int), 2), 0);
    EXPECT_FALSE(stack_full(&s));
    EXPECT_EQ_INT(stack_push(&s, &a), 0);
    EXPECT_FALSE(stack_full(&s));
    EXPECT_EQ_INT(stack_push(&s, &b), 0);
    EXPECT_TRUE(stack_full(&s));
    EXPECT_EQ_UINT(stack_avail(&s), 0);
    EXPECT_EQ_INT(stack_pop(&s, &a), 0);
    EXPECT_FALSE(stack_full(&s));
    stack_free(&s);

    {
        char c = 'z';

        EXPECT_EQ_INT(stack_init(&s, sizeof(c), 1), 0);
        EXPECT_FALSE(stack_full(&s));
        EXPECT_EQ_INT(stack_push(&s, &c), 0);
        EXPECT_TRUE(stack_full(&s));
        EXPECT_EQ_UINT(stack_avail(&s), 0);
        stack_free(&s);
    }
}

UTEST_CASE(stack_avail)
{
    struct stack s;
    int x = 1;

    EXPECT_EQ_INT(stack_init(&s, sizeof(int), 4), 0);
    EXPECT_EQ_UINT(stack_avail(&s), 4);
    EXPECT_EQ_INT(stack_push(&s, &x), 0);
    EXPECT_EQ_UINT(stack_avail(&s), 3);
    EXPECT_EQ_INT(stack_push(&s, &x), 0);
    EXPECT_EQ_INT(stack_push(&s, &x), 0);
    EXPECT_EQ_INT(stack_push(&s, &x), 0);
    EXPECT_EQ_UINT(stack_avail(&s), 0);
    EXPECT_EQ_INT(stack_pop(&s, &x), 0);
    EXPECT_EQ_UINT(stack_avail(&s), 1);
    stack_free(&s);
}

UTEST_CASE(stack_init)
{
    struct stack s;

    EXPECT_EQ_INT(stack_init(&s, 0, 4), -EINVAL);

    EXPECT_EQ_INT(stack_init(&s, sizeof(int), 4), 0);
    EXPECT_EQ_UINT(stack_len(&s), 0);
    EXPECT_EQ_UINT(stack_elesz(&s), sizeof(int));
    EXPECT_TRUE(stack_empty(&s));
    EXPECT_EQ_UINT(stack_avail(&s), 4);
    stack_free(&s);

    EXPECT_EQ_INT(stack_init(&s, sizeof(int), 0), 0);
    EXPECT_TRUE(stack_empty(&s));
    EXPECT_TRUE(stack_full(&s));
    EXPECT_EQ_UINT(stack_avail(&s), 0);
    stack_free(&s);

    EXPECT_EQ_INT(stack_init(&s, sizeof(char), 1), 0);
    EXPECT_EQ_UINT(stack_elesz(&s), sizeof(char));
    EXPECT_EQ_UINT(stack_avail(&s), 1);
    EXPECT_FALSE(stack_full(&s));
    stack_free(&s);

    EXPECT_EQ_INT(stack_init(&s, sizeof(struct rec), 3), 0);
    EXPECT_EQ_UINT(stack_elesz(&s), sizeof(struct rec));
    EXPECT_EQ_UINT(stack_len(&s), 0);
    stack_free(&s);
}

UTEST_CASE(stack_init_empty)
{
    struct stack s;

    EXPECT_EQ_INT(stack_init_empty(&s, 0), -EINVAL);

    EXPECT_EQ_INT(stack_init_empty(&s, sizeof(int)), 0);
    EXPECT_EQ_UINT(stack_len(&s), 0);
    EXPECT_TRUE(stack_empty(&s));
    EXPECT_TRUE(stack_full(&s));
    stack_free(&s);

    EXPECT_EQ_INT(stack_init_empty(&s, sizeof(double)), 0);
    EXPECT_EQ_UINT(stack_elesz(&s), sizeof(double));
    stack_free(&s);

    EXPECT_EQ_INT(stack_init_empty(&s, sizeof(struct rec)), 0);
    EXPECT_EQ_UINT(stack_elesz(&s), sizeof(struct rec));
    stack_free(&s);
}

UTEST_CASE(stack_free)
{
    struct stack s;
    int x = 1;

    stack_free(NULL);

    EXPECT_EQ_INT(stack_init_empty(&s, sizeof(int)), 0);
    EXPECT_EQ_INT(stack_push(&s, &x), 0);
    stack_free(&s);
    EXPECT_EQ_UINT(stack_len(&s), 0);
}

UTEST_CASE(stack_clear)
{
    struct stack s;
    int x = 7;
    size_t avail;

    EXPECT_EQ_INT(stack_init(&s, sizeof(int), 4), 0);
    EXPECT_EQ_INT(stack_push(&s, &x), 0);
    avail = stack_avail(&s);
    stack_clear(&s);
    EXPECT_TRUE(stack_empty(&s));
    EXPECT_EQ_UINT(stack_avail(&s), avail + 1);
    EXPECT_EQ_INT(stack_pop(&s, &x), -ENOENT);
    stack_clear(&s);
    EXPECT_TRUE(stack_empty(&s));
    EXPECT_EQ_INT(stack_push(&s, &x), 0);
    EXPECT_EQ_UINT(stack_len(&s), 1);
    stack_clear(&s);
    EXPECT_EQ_UINT(stack_len(&s), 0);
    EXPECT_EQ_UINT(stack_avail(&s), avail + 1);
    stack_free(&s);
}

UTEST_CASE(stack_push)
{
    struct stack s;
    int a = 1, b = 2, c = 3, out = 0;

    EXPECT_EQ_INT(stack_init_empty(&s, sizeof(int)), 0);
    EXPECT_EQ_INT(stack_push(&s, &a), 0);
    EXPECT_EQ_INT(stack_push(&s, &b), 0);
    EXPECT_EQ_INT(stack_push(&s, &c), 0);
    EXPECT_EQ_UINT(stack_len(&s), 3);
    EXPECT_EQ_INT(stack_pop(&s, &out), 0);
    EXPECT_EQ_INT(out, 3);
    EXPECT_EQ_INT(stack_pop(&s, &out), 0);
    EXPECT_EQ_INT(out, 2);
    EXPECT_EQ_INT(stack_pop(&s, &out), 0);
    EXPECT_EQ_INT(out, 1);
    stack_free(&s);

    {
        struct rec a = {1, 'a', 1.5}, b = {2, 'b', -3.25}, got;
        char bytes[4] = {'a', 'b', 'c', 'd'};
        char ch;
        int i;

        EXPECT_EQ_INT(stack_init(&s, sizeof(a), 1), 0);
        EXPECT_EQ_INT(stack_push(&s, &a), 0);
        EXPECT_TRUE(stack_full(&s));
        EXPECT_EQ_INT(stack_push(&s, &b), 0);
        EXPECT_EQ_INT(stack_pop(&s, &got), 0);
        EXPECT_EQ_INT(got.id, 2);
        EXPECT_EQ_CHAR(got.tag, 'b');
        EXPECT_EQ_DOUBLE(got.score, -3.25);
        EXPECT_EQ_INT(stack_pop(&s, &got), 0);
        EXPECT_EQ_INT(got.id, 1);
        EXPECT_EQ_DOUBLE(got.score, 1.5);
        EXPECT_TRUE(stack_empty(&s));
        stack_free(&s);

        EXPECT_EQ_INT(stack_init_empty(&s, sizeof(char)), 0);
        for (i = 0; i < 4; i++)
            EXPECT_EQ_INT(stack_push(&s, &bytes[i]), 0);
        for (i = 3; i >= 0; i--) {
            EXPECT_EQ_INT(stack_pop(&s, &ch), 0);
            EXPECT_EQ_CHAR(ch, bytes[i]);
        }
        EXPECT_EQ_INT(stack_pop(&s, &ch), -ENOENT);
        stack_free(&s);
    }
}

UTEST_CASE(stack_pop)
{
    struct stack s;
    int a = 1, b = 2, out = 0;

    EXPECT_EQ_INT(stack_init_empty(&s, sizeof(int)), 0);
    EXPECT_EQ_INT(stack_pop(&s, &out), -ENOENT);
    EXPECT_EQ_INT(stack_push(&s, &a), 0);
    EXPECT_EQ_INT(stack_push(&s, &b), 0);
    EXPECT_EQ_INT(stack_pop(&s, &out), 0);
    EXPECT_EQ_INT(out, 2);
    EXPECT_EQ_UINT(stack_len(&s), 1);
    EXPECT_EQ_INT(stack_pop(&s, NULL), 0);
    EXPECT_TRUE(stack_empty(&s));
    EXPECT_EQ_INT(stack_pop(&s, &out), -ENOENT);
    stack_free(&s);

    {
        double x = 1.25, y = -8.0, got;

        EXPECT_EQ_INT(stack_init(&s, sizeof(double), 1), 0);
        EXPECT_EQ_INT(stack_push(&s, &x), 0);
        EXPECT_EQ_INT(stack_pop(&s, &got), 0);
        EXPECT_EQ_DOUBLE(got, 1.25);
        EXPECT_TRUE(stack_empty(&s));
        EXPECT_EQ_INT(stack_push(&s, &y), 0);
        EXPECT_EQ_INT(stack_pop(&s, &got), 0);
        EXPECT_EQ_DOUBLE(got, -8.0);
        EXPECT_EQ_INT(stack_pop(&s, &got), -ENOENT);
        stack_free(&s);
    }
}

UTEST_CASE(stack_peek)
{
    struct stack s;
    int a = 1, b = 2, out = 0;

    EXPECT_EQ_INT(stack_init_empty(&s, sizeof(int)), 0);
    EXPECT_EQ_INT(stack_peek(&s, &out), -ENOENT);
    EXPECT_EQ_INT(stack_push(&s, &a), 0);
    EXPECT_EQ_INT(stack_push(&s, &b), 0);
    EXPECT_EQ_INT(stack_peek(&s, &out), 0);
    EXPECT_EQ_INT(out, 2);
    EXPECT_EQ_UINT(stack_len(&s), 2);
    EXPECT_EQ_INT(stack_peek(&s, NULL), 0);
    EXPECT_EQ_UINT(stack_len(&s), 2);
    EXPECT_EQ_INT(stack_pop(&s, &out), 0);
    EXPECT_EQ_INT(out, 2);
    EXPECT_EQ_INT(stack_peek(&s, &out), 0);
    EXPECT_EQ_INT(out, 1);
    stack_free(&s);

    {
        struct rec a = {7, 'p', 0.25}, b = {8, 'q', 9.5}, got;
        double x = -4.5;

        EXPECT_EQ_INT(stack_init_empty(&s, sizeof(a)), 0);
        EXPECT_EQ_INT(stack_peek(&s, &got), -ENOENT);
        EXPECT_EQ_INT(stack_push(&s, &a), 0);
        EXPECT_EQ_INT(stack_push(&s, &b), 0);
        EXPECT_EQ_INT(stack_peek(&s, &got), 0);
        EXPECT_EQ_INT(got.id, 8);
        EXPECT_EQ_CHAR(got.tag, 'q');
        EXPECT_EQ_DOUBLE(got.score, 9.5);
        EXPECT_EQ_UINT(stack_len(&s), 2);
        EXPECT_EQ_INT(stack_pop(&s, &got), 0);
        EXPECT_EQ_INT(stack_peek(&s, &got), 0);
        EXPECT_EQ_INT(got.id, 7);
        stack_free(&s);

        EXPECT_EQ_INT(stack_init(&s, sizeof(double), 1), 0);
        EXPECT_EQ_INT(stack_push(&s, &x), 0);
        EXPECT_EQ_INT(stack_peek(&s, &x), 0);
        EXPECT_EQ_DOUBLE(x, -4.5);
        EXPECT_EQ_UINT(stack_len(&s), 1);
        stack_free(&s);
    }
}

UTEST_CASE(stack_grow)
{
    struct stack s;
    int a = 1, b = 2, c = 3, out = 0;

    EXPECT_EQ_INT(stack_init(&s, sizeof(int), 2), 0);
    EXPECT_EQ_INT(stack_push(&s, &a), 0);
    EXPECT_EQ_INT(stack_push(&s, &b), 0);
    EXPECT_TRUE(stack_full(&s));
    EXPECT_EQ_INT(stack_push(&s, &c), 0);
    EXPECT_EQ_UINT(stack_len(&s), 3);
    EXPECT_FALSE(stack_full(&s));
    EXPECT_EQ_INT(stack_pop(&s, &out), 0);
    EXPECT_EQ_INT(out, 3);
    EXPECT_EQ_INT(stack_pop(&s, &out), 0);
    EXPECT_EQ_INT(out, 2);
    stack_free(&s);
}

UTEST_SUITE(stack)
{
    UTEST_RUNCASE(stack_len);
    UTEST_RUNCASE(stack_elesz);
    UTEST_RUNCASE(stack_empty);
    UTEST_RUNCASE(stack_full);
    UTEST_RUNCASE(stack_avail);
    UTEST_RUNCASE(stack_init);
    UTEST_RUNCASE(stack_init_empty);
    UTEST_RUNCASE(stack_free);
    UTEST_RUNCASE(stack_clear);
    UTEST_RUNCASE(stack_push);
    UTEST_RUNCASE(stack_pop);
    UTEST_RUNCASE(stack_peek);
    UTEST_RUNCASE(stack_grow);
}
