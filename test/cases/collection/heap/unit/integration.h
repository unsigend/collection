#include <heap.h>
#include <stdlib.h>
#include <string.h>
#include <utest.h>

struct intg_pair {
  int x;
  int y;
};

struct intg_nested {
  short tag;
  int id;
};

struct intg_strkv {
  char k[8];
  int ord;
};

static int intg_dtor_n;
static void intg_dtor_inc(void *p)
{
  (void)p;
  intg_dtor_n++;
}

static int intg_cmp_min_int(void *a, void *b)
{
  int x = *(int *)a;
  int y = *(int *)b;
  return (x > y) - (x < y);
}

static int intg_cmp_max_int(void *a, void *b)
{
  int x = *(int *)a;
  int y = *(int *)b;
  return (y > x) - (y < x);
}

static int intg_cmp_min_u(void *a, void *b)
{
  unsigned x = *(unsigned *)a;
  unsigned y = *(unsigned *)b;
  return (x > y) - (x < y);
}

static int intg_cmp_min_ll(void *a, void *b)
{
  long long x = *(long long *)a;
  long long y = *(long long *)b;
  return (x > y) - (x < y);
}

static int intg_cmp_min_double(void *a, void *b)
{
  double x = *(double *)a;
  double y = *(double *)b;
  return (x > y) - (x < y);
}

static int intg_cmp_pair_x(void *a, void *b)
{
  int xa = ((struct intg_pair *)a)->x;
  int xb = ((struct intg_pair *)b)->x;
  return (xa > xb) - (xa < xb);
}

static int intg_cmp_nested_id(void *a, void *b)
{
  int ia = ((struct intg_nested *)a)->id;
  int ib = ((struct intg_nested *)b)->id;
  return (ia > ib) - (ia < ib);
}

static int intg_cmp_abs_int(void *a, void *b)
{
  int x = *(int *)a;
  int y = *(int *)b;
  int ax = x < 0 ? -x : x;
  int ay = y < 0 ? -y : y;
  if (ax != ay)
    return (ax > ay) - (ax < ay);
  return (x > y) - (x < y);
}

static int intg_cmp_strkv(void *a, void *b)
{
  struct intg_strkv *sa = (struct intg_strkv *)a;
  struct intg_strkv *sb = (struct intg_strkv *)b;
  int c = strcmp(sa->k, sb->k);
  if (c < 0)
    return -1;
  if (c > 0)
    return 1;
  return (sa->ord > sb->ord) - (sa->ord < sb->ord);
}

static int intg_cmp_iptr_val(void *a, void *b)
{
  int xa = **(int **)a;
  int xb = **(int **)b;
  return (xa > xb) - (xa < xb);
}

