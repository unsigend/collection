#include <heap.h>
#include <utest.h>

static int cmp_int(void *a, void *b)
{
  int x = *(int *)a;
  int y = *(int *)b;
  return (x > y) - (x < y);
}

static int dtor_n;
static void dtor_inc(void *p)
{
  (void)p;
  dtor_n++;
}

UTEST_CASE(basic)
{
  {
    struct heap h;

    EXPECT_EQ_INT(heap_init(NULL, sizeof(int), cmp_int, NULL), -1);
    EXPECT_EQ_INT(heap_init(&h, 0, cmp_int, NULL), -1);
    EXPECT_EQ_INT(heap_init(&h, sizeof(int), NULL, NULL), -1);
    EXPECT_EQ_INT(heap_init(&h, sizeof(int), cmp_int, NULL), 0);
    heap_fini(&h);
  }

  {
    struct heap h;
    int out;

    EXPECT_EQ_INT(heap_init(&h, sizeof(int), cmp_int, NULL), 0);
    EXPECT_TRUE(heap_empty(&h));
    EXPECT_EQ_UINT(heap_size(&h), 0);
    EXPECT_NULL(heap_peek(&h));
    out = 0;
    EXPECT_EQ_INT(heap_pop(&h, &out), -1);
    EXPECT_EQ_INT(heap_pop(&h, NULL), -1);
    heap_fini(&h);
  }

  {
    struct heap h;
    int x;
    void *pk;

    EXPECT_EQ_INT(heap_init(&h, sizeof(int), cmp_int, NULL), 0);
    x = 10;
    EXPECT_EQ_INT(heap_push(&h, &x), 0);
    EXPECT_FALSE(heap_empty(&h));
    EXPECT_EQ_UINT(heap_size(&h), 1);
    pk = heap_peek(&h);
    EXPECT_NOTNULL(pk);
    EXPECT_EQ_INT(*(int *)pk, 10);
    heap_fini(&h);
  }

  {
    struct heap h;
    int a, b, c, out;

    EXPECT_EQ_INT(heap_init(&h, sizeof(int), cmp_int, NULL), 0);
    a = 3;
    b = 1;
    c = 2;
    EXPECT_EQ_INT(heap_push(&h, &a), 0);
    EXPECT_EQ_INT(heap_push(&h, &b), 0);
    EXPECT_EQ_INT(heap_push(&h, &c), 0);
    EXPECT_EQ_UINT(heap_size(&h), 3);
    out = 0;
    EXPECT_EQ_INT(heap_pop(&h, &out), 0);
    EXPECT_EQ_INT(out, 1);
    out = 0;
    EXPECT_EQ_INT(heap_pop(&h, &out), 0);
    EXPECT_EQ_INT(out, 2);
    out = 0;
    EXPECT_EQ_INT(heap_pop(&h, &out), 0);
    EXPECT_EQ_INT(out, 3);
    EXPECT_TRUE(heap_empty(&h));
    EXPECT_EQ_INT(heap_pop(&h, &out), -1);
    EXPECT_EQ_INT(heap_pop(&h, NULL), -1);
    heap_fini(&h);
  }

  {
    struct heap h;
    int a, b;
    void *pk;
    size_t sz0, sz1;

    EXPECT_EQ_INT(heap_init(&h, sizeof(int), cmp_int, NULL), 0);
    a = 5;
    b = 3;
    EXPECT_EQ_INT(heap_push(&h, &a), 0);
    EXPECT_EQ_INT(heap_push(&h, &b), 0);
    sz0 = heap_size(&h);
    pk = heap_peek(&h);
    EXPECT_NOTNULL(pk);
    EXPECT_EQ_INT(*(int *)pk, 3);
    sz1 = heap_size(&h);
    EXPECT_EQ_UINT(sz0, sz1);
    heap_fini(&h);
  }

  {
    struct heap h;
    int a, b, c;

    EXPECT_EQ_INT(heap_init(&h, sizeof(int), cmp_int, NULL), 0);
    a = 5;
    b = 6;
    c = 7;
    heap_push(&h, &a);
    heap_push(&h, &b);
    heap_push(&h, &c);
    heap_clear(&h);
    EXPECT_TRUE(heap_empty(&h));
    EXPECT_EQ_UINT(heap_size(&h), 0);
    EXPECT_NULL(heap_peek(&h));
    EXPECT_EQ_INT(heap_push(&h, &a), 0);
    EXPECT_EQ_UINT(heap_size(&h), 1);
    heap_fini(&h);
  }

  {
    struct heap h;

    dtor_n = 0;
    EXPECT_EQ_INT(heap_init(&h, sizeof(int), cmp_int, dtor_inc), 0);
    heap_fini(&h);
    EXPECT_EQ_INT(dtor_n, 0);
  }

  {
    struct heap h;
    int a, b, c;

    dtor_n = 0;
    EXPECT_EQ_INT(heap_init(&h, sizeof(int), cmp_int, dtor_inc), 0);
    a = 1;
    b = 2;
    c = 3;
    heap_push(&h, &a);
    heap_push(&h, &b);
    heap_push(&h, &c);
    heap_clear(&h);
    EXPECT_EQ_INT(dtor_n, 3);
    heap_fini(&h);
  }

  {
    struct heap h;
    int x, out;

    dtor_n = 0;
    EXPECT_EQ_INT(heap_init(&h, sizeof(int), cmp_int, dtor_inc), 0);
    x = 42;
    EXPECT_EQ_INT(heap_push(&h, &x), 0);
    out = -1;
    EXPECT_EQ_INT(heap_pop(&h, &out), 0);
    EXPECT_EQ_INT(out, 42);
    EXPECT_EQ_INT(dtor_n, 0);
    EXPECT_TRUE(heap_empty(&h));
    heap_fini(&h);
    EXPECT_EQ_INT(dtor_n, 0);
  }

  {
    struct heap h;
    int x;

    dtor_n = 0;
    EXPECT_EQ_INT(heap_init(&h, sizeof(int), cmp_int, dtor_inc), 0);
    x = 7;
    EXPECT_EQ_INT(heap_push(&h, &x), 0);
    EXPECT_EQ_INT(heap_pop(&h, NULL), 0);
    EXPECT_EQ_INT(dtor_n, 1);
    EXPECT_TRUE(heap_empty(&h));
    heap_fini(&h);
    EXPECT_EQ_INT(dtor_n, 1);
  }

  {
    struct heap h;
    int x;

    EXPECT_EQ_INT(heap_init(&h, sizeof(int), cmp_int, NULL), 0);
    x = 8;
    EXPECT_EQ_INT(heap_push(&h, &x), 0);
    EXPECT_EQ_INT(heap_pop(&h, NULL), 0);
    EXPECT_TRUE(heap_empty(&h));
    heap_fini(&h);
  }

  {
    struct heap h;
    int a, b, c, out;

    dtor_n = 0;
    EXPECT_EQ_INT(heap_init(&h, sizeof(int), cmp_int, dtor_inc), 0);
    a = 3;
    b = 1;
    c = 2;
    EXPECT_EQ_INT(heap_push(&h, &a), 0);
    EXPECT_EQ_INT(heap_push(&h, &b), 0);
    EXPECT_EQ_INT(heap_push(&h, &c), 0);
    EXPECT_EQ_UINT(heap_size(&h), 3);
    out = -1;
    EXPECT_EQ_INT(heap_pop(&h, &out), 0);
    EXPECT_EQ_INT(out, 1);
    EXPECT_EQ_INT(dtor_n, 0);
    EXPECT_EQ_UINT(heap_size(&h), 2);
    heap_fini(&h);
    EXPECT_EQ_INT(dtor_n, 2);
  }

  {
    struct heap h;
    int a, b, c, out;

    dtor_n = 0;
    EXPECT_EQ_INT(heap_init(&h, sizeof(int), cmp_int, dtor_inc), 0);
    a = 2;
    b = 1;
    c = 3;
    heap_push(&h, &a);
    heap_push(&h, &b);
    heap_push(&h, &c);
    out = 0;
    EXPECT_EQ_INT(heap_pop(&h, &out), 0);
    EXPECT_EQ_INT(out, 1);
    EXPECT_EQ_INT(dtor_n, 0);
    out = 0;
    EXPECT_EQ_INT(heap_pop(&h, &out), 0);
    EXPECT_EQ_INT(out, 2);
    EXPECT_EQ_INT(dtor_n, 0);
    out = 0;
    EXPECT_EQ_INT(heap_pop(&h, &out), 0);
    EXPECT_EQ_INT(out, 3);
    EXPECT_EQ_INT(dtor_n, 0);
    heap_fini(&h);
    EXPECT_EQ_INT(dtor_n, 0);
  }

  {
    struct heap h;
    int a, b, c, out;

    dtor_n = 0;
    EXPECT_EQ_INT(heap_init(&h, sizeof(int), cmp_int, dtor_inc), 0);
    a = 2;
    b = 1;
    c = 3;
    heap_push(&h, &a);
    heap_push(&h, &b);
    heap_push(&h, &c);
    out = 0;
    EXPECT_EQ_INT(heap_pop(&h, &out), 0);
    EXPECT_EQ_INT(out, 1);
    EXPECT_EQ_INT(dtor_n, 0);
    out = 0;
    EXPECT_EQ_INT(heap_pop(&h, &out), 0);
    EXPECT_EQ_INT(out, 2);
    EXPECT_EQ_INT(dtor_n, 0);
    out = -1;
    EXPECT_EQ_INT(heap_pop(&h, NULL), 0);
    EXPECT_EQ_INT(dtor_n, 1);
    EXPECT_TRUE(heap_empty(&h));
    heap_fini(&h);
    EXPECT_EQ_INT(dtor_n, 1);
  }

  {
    int x;
    void *outp;
    struct heap h;

    EXPECT_EQ_INT(heap_push(NULL, &x), -1);
    outp = (void *)1;
    EXPECT_EQ_INT(heap_pop(NULL, &outp), -1);
    EXPECT_EQ_INT(heap_init(&h, sizeof(int), cmp_int, NULL), 0);
    EXPECT_EQ_INT(heap_push(&h, NULL), -1);
    heap_fini(&h);
    EXPECT_NULL(heap_peek(NULL));
  }

  {
    heap_fini(NULL);
    heap_clear(NULL);
  }

  {
    struct heap h;
    int a, b, c, d, out;
    void *pk;

    EXPECT_EQ_INT(heap_init(&h, sizeof(int), cmp_int, NULL), 0);
    a = 4;
    b = 2;
    c = 6;
    d = 1;
    EXPECT_EQ_INT(heap_push(&h, &a), 0);
    EXPECT_EQ_INT(heap_push(&h, &b), 0);
    EXPECT_EQ_INT(heap_push(&h, &c), 0);
    EXPECT_EQ_INT(heap_push(&h, &d), 0);
    out = -1;
    EXPECT_EQ_INT(heap_pop(&h, &out), 0);
    EXPECT_EQ_INT(out, 1);
    pk = heap_peek(&h);
    EXPECT_NOTNULL(pk);
    EXPECT_EQ_INT(*(int *)pk, 2);
    EXPECT_EQ_UINT(heap_size(&h), 3);
    out = -1;
    EXPECT_EQ_INT(heap_pop(&h, &out), 0);
    EXPECT_EQ_INT(out, 2);
    EXPECT_EQ_UINT(heap_size(&h), 2);
    heap_fini(&h);
  }

  {
    struct heap h;
    int vals[5];
    int out;
    int i;

    EXPECT_EQ_INT(heap_init(&h, sizeof(int), cmp_int, NULL), 0);
    for (i = 0; i < 5; i++) {
      vals[i] = i;
      EXPECT_EQ_INT(heap_push(&h, &vals[i]), 0);
    }
    for (i = 0; i < 5; i++) {
      out = -1;
      EXPECT_EQ_INT(heap_pop(&h, &out), 0);
      EXPECT_EQ_INT(out, i);
    }
    EXPECT_TRUE(heap_empty(&h));
    vals[0] = 99;
    EXPECT_EQ_INT(heap_push(&h, &vals[0]), 0);
    EXPECT_EQ_UINT(heap_size(&h), 1);
    out = -1;
    EXPECT_EQ_INT(heap_pop(&h, &out), 0);
    EXPECT_EQ_INT(out, 99);
    heap_fini(&h);
  }

  {
    struct heap h;
    int a, b, c, d, out;

    dtor_n = 0;
    EXPECT_EQ_INT(heap_init(&h, sizeof(int), cmp_int, dtor_inc), 0);
    a = 8;
    b = 3;
    c = 9;
    d = 1;
    EXPECT_EQ_INT(heap_push(&h, &a), 0);
    EXPECT_EQ_INT(heap_push(&h, &b), 0);
    EXPECT_EQ_INT(heap_push(&h, &c), 0);
    EXPECT_EQ_INT(heap_push(&h, &d), 0);
    out = -1;
    EXPECT_EQ_INT(heap_pop(&h, NULL), 0);
    EXPECT_EQ_INT(dtor_n, 1);
    out = -1;
    EXPECT_EQ_INT(heap_pop(&h, NULL), 0);
    EXPECT_EQ_INT(dtor_n, 2);
    out = -1;
    EXPECT_EQ_INT(heap_pop(&h, &out), 0);
    EXPECT_EQ_INT(out, 8);
    EXPECT_EQ_INT(dtor_n, 2);
    out = -1;
    EXPECT_EQ_INT(heap_pop(&h, NULL), 0);
    EXPECT_EQ_INT(dtor_n, 3);
    EXPECT_TRUE(heap_empty(&h));
    heap_fini(&h);
    EXPECT_EQ_INT(dtor_n, 3);
  }
}
