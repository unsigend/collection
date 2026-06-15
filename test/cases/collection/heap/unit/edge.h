#include <heap.h>
#include <utest.h>

static int edg_cmp_int(void *a, void *b)
{
  int x = *(int *)a;
  int y = *(int *)b;
  return (x > y) - (x < y);
}

static int edg_cmp_double(void *a, void *b)
{
  double x = *(double *)a;
  double y = *(double *)b;
  return (x > y) - (x < y);
}

static int edg_dtor_n;
static void edg_dtor_inc(void *p)
{
  (void)p;
  edg_dtor_n++;
}

UTEST_CASE(edge)
{
  {
    struct heap h;
    int x;
    int out;

    EXPECT_EQ_INT(heap_init(&h, sizeof(int), edg_cmp_int, NULL), 0);
    x = 1;
    EXPECT_EQ_INT(heap_push(&h, &x), 0);
    EXPECT_EQ_INT(heap_pop(&h, NULL), 0);
    EXPECT_TRUE(heap_empty(&h));
    out = 0;
    EXPECT_EQ_INT(heap_pop(&h, &out), -1);
    EXPECT_EQ_INT(heap_pop(&h, NULL), -1);
    heap_fini(&h);
  }

  {
    struct heap h;
    int x;
    int out;

    EXPECT_EQ_INT(heap_init(&h, sizeof(int), edg_cmp_int, NULL), 0);
    x = 1;
    EXPECT_EQ_INT(heap_push(&h, &x), 0);
    out = 0;
    EXPECT_EQ_INT(heap_pop(&h, &out), 0);
    EXPECT_EQ_INT(out, 1);
    EXPECT_TRUE(heap_empty(&h));
    EXPECT_EQ_INT(heap_pop(&h, NULL), -1);
    heap_fini(&h);
  }

  {
    struct heap h;
    int x;

    EXPECT_EQ_INT(heap_init(&h, sizeof(int), edg_cmp_int, NULL), 0);
    x = 7;
    EXPECT_EQ_INT(heap_push(&h, &x), 0);
    heap_clear(&h);
    EXPECT_TRUE(heap_empty(&h));
    EXPECT_EQ_INT(heap_pop(&h, NULL), -1);
    heap_fini(&h);
  }

  {
    struct heap h;
    int x;

    EXPECT_EQ_INT(heap_init(&h, sizeof(int), edg_cmp_int, NULL), 0);
    x = 7;
    EXPECT_EQ_INT(heap_push(&h, &x), 0);
    heap_clear(&h);
    heap_clear(&h);
    EXPECT_TRUE(heap_empty(&h));
    EXPECT_EQ_UINT(heap_size(&h), 0);
    heap_fini(&h);
  }

  {
    struct heap h;
    int a, b, c;
    int out;
    void *pk;

    EXPECT_EQ_INT(heap_init(&h, sizeof(int), edg_cmp_int, NULL), 0);
    a = 5;
    b = 5;
    c = 5;
    EXPECT_EQ_INT(heap_push(&h, &a), 0);
    EXPECT_EQ_INT(heap_push(&h, &b), 0);
    EXPECT_EQ_INT(heap_push(&h, &c), 0);
    EXPECT_EQ_UINT(heap_size(&h), 3);
    out = 0;
    EXPECT_EQ_INT(heap_pop(&h, &out), 0);
    EXPECT_EQ_INT(out, 5);
    pk = heap_peek(&h);
    EXPECT_NOTNULL(pk);
    EXPECT_EQ_INT(*(int *)pk, 5);
    out = 0;
    EXPECT_EQ_INT(heap_pop(&h, &out), 0);
    EXPECT_EQ_INT(out, 5);
    out = 0;
    EXPECT_EQ_INT(heap_pop(&h, &out), 0);
    EXPECT_EQ_INT(out, 5);
    EXPECT_TRUE(heap_empty(&h));
    heap_fini(&h);
  }

  {
    struct heap h;
    int i;
    int buf[10];
    int out;
    int k;

    EXPECT_EQ_INT(heap_init(&h, sizeof(int), edg_cmp_int, NULL), 0);
    for (i = 0; i < 10; i++)
      buf[i] = 9 - i;
    for (i = 0; i < 10; i++)
      EXPECT_EQ_INT(heap_push(&h, &buf[i]), 0);
    for (k = 0; k < 10; k++) {
      out = -1;
      EXPECT_EQ_INT(heap_pop(&h, &out), 0);
      EXPECT_EQ_INT(out, k);
    }
    EXPECT_TRUE(heap_empty(&h));
    heap_fini(&h);
  }

  {
    struct heap h;
    int hi, lo;
    int out;

    EXPECT_EQ_INT(heap_init(&h, sizeof(int), edg_cmp_int, NULL), 0);
    hi = 100;
    lo = 1;
    EXPECT_EQ_INT(heap_push(&h, &hi), 0);
    EXPECT_EQ_INT(heap_push(&h, &lo), 0);
    out = 0;
    EXPECT_EQ_INT(heap_pop(&h, &out), 0);
    EXPECT_EQ_INT(out, 1);
    out = 0;
    EXPECT_EQ_INT(heap_pop(&h, &out), 0);
    EXPECT_EQ_INT(out, 100);
    heap_fini(&h);
  }

  {
    struct heap h;
    int a, b, c;
    int out;

    EXPECT_EQ_INT(heap_init(&h, sizeof(int), edg_cmp_int, NULL), 0);
    a = 1;
    b = 0;
    c = 2;
    EXPECT_EQ_INT(heap_push(&h, &a), 0);
    EXPECT_EQ_INT(heap_push(&h, &b), 0);
    out = 0;
    EXPECT_EQ_INT(heap_pop(&h, &out), 0);
    EXPECT_EQ_INT(out, 0);
    EXPECT_EQ_INT(heap_push(&h, &c), 0);
    out = 0;
    EXPECT_EQ_INT(heap_pop(&h, &out), 0);
    EXPECT_EQ_INT(out, 1);
    out = 0;
    EXPECT_EQ_INT(heap_pop(&h, &out), 0);
    EXPECT_EQ_INT(out, 2);
    heap_fini(&h);
  }

  {
    struct heap h;
    double u, v, w;
    double out;

    EXPECT_EQ_INT(heap_init(&h, sizeof(double), edg_cmp_double, NULL), 0);
    u = 3.0;
    v = 1.0;
    w = 2.0;
    EXPECT_EQ_INT(heap_push(&h, &u), 0);
    EXPECT_EQ_INT(heap_push(&h, &v), 0);
    EXPECT_EQ_INT(heap_push(&h, &w), 0);
    out = 0.0;
    EXPECT_EQ_INT(heap_pop(&h, &out), 0);
    EXPECT_EQ_DOUBLE(out, 1.0);
    out = 0.0;
    EXPECT_EQ_INT(heap_pop(&h, &out), 0);
    EXPECT_EQ_DOUBLE(out, 2.0);
    out = 0.0;
    EXPECT_EQ_INT(heap_pop(&h, &out), 0);
    EXPECT_EQ_DOUBLE(out, 3.0);
    heap_fini(&h);
  }

  {
    struct heap h;
    int x;
    void *pk0, *pk1;

    EXPECT_EQ_INT(heap_init(&h, sizeof(int), edg_cmp_int, NULL), 0);
    x = 42;
    EXPECT_EQ_INT(heap_push(&h, &x), 0);
    pk0 = heap_peek(&h);
    EXPECT_NOTNULL(pk0);
    EXPECT_EQ_INT(*(int *)pk0, 42);
    pk1 = heap_peek(&h);
    EXPECT_NOTNULL(pk1);
    EXPECT_EQ_PTR(pk0, pk1);
    EXPECT_EQ_UINT(heap_size(&h), 1);
    heap_fini(&h);
  }

  {
    struct heap h;
    int a, b;
    int out;

    edg_dtor_n = 0;
    EXPECT_EQ_INT(heap_init(&h, sizeof(int), edg_cmp_int, edg_dtor_inc), 0);
    a = 1;
    b = 2;
    EXPECT_EQ_INT(heap_push(&h, &a), 0);
    EXPECT_EQ_INT(heap_push(&h, &b), 0);
    out = 0;
    EXPECT_EQ_INT(heap_pop(&h, &out), 0);
    EXPECT_EQ_INT(out, 1);
    EXPECT_EQ_INT(edg_dtor_n, 0);
    EXPECT_EQ_INT(heap_pop(&h, NULL), 0);
    EXPECT_EQ_INT(edg_dtor_n, 1);
    heap_fini(&h);
  }

  {
    struct heap h;
    int a, b, out;

    edg_dtor_n = 0;
    EXPECT_EQ_INT(heap_init(&h, sizeof(int), edg_cmp_int, edg_dtor_inc), 0);
    a = 5;
    b = 9;
    EXPECT_EQ_INT(heap_push(&h, &a), 0);
    EXPECT_EQ_INT(heap_push(&h, &b), 0);
    out = -1;
    EXPECT_EQ_INT(heap_pop(&h, &out), 0);
    EXPECT_EQ_INT(out, 5);
    EXPECT_EQ_INT(edg_dtor_n, 0);
    EXPECT_EQ_UINT(heap_size(&h), 1);
    EXPECT_EQ_INT(heap_pop(&h, NULL), 0);
    EXPECT_EQ_INT(edg_dtor_n, 1);
    EXPECT_TRUE(heap_empty(&h));
    heap_fini(&h);
  }

  {
    struct heap h;
    int a, b, c, out;

    edg_dtor_n = 0;
    EXPECT_EQ_INT(heap_init(&h, sizeof(int), edg_cmp_int, edg_dtor_inc), 0);
    a = 4;
    b = 2;
    c = 6;
    EXPECT_EQ_INT(heap_push(&h, &a), 0);
    EXPECT_EQ_INT(heap_push(&h, &b), 0);
    EXPECT_EQ_INT(heap_push(&h, &c), 0);
    out = -1;
    EXPECT_EQ_INT(heap_pop(&h, &out), 0);
    EXPECT_EQ_INT(out, 2);
    EXPECT_EQ_INT(edg_dtor_n, 0);
    out = -1;
    EXPECT_EQ_INT(heap_pop(&h, &out), 0);
    EXPECT_EQ_INT(out, 4);
    EXPECT_EQ_INT(edg_dtor_n, 0);
    EXPECT_EQ_UINT(heap_size(&h), 1);
    EXPECT_EQ_INT(heap_pop(&h, NULL), 0);
    EXPECT_EQ_INT(edg_dtor_n, 1);
    heap_fini(&h);
  }

  {
    struct heap h;
    int x, out;

    edg_dtor_n = 0;
    EXPECT_EQ_INT(heap_init(&h, sizeof(int), edg_cmp_int, edg_dtor_inc), 0);
    x = 1;
    EXPECT_EQ_INT(heap_push(&h, &x), 0);
    out = -1;
    EXPECT_EQ_INT(heap_pop(&h, &out), 0);
    EXPECT_EQ_INT(out, 1);
    EXPECT_EQ_INT(edg_dtor_n, 0);
    EXPECT_EQ_INT(heap_pop(&h, NULL), -1);
    EXPECT_EQ_INT(edg_dtor_n, 0);
    x = 2;
    EXPECT_EQ_INT(heap_push(&h, &x), 0);
    EXPECT_EQ_INT(heap_pop(&h, NULL), 0);
    EXPECT_EQ_INT(edg_dtor_n, 1);
    heap_fini(&h);
  }

  {
    struct heap h;
    int x;

    edg_dtor_n = 0;
    EXPECT_EQ_INT(heap_init(&h, sizeof(int), edg_cmp_int, edg_dtor_inc), 0);
    x = 9;
    EXPECT_EQ_INT(heap_push(&h, &x), 0);
    heap_clear(&h);
    EXPECT_EQ_INT(edg_dtor_n, 1);
    EXPECT_TRUE(heap_empty(&h));
    heap_fini(&h);
  }

  {
    int x;
    void *outp;

    EXPECT_EQ_INT(heap_init(NULL, sizeof(int), edg_cmp_int, edg_dtor_inc), -1);
    EXPECT_EQ_INT(heap_push(NULL, &x), -1);
    outp = (void *)1;
    EXPECT_EQ_INT(heap_pop(NULL, &outp), -1);
  }

  {
    struct heap h;

    EXPECT_EQ_INT(heap_init(&h, sizeof(int), edg_cmp_int, NULL), 0);
    EXPECT_EQ_INT(heap_push(&h, NULL), -1);
    EXPECT_EQ_UINT(heap_size(&h), 0);
    heap_fini(&h);
  }

  {
    heap_fini(NULL);
    heap_clear(NULL);
  }

  {
    struct heap h;
    int a, b, c, out;

    EXPECT_EQ_INT(heap_init(&h, sizeof(int), edg_cmp_int, NULL), 0);
    a = -3;
    b = -1;
    c = -2;
    EXPECT_EQ_INT(heap_push(&h, &a), 0);
    EXPECT_EQ_INT(heap_push(&h, &b), 0);
    EXPECT_EQ_INT(heap_push(&h, &c), 0);
    out = 0;
    EXPECT_EQ_INT(heap_pop(&h, &out), 0);
    EXPECT_EQ_INT(out, -3);
    out = 0;
    EXPECT_EQ_INT(heap_pop(&h, &out), 0);
    EXPECT_EQ_INT(out, -2);
    out = 0;
    EXPECT_EQ_INT(heap_pop(&h, &out), 0);
    EXPECT_EQ_INT(out, -1);
    heap_fini(&h);
  }

  {
    struct heap h;
    int buf[15];
    int out;
    int i;

    EXPECT_EQ_INT(heap_init(&h, sizeof(int), edg_cmp_int, NULL), 0);
    for (i = 0; i < 15; i++)
      buf[i] = i;
    for (i = 0; i < 15; i++)
      EXPECT_EQ_INT(heap_push(&h, &buf[i]), 0);
    for (i = 0; i < 15; i++) {
      out = -1;
      EXPECT_EQ_INT(heap_pop(&h, &out), 0);
      EXPECT_EQ_INT(out, i);
    }
    heap_fini(&h);
  }

  {
    struct heap h;
    int a, b, c, d, e, out;

    EXPECT_EQ_INT(heap_init(&h, sizeof(int), edg_cmp_int, NULL), 0);
    a = 10;
    b = 5;
    EXPECT_EQ_INT(heap_push(&h, &a), 0);
    EXPECT_EQ_INT(heap_push(&h, &b), 0);
    out = -1;
    EXPECT_EQ_INT(heap_pop(&h, &out), 0);
    EXPECT_EQ_INT(out, 5);
    c = 3;
    d = 8;
    EXPECT_EQ_INT(heap_push(&h, &c), 0);
    EXPECT_EQ_INT(heap_push(&h, &d), 0);
    out = -1;
    EXPECT_EQ_INT(heap_pop(&h, &out), 0);
    EXPECT_EQ_INT(out, 3);
    e = 1;
    EXPECT_EQ_INT(heap_push(&h, &e), 0);
    out = -1;
    EXPECT_EQ_INT(heap_pop(&h, &out), 0);
    EXPECT_EQ_INT(out, 1);
    out = -1;
    EXPECT_EQ_INT(heap_pop(&h, &out), 0);
    EXPECT_EQ_INT(out, 8);
    out = -1;
    EXPECT_EQ_INT(heap_pop(&h, &out), 0);
    EXPECT_EQ_INT(out, 10);
    heap_fini(&h);
  }

  {
    struct heap h;
    int vals[6];
    int i;

    edg_dtor_n = 0;
    EXPECT_EQ_INT(heap_init(&h, sizeof(int), edg_cmp_int, edg_dtor_inc), 0);
    for (i = 0; i < 6; i++) {
      vals[i] = 60 - i;
      EXPECT_EQ_INT(heap_push(&h, &vals[i]), 0);
    }
    for (i = 0; i < 6; i++) {
      EXPECT_EQ_INT(heap_pop(&h, NULL), 0);
    }
    EXPECT_EQ_INT(edg_dtor_n, 6);
    EXPECT_TRUE(heap_empty(&h));
    heap_fini(&h);
  }

  {
    struct heap h;
    int a, b, c, out;

    edg_dtor_n = 0;
    EXPECT_EQ_INT(heap_init(&h, sizeof(int), edg_cmp_int, edg_dtor_inc), 0);
    a = 2;
    b = 5;
    c = 8;
    EXPECT_EQ_INT(heap_push(&h, &a), 0);
    EXPECT_EQ_INT(heap_push(&h, &b), 0);
    EXPECT_EQ_INT(heap_push(&h, &c), 0);
    out = -1;
    EXPECT_EQ_INT(heap_pop(&h, &out), 0);
    EXPECT_EQ_INT(out, 2);
    EXPECT_EQ_INT(edg_dtor_n, 0);
    heap_clear(&h);
    EXPECT_EQ_INT(edg_dtor_n, 2);
    EXPECT_TRUE(heap_empty(&h));
    heap_fini(&h);
  }

  {
    struct heap h;
    int a, b, out;

    edg_dtor_n = 0;
    EXPECT_EQ_INT(heap_init(&h, sizeof(int), edg_cmp_int, edg_dtor_inc), 0);
    a = 4;
    b = 7;
    EXPECT_EQ_INT(heap_push(&h, &a), 0);
    EXPECT_EQ_INT(heap_push(&h, &b), 0);
    out = -1;
    EXPECT_EQ_INT(heap_pop(&h, &out), 0);
    EXPECT_EQ_INT(out, 4);
    EXPECT_EQ_UINT(heap_size(&h), 1);
    EXPECT_NOTNULL(heap_peek(&h));
    EXPECT_EQ_INT(*(int *)heap_peek(&h), 7);
    a = 1;
    EXPECT_EQ_INT(heap_push(&h, &a), 0);
    out = -1;
    EXPECT_EQ_INT(heap_pop(&h, &out), 0);
    EXPECT_EQ_INT(out, 1);
    EXPECT_EQ_INT(edg_dtor_n, 0);
    EXPECT_EQ_INT(heap_pop(&h, NULL), 0);
    EXPECT_EQ_INT(edg_dtor_n, 1);
    heap_fini(&h);
  }
}
