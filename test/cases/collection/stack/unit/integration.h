#include <stack.h>
#include <stdlib.h>
#include <string.h>
#include <utest.h>

struct intg_pair {
  int x;
  int y;
};

struct intg_nested {
  short layer;
  int id;
};

struct intg_heap_box {
  int *p;
};

static int intg_dtor_n;
static void intg_dtor_inc(void *p)
{
  (void)p;
  intg_dtor_n++;
}

static int intg_heap_dtor_n;
static void intg_heap_dtor(void *ele)
{
  struct intg_heap_box *b = (struct intg_heap_box *)ele;
  free(b->p);
  b->p = NULL;
  intg_heap_dtor_n++;
}

UTEST_CASE(integration)
{
  {
    struct stack s;
    int x;
    size_t idx;
    void *p;

    EXPECT_EQ_INT(stack_init(&s, sizeof(int), NULL), 0);
    for (idx = 0; idx < 5; idx++) {
      x = (int)idx;
      EXPECT_EQ_INT(stack_push(&s, &x), 0);
    }
    EXPECT_EQ_UINT(stack_size(&s), 5);
    EXPECT_FALSE(stack_empty(&s));
    for (idx = 0; idx < 5; idx++) {
      p = stack_peek(&s);
      EXPECT_NOTNULL(p);
      EXPECT_EQ_INT(*(int *)p, (int)(4 - idx));
      x = 0;
      EXPECT_EQ_INT(stack_pop(&s, &x), 0);
      EXPECT_EQ_INT(x, (int)(4 - idx));
    }
    EXPECT_TRUE(stack_empty(&s));
    EXPECT_NULL(stack_peek(&s));
    stack_fini(&s);
  }

  {
    struct stack s;
    int x;
    int out;

    EXPECT_EQ_INT(stack_init(&s, sizeof(int), NULL), 0);
    x = 1;
    stack_push(&s, &x);
    x = 2;
    stack_push(&s, &x);
    x = 3;
    stack_push(&s, &x);
    EXPECT_EQ_INT(*(int *)stack_peek(&s), 3);
    out = 0;
    EXPECT_EQ_INT(stack_pop(&s, &out), 0);
    EXPECT_EQ_INT(out, 3);
    EXPECT_EQ_INT(*(int *)stack_peek(&s), 2);
    x = 4;
    EXPECT_EQ_INT(stack_push(&s, &x), 0);
    EXPECT_EQ_INT(*(int *)stack_peek(&s), 4);
    out = 0;
    EXPECT_EQ_INT(stack_pop(&s, &out), 0);
    EXPECT_EQ_INT(out, 4);
    out = 0;
    EXPECT_EQ_INT(stack_pop(&s, &out), 0);
    EXPECT_EQ_INT(out, 2);
    out = 0;
    EXPECT_EQ_INT(stack_pop(&s, &out), 0);
    EXPECT_EQ_INT(out, 1);
    EXPECT_TRUE(stack_empty(&s));
    stack_fini(&s);
  }

  {
    struct stack s;
    int x;

    EXPECT_EQ_INT(stack_init(&s, sizeof(int), NULL), 0);
    stack_clear(&s);
    EXPECT_TRUE(stack_empty(&s));
    x = 10;
    stack_push(&s, &x);
    x = 20;
    EXPECT_EQ_INT(stack_push(&s, &x), 0);
    EXPECT_EQ_INT(*(int *)stack_peek(&s), 20);
    EXPECT_EQ_INT(stack_pop(&s, NULL), 0);
    EXPECT_EQ_UINT(stack_size(&s), 1);
    x = 30;
    EXPECT_EQ_INT(stack_push(&s, &x), 0);
    EXPECT_EQ_UINT(stack_size(&s), 2);
    EXPECT_EQ_INT(*(int *)stack_peek(&s), 30);
    EXPECT_NOTNULL(stack_peek(&s));
    stack_fini(&s);
  }

  {
    struct stack s;
    unsigned char a, b, c, out;

    EXPECT_EQ_INT(stack_init(&s, sizeof(unsigned char), NULL), 0);
    a = 7;
    b = 8;
    c = 9;
    EXPECT_EQ_INT(stack_push(&s, &a), 0);
    EXPECT_EQ_INT(stack_push(&s, &b), 0);
    EXPECT_EQ_INT(stack_push(&s, &c), 0);
    EXPECT_EQ_UINT(stack_size(&s), 3);
    out = 0;
    EXPECT_EQ_INT(stack_pop(&s, &out), 0);
    EXPECT_EQ_INT((int)out, 9);
    EXPECT_EQ_INT((int)*(unsigned char *)stack_peek(&s), 8);
    out = 0;
    EXPECT_EQ_INT(stack_pop(&s, &out), 0);
    EXPECT_EQ_INT((int)out, 8);
    EXPECT_EQ_INT((int)*(unsigned char *)stack_peek(&s), 7);
    stack_fini(&s);
  }

  {
    struct stack s;
    int x;
    size_t idx;

    intg_dtor_n = 0;
    EXPECT_EQ_INT(stack_init(&s, sizeof(int), intg_dtor_inc), 0);
    for (idx = 0; idx < 4; idx++) {
      x = (int)(idx + 10);
      stack_push(&s, &x);
    }
    EXPECT_EQ_INT(stack_pop(&s, NULL), 0);
    EXPECT_EQ_INT(stack_pop(&s, NULL), 0);
    EXPECT_EQ_INT(intg_dtor_n, 2);
    EXPECT_EQ_INT(*(int *)stack_peek(&s), 11);
    stack_clear(&s);
    EXPECT_EQ_INT(intg_dtor_n, 4);
    EXPECT_TRUE(stack_empty(&s));
    x = 1;
    stack_push(&s, &x);
    EXPECT_EQ_INT(stack_pop(&s, NULL), 0);
    EXPECT_EQ_INT(intg_dtor_n, 5);
    stack_fini(&s);
    EXPECT_EQ_INT(intg_dtor_n, 5);
  }

  {
    struct stack s;
    int x;
    int out;

    EXPECT_EQ_INT(stack_init(&s, sizeof(int), NULL), 0);
    x = 1;
    stack_push(&s, &x);
    x = 2;
    stack_push(&s, &x);
    x = 3;
    stack_push(&s, &x);
    stack_clear(&s);
    EXPECT_TRUE(stack_empty(&s));
    x = 7;
    stack_push(&s, &x);
    EXPECT_EQ_UINT(stack_size(&s), 1);
    EXPECT_EQ_INT(*(int *)stack_peek(&s), 7);
    x = 8;
    EXPECT_EQ_INT(stack_push(&s, &x), 0);
    EXPECT_EQ_INT(*(int *)stack_peek(&s), 8);
    out = 0;
    EXPECT_EQ_INT(stack_pop(&s, &out), 0);
    EXPECT_EQ_INT(out, 8);
    out = 0;
    EXPECT_EQ_INT(stack_pop(&s, &out), 0);
    EXPECT_EQ_INT(out, 7);
    EXPECT_TRUE(stack_empty(&s));
    stack_fini(&s);
  }

  {
    struct stack s;
    int x;
    int out;

    EXPECT_EQ_INT(stack_init(&s, sizeof(int), NULL), 0);
    x = 1;
    stack_push(&s, &x);
    x = 2;
    stack_push(&s, &x);
    x = 3;
    stack_push(&s, &x);
    out = 0;
    EXPECT_EQ_INT(stack_pop(&s, &out), 0);
    EXPECT_EQ_INT(out, 3);
    EXPECT_EQ_INT(stack_pop(&s, &out), 0);
    EXPECT_EQ_INT(out, 2);
    EXPECT_EQ_INT(stack_pop(&s, &out), 0);
    EXPECT_EQ_INT(out, 1);
    EXPECT_TRUE(stack_empty(&s));
    x = 100;
    stack_push(&s, &x);
    x = 200;
    stack_push(&s, &x);
    EXPECT_EQ_INT(*(int *)stack_peek(&s), 200);
    out = 0;
    EXPECT_EQ_INT(stack_pop(&s, &out), 0);
    EXPECT_EQ_INT(out, 200);
    EXPECT_EQ_INT(*(int *)stack_peek(&s), 100);
    stack_fini(&s);
  }

  {
    struct stack s;
    int x;
    size_t idx;

    EXPECT_EQ_INT(stack_init(&s, sizeof(int), NULL), 0);
    for (idx = 0; idx < 3; idx++) {
      x = (int)(3 - idx);
      EXPECT_EQ_INT(stack_push(&s, &x), 0);
    }
    EXPECT_EQ_UINT(stack_size(&s), 3);
    for (idx = 0; idx < 3; idx++) {
      EXPECT_NOTNULL(stack_peek(&s));
      EXPECT_EQ_INT(*(int *)stack_peek(&s), (int)(idx + 1));
      EXPECT_EQ_INT(stack_pop(&s, NULL), 0);
    }
    EXPECT_TRUE(stack_empty(&s));
    stack_fini(&s);
  }

  {
    struct stack s;
    int x;
    size_t idx;

    EXPECT_EQ_INT(stack_init(&s, sizeof(int), NULL), 0);
    for (idx = 0; idx < 5; idx++) {
      x = (int)(idx * 2);
      stack_push(&s, &x);
    }
    x = 1000;
    stack_push(&s, &x);
    x = 2000;
    stack_push(&s, &x);
    EXPECT_EQ_UINT(stack_size(&s), 7);
    EXPECT_EQ_INT(*(int *)stack_peek(&s), 2000);
    EXPECT_EQ_INT(stack_pop(&s, NULL), 0);
    EXPECT_EQ_INT(*(int *)stack_peek(&s), 1000);
    EXPECT_EQ_INT(stack_pop(&s, NULL), 0);
    EXPECT_EQ_INT(*(int *)stack_peek(&s), 8);
    EXPECT_EQ_INT(stack_pop(&s, NULL), 0);
    EXPECT_EQ_INT(*(int *)stack_peek(&s), 6);
    EXPECT_EQ_UINT(stack_size(&s), 4);
    stack_clear(&s);
    EXPECT_TRUE(stack_empty(&s));
    x = 9;
    stack_push(&s, &x);
    EXPECT_EQ_UINT(stack_size(&s), 1);
    stack_fini(&s);
  }

  {
    struct stack s;
    struct intg_pair a, b, c, out;

    EXPECT_EQ_INT(stack_init(&s, sizeof(struct intg_pair), NULL), 0);
    a.x = 1;
    a.y = 10;
    b.x = 2;
    b.y = 20;
    c.x = 3;
    c.y = 30;
    EXPECT_EQ_INT(stack_push(&s, &a), 0);
    EXPECT_EQ_INT(stack_push(&s, &b), 0);
    EXPECT_EQ_INT(stack_push(&s, &c), 0);
    EXPECT_NOTNULL(stack_peek(&s));
    EXPECT_EQ_INT(((struct intg_pair *)stack_peek(&s))->x, 3);
    EXPECT_EQ_INT(((struct intg_pair *)stack_peek(&s))->y, 30);
    memset(&out, 0, sizeof(out));
    EXPECT_EQ_INT(stack_pop(&s, &out), 0);
    EXPECT_EQ_INT(out.x, 3);
    EXPECT_EQ_INT(out.y, 30);
    EXPECT_EQ_INT(((struct intg_pair *)stack_peek(&s))->x, 2);
    a.x = 0;
    a.y = 0;
    EXPECT_EQ_INT(stack_push(&s, &a), 0);
    EXPECT_EQ_INT(((struct intg_pair *)stack_peek(&s))->y, 0);
    memset(&out, 0, sizeof(out));
    EXPECT_EQ_INT(stack_pop(&s, &out), 0);
    EXPECT_EQ_INT(out.x, 0);
    EXPECT_EQ_INT(out.y, 0);
    memset(&out, 0, sizeof(out));
    EXPECT_EQ_INT(stack_pop(&s, &out), 0);
    EXPECT_EQ_INT(out.x, 2);
    EXPECT_EQ_INT(out.y, 20);
    EXPECT_EQ_UINT(stack_size(&s), 1);
    stack_fini(&s);
  }

  {
    struct stack s;
    struct intg_nested n0, n1;

    EXPECT_EQ_INT(stack_init(&s, sizeof(struct intg_nested), NULL), 0);
    n0.layer = 7;
    n0.id = 700;
    n1.layer = 8;
    n1.id = 800;
    EXPECT_EQ_INT(stack_push(&s, &n0), 0);
    EXPECT_EQ_INT(stack_push(&s, &n1), 0);
    EXPECT_NOTNULL(stack_peek(&s));
    EXPECT_EQ_INT((int)((struct intg_nested *)stack_peek(&s))->layer, 8);
    EXPECT_EQ_INT(((struct intg_nested *)stack_peek(&s))->id, 800);
    EXPECT_EQ_INT(stack_pop(&s, NULL), 0);
    EXPECT_NOTNULL(stack_peek(&s));
    EXPECT_EQ_INT((int)((struct intg_nested *)stack_peek(&s))->layer, 7);
    EXPECT_EQ_INT(((struct intg_nested *)stack_peek(&s))->id, 700);
    stack_fini(&s);
  }

  {
    struct stack s;
    double a, b, c, out;

    EXPECT_EQ_INT(stack_init(&s, sizeof(double), NULL), 0);
    a = 0.25;
    b = 0.5;
    c = 0.75;
    EXPECT_EQ_INT(stack_push(&s, &a), 0);
    EXPECT_EQ_INT(stack_push(&s, &b), 0);
    EXPECT_EQ_INT(stack_push(&s, &c), 0);
    EXPECT_EQ_DOUBLE(*(double *)stack_peek(&s), 0.75);
    out = 0.0;
    EXPECT_EQ_INT(stack_pop(&s, &out), 0);
    EXPECT_EQ_DOUBLE(out, 0.75);
    stack_clear(&s);
    EXPECT_TRUE(stack_empty(&s));
    a = 1.0;
    EXPECT_EQ_INT(stack_push(&s, &a), 0);
    EXPECT_EQ_DOUBLE(*(double *)stack_peek(&s), 1.0);
    stack_fini(&s);
  }

  {
    struct stack s;
    struct intg_heap_box h;
    size_t idx;

    intg_heap_dtor_n = 0;
    EXPECT_EQ_INT(stack_init(&s, sizeof(struct intg_heap_box), intg_heap_dtor),
                  0);
    for (idx = 0; idx < 3; idx++) {
      h.p = (int *)malloc(sizeof(int));
      EXPECT_NOTNULL(h.p);
      *h.p = (int)(idx + 1);
      EXPECT_EQ_INT(stack_push(&s, &h), 0);
    }
    stack_clear(&s);
    EXPECT_EQ_INT(intg_heap_dtor_n, 3);
    EXPECT_TRUE(stack_empty(&s));
    stack_fini(&s);
    EXPECT_EQ_INT(intg_heap_dtor_n, 3);
  }

  {
    struct stack s;
    struct intg_heap_box h, out;

    intg_heap_dtor_n = 0;
    EXPECT_EQ_INT(stack_init(&s, sizeof(struct intg_heap_box), intg_heap_dtor),
                  0);
    h.p = (int *)malloc(sizeof(int));
    EXPECT_NOTNULL(h.p);
    *h.p = 77;
    EXPECT_EQ_INT(stack_push(&s, &h), 0);
    memset(&out, 0, sizeof(out));
    EXPECT_EQ_INT(stack_pop(&s, &out), 0);
    EXPECT_EQ_INT(intg_heap_dtor_n, 0);
    EXPECT_NOTNULL(out.p);
    EXPECT_EQ_INT(*out.p, 77);
    free(out.p);
    out.p = NULL;
    EXPECT_TRUE(stack_empty(&s));
    stack_fini(&s);
    EXPECT_EQ_INT(intg_heap_dtor_n, 0);
  }

  {
    struct stack s;
    struct intg_heap_box h0, h1, out;

    intg_heap_dtor_n = 0;
    EXPECT_EQ_INT(stack_init(&s, sizeof(struct intg_heap_box), intg_heap_dtor),
                  0);
    h0.p = (int *)malloc(sizeof(int));
    h1.p = (int *)malloc(sizeof(int));
    EXPECT_NOTNULL(h0.p);
    EXPECT_NOTNULL(h1.p);
    *h0.p = 1;
    *h1.p = 2;
    stack_push(&s, &h0);
    stack_push(&s, &h1);
    memset(&out, 0, sizeof(out));
    EXPECT_EQ_INT(stack_pop(&s, &out), 0);
    EXPECT_EQ_INT(intg_heap_dtor_n, 0);
    EXPECT_NOTNULL(out.p);
    EXPECT_EQ_INT(*out.p, 2);
    free(out.p);
    stack_fini(&s);
    EXPECT_EQ_INT(intg_heap_dtor_n, 1);
  }

  {
    struct stack s;
    struct intg_heap_box h;
    size_t idx;

    intg_heap_dtor_n = 0;
    EXPECT_EQ_INT(stack_init(&s, sizeof(struct intg_heap_box), intg_heap_dtor),
                  0);
    for (idx = 0; idx < 4; idx++) {
      h.p = (int *)malloc(sizeof(int));
      EXPECT_NOTNULL(h.p);
      *h.p = (int)(idx * 10);
      EXPECT_EQ_INT(stack_push(&s, &h), 0);
    }
    EXPECT_EQ_INT(stack_pop(&s, NULL), 0);
    EXPECT_EQ_INT(stack_pop(&s, NULL), 0);
    EXPECT_EQ_INT(intg_heap_dtor_n, 2);
    EXPECT_NOTNULL(stack_peek(&s));
    EXPECT_NOTNULL(((struct intg_heap_box *)stack_peek(&s))->p);
    EXPECT_EQ_INT(*((struct intg_heap_box *)stack_peek(&s))->p, 10);
    stack_fini(&s);
    EXPECT_EQ_INT(intg_heap_dtor_n, 4);
  }

  {
    struct stack s;
    struct intg_heap_box h;
    struct intg_heap_box *slot;

    intg_heap_dtor_n = 0;
    EXPECT_EQ_INT(stack_init(&s, sizeof(struct intg_heap_box), NULL), 0);
    h.p = (int *)malloc(sizeof(int));
    EXPECT_NOTNULL(h.p);
    *h.p = 99;
    EXPECT_EQ_INT(stack_push(&s, &h), 0);
    slot = (struct intg_heap_box *)stack_peek(&s);
    EXPECT_NOTNULL(slot);
    EXPECT_NOTNULL(slot->p);
    free(slot->p);
    slot->p = NULL;
    stack_clear(&s);
    stack_fini(&s);
    EXPECT_EQ_INT(intg_heap_dtor_n, 0);
  }

  {
    struct stack s;
    struct intg_heap_box h0, h1, h2, out;

    intg_heap_dtor_n = 0;
    EXPECT_EQ_INT(stack_init(&s, sizeof(struct intg_heap_box), intg_heap_dtor),
                  0);
    h0.p = (int *)malloc(sizeof(int));
    h1.p = (int *)malloc(sizeof(int));
    EXPECT_NOTNULL(h0.p);
    EXPECT_NOTNULL(h1.p);
    *h0.p = 1;
    *h1.p = 2;
    stack_push(&s, &h0);
    stack_push(&s, &h1);
    memset(&out, 0, sizeof(out));
    EXPECT_EQ_INT(stack_pop(&s, &out), 0);
    EXPECT_EQ_INT(*out.p, 2);
    h2.p = (int *)malloc(sizeof(int));
    EXPECT_NOTNULL(h2.p);
    *h2.p = 50;
    EXPECT_EQ_INT(stack_push(&s, &h2), 0);
    EXPECT_EQ_INT(stack_push(&s, &out), 0);
    EXPECT_EQ_INT(*((struct intg_heap_box *)stack_peek(&s))->p, 2);
    EXPECT_EQ_INT(stack_pop(&s, NULL), 0);
    EXPECT_EQ_INT(intg_heap_dtor_n, 1);
    stack_fini(&s);
    EXPECT_EQ_INT(intg_heap_dtor_n, 3);
  }

  {
    struct stack s;
    struct intg_heap_box h;

    intg_heap_dtor_n = 0;
    EXPECT_EQ_INT(stack_init(&s, sizeof(struct intg_heap_box), intg_heap_dtor),
                  0);
    h.p = (int *)malloc(sizeof(int));
    *h.p = 5;
    stack_push(&s, &h);
    EXPECT_EQ_INT(stack_pop(&s, NULL), 0);
    EXPECT_EQ_INT(intg_heap_dtor_n, 1);
    stack_fini(&s);
    EXPECT_EQ_INT(intg_heap_dtor_n, 1);
  }
}