UTEST_CASE(integration)
{
  {
    struct heap h;
    int buf[50];
    int out;
    int i, j;

    EXPECT_EQ_INT(heap_init(&h, sizeof(int), intg_cmp_min_int, NULL), 0);
    for (i = 0; i < 50; i++)
      buf[i] = i;
    for (j = 49; j >= 0; j--)
      EXPECT_EQ_INT(heap_push(&h, &buf[j]), 0);
    EXPECT_EQ_UINT(heap_size(&h), 50);
    for (i = 0; i < 50; i++) {
      out = -1;
      EXPECT_EQ_INT(heap_pop(&h, &out), 0);
      EXPECT_EQ_INT(out, i);
    }
    EXPECT_TRUE(heap_empty(&h));
    heap_fini(&h);
  }

  {
    struct heap h;
    int buf[50];
    int out;
    int i;

    EXPECT_EQ_INT(heap_init(&h, sizeof(int), intg_cmp_max_int, NULL), 0);
    for (i = 0; i < 50; i++)
      buf[i] = i;
    for (i = 0; i < 50; i++)
      EXPECT_EQ_INT(heap_push(&h, &buf[i]), 0);
    for (i = 0; i < 50; i++) {
      out = -1;
      EXPECT_EQ_INT(heap_pop(&h, &out), 0);
      EXPECT_EQ_INT(out, 49 - i);
    }
    EXPECT_TRUE(heap_empty(&h));
    heap_fini(&h);
  }

  {
    struct heap h;
    unsigned buf[32];
    unsigned out;
    size_t i;

    EXPECT_EQ_INT(heap_init(&h, sizeof(unsigned), intg_cmp_min_u, NULL), 0);
    for (i = 0; i < 32; i++)
      buf[i] = (unsigned)(31 - i) * 3u;
    for (i = 0; i < 32; i++)
      EXPECT_EQ_INT(heap_push(&h, &buf[i]), 0);
    for (i = 0; i < 32; i++) {
      out = 0;
      EXPECT_EQ_INT(heap_pop(&h, &out), 0);
      EXPECT_EQ_UINT(out, i * 3u);
    }
    heap_fini(&h);
  }

  {
    struct heap h;
    long long buf[24];
    long long out;
    int i;

    EXPECT_EQ_INT(heap_init(&h, sizeof(long long), intg_cmp_min_ll, NULL), 0);
    for (i = 0; i < 24; i++)
      buf[i] = (long long)i;
    for (i = 0; i < 24; i++)
      EXPECT_EQ_INT(heap_push(&h, &buf[23 - i]), 0);
    for (i = 0; i < 24; i++) {
      out = -1;
      EXPECT_EQ_INT(heap_pop(&h, &out), 0);
      EXPECT_EQ_INT((int)out, i);
    }
    EXPECT_TRUE(heap_empty(&h));
    heap_fini(&h);
  }

  {
    struct heap h;
    double buf[28];
    double out;
    int i;

    EXPECT_EQ_INT(heap_init(&h, sizeof(double), intg_cmp_min_double, NULL), 0);
    for (i = 0; i < 28; i++)
      buf[i] = (double)(27 - i) * 0.25 - 3.0;
    for (i = 0; i < 28; i++)
      EXPECT_EQ_INT(heap_push(&h, &buf[i]), 0);
    for (i = 0; i < 28; i++) {
      out = 0.0;
      EXPECT_EQ_INT(heap_pop(&h, &out), 0);
      EXPECT_EQ_DOUBLE(out, (double)i * 0.25 - 3.0);
    }
    heap_fini(&h);
  }

  {
    struct heap h;
    struct intg_pair a, b, c, d0, out;

    EXPECT_EQ_INT(
        heap_init(&h, sizeof(struct intg_pair), intg_cmp_pair_x, NULL), 0);
    a.x = 40;
    a.y = 1;
    b.x = 10;
    b.y = 2;
    c.x = 30;
    c.y = 3;
    d0.x = 20;
    d0.y = 4;
    EXPECT_EQ_INT(heap_push(&h, &a), 0);
    EXPECT_EQ_INT(heap_push(&h, &b), 0);
    EXPECT_EQ_INT(heap_push(&h, &c), 0);
    EXPECT_EQ_INT(heap_push(&h, &d0), 0);
    EXPECT_EQ_INT(heap_pop(&h, &out), 0);
    EXPECT_EQ_INT(out.x, 10);
    EXPECT_EQ_INT(out.y, 2);
    EXPECT_EQ_INT(heap_pop(&h, &out), 0);
    EXPECT_EQ_INT(out.x, 20);
    EXPECT_EQ_INT(heap_pop(&h, &out), 0);
    EXPECT_EQ_INT(out.x, 30);
    EXPECT_EQ_INT(heap_pop(&h, &out), 0);
    EXPECT_EQ_INT(out.x, 40);
    heap_fini(&h);
  }

  {
    struct heap h;
    struct intg_nested n0, n1, n2, out;

    EXPECT_EQ_INT(
        heap_init(&h, sizeof(struct intg_nested), intg_cmp_nested_id, NULL), 0);
    n0.tag = 1;
    n0.id = 300;
    n1.tag = 2;
    n1.id = 100;
    n2.tag = 3;
    n2.id = 200;
    EXPECT_EQ_INT(heap_push(&h, &n0), 0);
    EXPECT_EQ_INT(heap_push(&h, &n1), 0);
    EXPECT_EQ_INT(heap_push(&h, &n2), 0);
    EXPECT_EQ_INT(heap_pop(&h, &out), 0);
    EXPECT_EQ_INT(out.id, 100);
    EXPECT_EQ_INT(heap_pop(&h, &out), 0);
    EXPECT_EQ_INT(out.id, 200);
    EXPECT_EQ_INT(heap_pop(&h, &out), 0);
    EXPECT_EQ_INT(out.id, 300);
    heap_fini(&h);
  }

  {
    struct heap h;
    struct intg_strkv s0, s1, s2, out;

    EXPECT_EQ_INT(
        heap_init(&h, sizeof(struct intg_strkv), intg_cmp_strkv, NULL), 0);
    strcpy(s0.k, "z");
    s0.ord = 1;
    strcpy(s1.k, "a");
    s1.ord = 2;
    strcpy(s2.k, "m");
    s2.ord = 3;
    EXPECT_EQ_INT(heap_push(&h, &s0), 0);
    EXPECT_EQ_INT(heap_push(&h, &s1), 0);
    EXPECT_EQ_INT(heap_push(&h, &s2), 0);
    EXPECT_EQ_INT(heap_pop(&h, &out), 0);
    EXPECT_EQ_STR(out.k, "a");
    EXPECT_EQ_INT(heap_pop(&h, &out), 0);
    EXPECT_EQ_STR(out.k, "m");
    EXPECT_EQ_INT(heap_pop(&h, &out), 0);
    EXPECT_EQ_STR(out.k, "z");
    heap_fini(&h);
  }

  {
    struct heap h;
    int a, b, c, d0, out;
    void *pk;

    EXPECT_EQ_INT(heap_init(&h, sizeof(int), intg_cmp_abs_int, NULL), 0);
    a = -5;
    b = 2;
    c = -1;
    d0 = 3;
    EXPECT_EQ_INT(heap_push(&h, &a), 0);
    EXPECT_EQ_INT(heap_push(&h, &b), 0);
    EXPECT_EQ_INT(heap_push(&h, &c), 0);
    EXPECT_EQ_INT(heap_push(&h, &d0), 0);
    pk = heap_peek(&h);
    EXPECT_NOTNULL(pk);
    EXPECT_EQ_INT(*(int *)pk, -1);
    EXPECT_EQ_INT(heap_pop(&h, &out), 0);
    EXPECT_EQ_INT(out, -1);
    EXPECT_EQ_INT(heap_pop(&h, &out), 0);
    EXPECT_EQ_INT(out, 2);
    EXPECT_EQ_INT(heap_pop(&h, &out), 0);
    EXPECT_EQ_INT(out, 3);
    EXPECT_EQ_INT(heap_pop(&h, &out), 0);
    EXPECT_EQ_INT(out, -5);
    heap_fini(&h);
  }

  {
    struct heap h;
    int buf[40];
    int out;
    int i;
    static const int exp_seq[20] = {50,  51,  52,  53,  54,  55,  56,
                                    57,  58,  59,  105, 106, 107, 108,
                                    109, 110, 111, 112, 113, 114};

    EXPECT_EQ_INT(heap_init(&h, sizeof(int), intg_cmp_min_int, NULL), 0);
    for (i = 0; i < 15; i++) {
      buf[i] = i + 100;
      EXPECT_EQ_INT(heap_push(&h, &buf[i]), 0);
    }
    for (i = 0; i < 5; i++) {
      out = 0;
      EXPECT_EQ_INT(heap_pop(&h, &out), 0);
      EXPECT_EQ_INT(out, 100 + i);
    }
    for (i = 0; i < 10; i++) {
      buf[i] = i + 50;
      EXPECT_EQ_INT(heap_push(&h, &buf[i]), 0);
    }
    EXPECT_EQ_UINT(heap_size(&h), 20);
    for (i = 0; i < 20; i++) {
      out = -999;
      EXPECT_EQ_INT(heap_pop(&h, &out), 0);
      EXPECT_EQ_INT(out, exp_seq[i]);
    }
    EXPECT_TRUE(heap_empty(&h));
    heap_fini(&h);
  }

  {
    struct heap h;
    int buf[50];
    int i;

    intg_dtor_n = 0;
    EXPECT_EQ_INT(heap_init(&h, sizeof(int), intg_cmp_min_int, intg_dtor_inc),
                  0);
    for (i = 0; i < 50; i++) {
      buf[i] = i;
      EXPECT_EQ_INT(heap_push(&h, &buf[i]), 0);
    }
    heap_clear(&h);
    EXPECT_EQ_INT(intg_dtor_n, 50);
    EXPECT_TRUE(heap_empty(&h));
    heap_fini(&h);
  }

  {
    struct heap h;
    int *p0, *p1, *p2;
    int *outp;

    EXPECT_EQ_INT(heap_init(&h, sizeof(int *), intg_cmp_iptr_val, NULL), 0);
    p0 = (int *)malloc(sizeof(int));
    p1 = (int *)malloc(sizeof(int));
    p2 = (int *)malloc(sizeof(int));
    EXPECT_NOTNULL(p0);
    EXPECT_NOTNULL(p1);
    EXPECT_NOTNULL(p2);
    *p0 = 30;
    *p1 = 10;
    *p2 = 20;
    EXPECT_EQ_INT(heap_push(&h, &p0), 0);
    EXPECT_EQ_INT(heap_push(&h, &p1), 0);
    EXPECT_EQ_INT(heap_push(&h, &p2), 0);
    outp = NULL;
    EXPECT_EQ_INT(heap_pop(&h, &outp), 0);
    EXPECT_NOTNULL(outp);
    EXPECT_EQ_INT(*outp, 10);
    free(outp);
    outp = NULL;
    EXPECT_EQ_INT(heap_pop(&h, &outp), 0);
    EXPECT_NOTNULL(outp);
    EXPECT_EQ_INT(*outp, 20);
    free(outp);
    outp = NULL;
    EXPECT_EQ_INT(heap_pop(&h, &outp), 0);
    EXPECT_NOTNULL(outp);
    EXPECT_EQ_INT(*outp, 30);
    free(outp);
    heap_fini(&h);
  }
}
