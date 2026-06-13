#include <errno.h>
#include <stack.h>
#include <stdint.h>
#include <string.h>
#include <utest.h>

static int edg_dtor_n;
static void edg_dtor_inc(void *p)
{
  (void)p;
  edg_dtor_n++;
}

UTEST_CASE(edge)
{
  {
    struct stack s;

    EXPECT_EQ_INT(stack_init(&s, sizeof(int), NULL), 0);
    EXPECT_TRUE(stack_empty(&s));
    EXPECT_NULL(stack_peek(&s));
    EXPECT_EQ_INT(stack_pop(&s, NULL), -1);
    stack_fini(&s);
  }

  {
    struct stack s;
    int x;

    EXPECT_EQ_INT(stack_init(&s, sizeof(int), NULL), 0);
    x = 1;
    EXPECT_EQ_INT(stack_push(&s, &x), 0);
    EXPECT_EQ_INT(stack_pop(&s, NULL), 0);
    EXPECT_EQ_INT(stack_pop(&s, NULL), -1);
    stack_fini(&s);
  }

  {
    struct stack s;
    int x;

    errno = 0;
    EXPECT_EQ_INT(stack_init(&s, SIZE_MAX, NULL), 0);
    x = 0;
    EXPECT_EQ_INT(stack_push(&s, &x), -1);
    EXPECT_EQ_INT(errno, ERANGE);
    EXPECT_EQ_UINT(stack_size(&s), 0);
    stack_fini(&s);
  }

  {
    struct stack s;
    int x;

    EXPECT_EQ_INT(stack_init(&s, sizeof(int), NULL), 0);
    x = 1;
    EXPECT_EQ_INT(stack_push(&s, &x), 0);
    EXPECT_EQ_INT(stack_pop(&s, NULL), 0);
    EXPECT_TRUE(stack_empty(&s));
    EXPECT_NULL(stack_peek(&s));
    EXPECT_EQ_INT(stack_pop(&s, NULL), -1);
    stack_fini(&s);
  }

  {
    struct stack s;
    int x;

    EXPECT_EQ_INT(stack_init(&s, sizeof(int), NULL), 0);
    x = 1;
    EXPECT_EQ_INT(stack_push(&s, &x), 0);
    stack_clear(&s);
    EXPECT_TRUE(stack_empty(&s));
    EXPECT_EQ_INT(stack_pop(&s, NULL), -1);
    stack_fini(&s);
  }

  {
    struct stack s;
    int x;

    EXPECT_EQ_INT(stack_init(&s, sizeof(int), NULL), 0);
    x = 1;
    EXPECT_EQ_INT(stack_push(&s, &x), 0);
    stack_clear(&s);
    stack_clear(&s);
    EXPECT_TRUE(stack_empty(&s));
    stack_fini(&s);
  }

  {
    struct stack s;

    EXPECT_EQ_INT(stack_init(&s, sizeof(int), NULL), 0);
    stack_clear(&s);
    stack_clear(&s);
    EXPECT_TRUE(stack_empty(&s));
    stack_fini(&s);
  }

  {
    struct stack s;
    int x, out;
    void *p0, *p1;

    EXPECT_EQ_INT(stack_init(&s, sizeof(int), NULL), 0);
    x = 1;
    EXPECT_EQ_INT(stack_push(&s, &x), 0);
    x = 2;
    EXPECT_EQ_INT(stack_push(&s, &x), 0);
    p0 = stack_peek(&s);
    p1 = stack_peek(&s);
    EXPECT_NOTNULL(p0);
    EXPECT_EQ_PTR(p0, p1);
    EXPECT_EQ_INT(*(int *)p0, 2);
    out = 0;
    EXPECT_EQ_INT(stack_pop(&s, &out), 0);
    EXPECT_EQ_INT(out, 2);
    EXPECT_NOTNULL(stack_peek(&s));
    EXPECT_EQ_INT(*(int *)stack_peek(&s), 1);
    stack_fini(&s);
  }

  {
    struct stack s;
    uint8_t a, b, out;

    EXPECT_EQ_INT(stack_init(&s, sizeof(uint8_t), NULL), 0);
    a = 200;
    b = 201;
    EXPECT_EQ_INT(stack_push(&s, &a), 0);
    EXPECT_EQ_INT(stack_push(&s, &b), 0);
    EXPECT_EQ_UINT(*(uint8_t *)stack_peek(&s), 201);
    out = 0;
    EXPECT_EQ_INT(stack_pop(&s, &out), 0);
    EXPECT_EQ_UINT(out, 201);
    EXPECT_EQ_UINT(*(uint8_t *)stack_peek(&s), 200);
    stack_fini(&s);
  }

  {
    struct stack s;
    int16_t a, b, out;

    EXPECT_EQ_INT(stack_init(&s, sizeof(int16_t), NULL), 0);
    a = -30000;
    b = 30000;
    EXPECT_EQ_INT(stack_push(&s, &a), 0);
    EXPECT_EQ_INT(stack_push(&s, &b), 0);
    out = 0;
    EXPECT_EQ_INT(stack_pop(&s, &out), 0);
    EXPECT_EQ_INT(out, 30000);
    EXPECT_EQ_INT(stack_pop(&s, &out), 0);
    EXPECT_EQ_INT(out, -30000);
    stack_fini(&s);
  }

  {
    struct pair {
      char u;
      char v;
    } s0, s1, out;
    struct stack s;

    EXPECT_EQ_INT(stack_init(&s, sizeof(struct pair), NULL), 0);
    s0.u = 1;
    s0.v = 2;
    s1.u = 3;
    s1.v = 4;
    EXPECT_EQ_INT(stack_push(&s, &s0), 0);
    EXPECT_EQ_INT(stack_push(&s, &s1), 0);
    memset(&out, 0, sizeof(out));
    EXPECT_EQ_INT(stack_pop(&s, &out), 0);
    EXPECT_EQ_CHAR(out.u, 3);
    EXPECT_EQ_CHAR(out.v, 4);
    stack_fini(&s);
  }

  {
    struct stack s;
    int x;
    size_t sz0, sz1;

    EXPECT_EQ_INT(stack_init(&s, sizeof(int), NULL), 0);
    x = 1;
    sz0 = stack_size(&s);
    EXPECT_EQ_INT(stack_push(&s, NULL), -1);
    sz1 = stack_size(&s);
    EXPECT_EQ_UINT(sz0, sz1);
    EXPECT_EQ_INT(stack_push(&s, &x), 0);
    stack_fini(&s);
  }

  {
    struct stack s;
    int x, y;

    EXPECT_EQ_INT(stack_init(&s, sizeof(int), NULL), 0);
    x = 100;
    EXPECT_EQ_INT(stack_push(&s, &x), 0);
    x = 200;
    EXPECT_EQ_INT(stack_push(&s, &x), 0);
    y = 0;
    EXPECT_EQ_INT(stack_pop(&s, &y), 0);
    EXPECT_EQ_INT(y, 200);
    x = 300;
    EXPECT_EQ_INT(stack_push(&s, &x), 0);
    EXPECT_EQ_INT(*(int *)stack_peek(&s), 300);
    y = 0;
    EXPECT_EQ_INT(stack_pop(&s, &y), 0);
    EXPECT_EQ_INT(y, 300);
    y = 0;
    EXPECT_EQ_INT(stack_pop(&s, &y), 0);
    EXPECT_EQ_INT(y, 100);
    stack_fini(&s);
  }

  {
    struct stack s;
    int x, out;

    EXPECT_EQ_INT(stack_init(&s, sizeof(int), NULL), 0);
    x = 42;
    EXPECT_EQ_INT(stack_push(&s, &x), 0);
    out = 0;
    EXPECT_EQ_INT(stack_pop(&s, &out), 0);
    EXPECT_EQ_INT(out, 42);
    EXPECT_TRUE(stack_empty(&s));
    EXPECT_NULL(stack_peek(&s));
    stack_fini(&s);
  }

  {
    struct stack s;
    int a, b;

    edg_dtor_n = 0;
    EXPECT_EQ_INT(stack_init(&s, sizeof(int), edg_dtor_inc), 0);
    a = 1;
    b = 2;
    stack_push(&s, &a);
    stack_push(&s, &b);
    stack_fini(&s);
    EXPECT_EQ_INT(edg_dtor_n, 2);
  }

  {
    struct stack s;

    edg_dtor_n = 0;
    EXPECT_EQ_INT(stack_init(&s, sizeof(int), edg_dtor_inc), 0);
    stack_clear(&s);
    EXPECT_EQ_INT(edg_dtor_n, 0);
    stack_fini(&s);
  }

  {
    struct stack s;
    int x;

    edg_dtor_n = 0;
    EXPECT_EQ_INT(stack_init(&s, sizeof(int), edg_dtor_inc), 0);
    x = 1;
    stack_push(&s, &x);
    stack_clear(&s);
    EXPECT_EQ_INT(edg_dtor_n, 1);
    stack_fini(&s);
  }

  {
    struct stack s;
    int x, out;

    edg_dtor_n = 0;
    EXPECT_EQ_INT(stack_init(&s, sizeof(int), edg_dtor_inc), 0);
    x = 9;
    stack_push(&s, &x);
    out = 0;
    EXPECT_EQ_INT(stack_pop(&s, &out), 0);
    EXPECT_EQ_INT(out, 9);
    EXPECT_EQ_INT(edg_dtor_n, 0);
    stack_fini(&s);
  }

  {
    struct stack s;
    int x;

    edg_dtor_n = 0;
    EXPECT_EQ_INT(stack_init(&s, sizeof(int), edg_dtor_inc), 0);
    x = 1;
    stack_push(&s, &x);
    x = 2;
    stack_push(&s, &x);
    EXPECT_EQ_INT(stack_pop(&s, NULL), 0);
    EXPECT_EQ_INT(edg_dtor_n, 1);
    EXPECT_EQ_INT(stack_pop(&s, NULL), 0);
    EXPECT_EQ_INT(edg_dtor_n, 2);
    stack_fini(&s);
  }
}
