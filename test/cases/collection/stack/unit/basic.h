#include <stack.h>
#include <utest.h>

static int dtor_n;
static void dtor_inc(void *p)
{
  (void)p;
  dtor_n++;
}

UTEST_CASE(basic)
{
  {
    struct stack s;

    EXPECT_EQ_INT(stack_init(NULL, sizeof(int), NULL), -1);
    EXPECT_EQ_INT(stack_init(&s, 0, NULL), -1);
  }

  {
    struct stack s;

    EXPECT_EQ_INT(stack_init(&s, sizeof(int), NULL), 0);
    EXPECT_TRUE(stack_empty(&s));
    EXPECT_EQ_UINT(stack_size(&s), 0u);
    EXPECT_NULL(stack_peek(&s));
    EXPECT_EQ_INT(stack_pop(&s, NULL), -1);
    stack_fini(&s);
  }

  {
    struct stack s;
    int x;
    void *p;

    EXPECT_EQ_INT(stack_init(&s, sizeof(int), NULL), 0);
    x = 10;
    EXPECT_EQ_INT(stack_push(&s, &x), 0);
    EXPECT_FALSE(stack_empty(&s));
    EXPECT_EQ_UINT(stack_size(&s), 1u);
    p = stack_peek(&s);
    EXPECT_NOTNULL(p);
    EXPECT_EQ_INT(*(int *)p, 10);
    EXPECT_EQ_PTR(stack_peek(&s), p);
    stack_fini(&s);
  }

  {
    struct stack s;
    int a, b, c, out;
    void *p;

    EXPECT_EQ_INT(stack_init(&s, sizeof(int), NULL), 0);
    a = 1;
    b = 2;
    c = 3;
    EXPECT_EQ_INT(stack_push(&s, &a), 0);
    EXPECT_EQ_INT(stack_push(&s, &b), 0);
    EXPECT_EQ_INT(stack_push(&s, &c), 0);
    EXPECT_EQ_UINT(stack_size(&s), 3u);
    p = stack_peek(&s);
    EXPECT_NOTNULL(p);
    EXPECT_EQ_INT(*(int *)p, 3);
    out = 0;
    EXPECT_EQ_INT(stack_pop(&s, &out), 0);
    EXPECT_EQ_INT(out, 3);
    EXPECT_EQ_INT(stack_pop(&s, &out), 0);
    EXPECT_EQ_INT(out, 2);
    EXPECT_EQ_INT(stack_pop(&s, &out), 0);
    EXPECT_EQ_INT(out, 1);
    EXPECT_TRUE(stack_empty(&s));
    EXPECT_EQ_INT(stack_pop(&s, NULL), -1);
    stack_fini(&s);
  }

  {
    struct stack s;
    int a, b, c;

    EXPECT_EQ_INT(stack_init(&s, sizeof(int), NULL), 0);
    a = 5;
    b = 6;
    c = 7;
    stack_push(&s, &a);
    stack_push(&s, &b);
    stack_push(&s, &c);
    stack_clear(&s);
    EXPECT_TRUE(stack_empty(&s));
    EXPECT_EQ_UINT(stack_size(&s), 0u);
    EXPECT_EQ_INT(stack_push(&s, &a), 0);
    EXPECT_EQ_UINT(stack_size(&s), 1u);
    stack_fini(&s);
  }

  {
    struct stack s;
    double a, b, out;

    EXPECT_EQ_INT(stack_init(&s, sizeof(double), NULL), 0);
    a = 3.14;
    b = 2.71;
    EXPECT_EQ_INT(stack_push(&s, &a), 0);
    EXPECT_EQ_INT(stack_push(&s, &b), 0);
    EXPECT_EQ_UINT(stack_size(&s), 2u);
    EXPECT_NOTNULL(stack_peek(&s));
    EXPECT_EQ_DOUBLE(*(double *)stack_peek(&s), 2.71);
    out = 0.0;
    EXPECT_EQ_INT(stack_pop(&s, &out), 0);
    EXPECT_EQ_DOUBLE(out, 2.71);
    EXPECT_EQ_DOUBLE(*(double *)stack_peek(&s), 3.14);
    stack_fini(&s);
  }

  {
    struct stack s;
    int x;

    EXPECT_EQ_INT(stack_init(&s, sizeof(int), NULL), 0);
    x = 1;
    EXPECT_EQ_INT(stack_push(&s, &x), 0);
    x = 2;
    EXPECT_EQ_INT(stack_push(&s, &x), 0);
    EXPECT_EQ_INT(*(int *)stack_peek(&s), 2);
    x = 3;
    EXPECT_EQ_INT(stack_push(&s, &x), 0);
    EXPECT_EQ_INT(*(int *)stack_peek(&s), 3);
    stack_fini(&s);
  }

  {
    struct stack s;

    dtor_n = 0;
    EXPECT_EQ_INT(stack_init(&s, sizeof(int), dtor_inc), 0);
    stack_fini(&s);
    EXPECT_EQ_INT(dtor_n, 0);
  }

  {
    struct stack s;
    int a, b, c;

    dtor_n = 0;
    EXPECT_EQ_INT(stack_init(&s, sizeof(int), dtor_inc), 0);
    a = 1;
    b = 2;
    c = 3;
    stack_push(&s, &a);
    stack_push(&s, &b);
    stack_push(&s, &c);
    stack_clear(&s);
    EXPECT_EQ_INT(dtor_n, 3);
    stack_fini(&s);
  }

  {
    struct stack s;
    int x;

    dtor_n = 0;
    EXPECT_EQ_INT(stack_init(&s, sizeof(int), dtor_inc), 0);
    x = 7;
    stack_push(&s, &x);
    EXPECT_EQ_INT(stack_pop(&s, NULL), 0);
    EXPECT_EQ_INT(dtor_n, 1);
    stack_fini(&s);
  }

  {
    struct stack s;
    int x, out;

    dtor_n = 0;
    EXPECT_EQ_INT(stack_init(&s, sizeof(int), dtor_inc), 0);
    x = 7;
    stack_push(&s, &x);
    out = 0;
    EXPECT_EQ_INT(stack_pop(&s, &out), 0);
    EXPECT_EQ_INT(out, 7);
    EXPECT_EQ_INT(dtor_n, 0);
    stack_fini(&s);
  }

  {
    int x;

    EXPECT_EQ_INT(stack_push(NULL, &x), -1);
    EXPECT_EQ_INT(stack_pop(NULL, &x), -1);
    EXPECT_NULL(stack_peek(NULL));
  }

  {
    struct stack s;
    int a;

    EXPECT_EQ_INT(stack_init(&s, sizeof(int), NULL), 0);
    a = 1;
    EXPECT_EQ_INT(stack_push(&s, NULL), -1);
    stack_push(&s, &a);
    stack_fini(&s);
  }

  {
    stack_fini(NULL);
    stack_clear(NULL);
  }
}
