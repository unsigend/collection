#include "common.h"
#include <limits.h>

typedef struct {
  int id;
  char tag;
} sort_massrec;

static int sort_cmp_massrec(void *a, void *b)
{
  const sort_massrec *x = (const sort_massrec *)a;
  const sort_massrec *y = (const sort_massrec *)b;
  if (x->id < y->id)
    return -1;
  if (x->id > y->id)
    return 1;
  return (int)x->tag - (int)y->tag;
}

typedef struct {
  int k;
  unsigned char tail[300 - sizeof(int)];
} sort_big300;

static int sort_cmp_big300(void *a, void *b)
{
  int x = ((const sort_big300 *)a)->k;
  int y = ((const sort_big300 *)b)->k;
  return (x > y) - (x < y);
}

UTEST_CASE(edge)
{
  {
    int buf[] = {2, 1};
    EXPECT_EQ_INT(sort_under_test(buf, 2, sizeof *buf, sort_cmp_int), 0);
    EXPECT_EQ_INT(buf[0], 1);
    EXPECT_EQ_INT(buf[1], 2);
  }

  {
    int buf[] = {1, 1};
    EXPECT_EQ_INT(sort_under_test(buf, 2, sizeof *buf, sort_cmp_int), 0);
    EXPECT_EQ_INT(buf[0], 1);
    EXPECT_EQ_INT(buf[1], 1);
  }

  {
    int buf[] = {1, 2, 3};
    EXPECT_EQ_INT(sort_under_test(buf, 3, sizeof *buf, sort_cmp_int), 0);
    EXPECT_EQ_INT(buf[0], 1);
    EXPECT_EQ_INT(buf[1], 2);
    EXPECT_EQ_INT(buf[2], 3);
  }

  {
    int buf[] = {1, 3, 2};
    EXPECT_EQ_INT(sort_under_test(buf, 3, sizeof *buf, sort_cmp_int), 0);
    EXPECT_TRUE(sort_sorted(buf, 3, sizeof *buf, sort_cmp_int));
  }

  {
    int buf[] = {3, 1, 2};
    EXPECT_EQ_INT(sort_under_test(buf, 3, sizeof *buf, sort_cmp_int), 0);
    EXPECT_TRUE(sort_sorted(buf, 3, sizeof *buf, sort_cmp_int));
  }

  {
    int v = 42;
    EXPECT_EQ_INT(sort_under_test(&v, 1, sizeof v, sort_cmp_int), 0);
    EXPECT_EQ_INT(v, 42);
  }

  {
    double v = 2.5;
    EXPECT_EQ_INT(sort_under_test(&v, 1, sizeof v, sort_cmp_double), 0);
    EXPECT_EQ_DOUBLE(v, 2.5);
  }

  {
    unsigned v = 1u;
    EXPECT_EQ_INT(sort_under_test(&v, 1, sizeof v, sort_cmp_uint), 0);
    EXPECT_EQ_UINT(v, 1u);
  }

  {
    int buf[] = {INT_MAX, INT_MAX, INT_MIN};
    EXPECT_EQ_INT(sort_under_test(buf, 3, sizeof *buf, sort_cmp_int), 0);
    EXPECT_TRUE(sort_sorted(buf, 3, sizeof *buf, sort_cmp_int));
  }

  {
    int v = INT_MIN;
    EXPECT_EQ_INT(sort_under_test(&v, 1, sizeof v, sort_cmp_int), 0);
    EXPECT_EQ_INT(v, INT_MIN);
  }

  {
    int v = 99;
    EXPECT_EQ_INT(sort_under_test(&v, 0, sizeof v, sort_cmp_int), 0);
    EXPECT_EQ_INT(v, 99);
  }

  {
    double x = 1.0;
    EXPECT_EQ_INT(sort_under_test(&x, 0, sizeof x, sort_cmp_double), 0);
    EXPECT_EQ_DOUBLE(x, 1.0);
  }

  {
    EXPECT_EQ_INT(sort_under_test(NULL, 0, sizeof(int), sort_cmp_int), -1);
  }

  {
    EXPECT_EQ_INT(sort_under_test(NULL, 3, sizeof(int), sort_cmp_int), -1);
  }

  {
    int buf[] = {1, 2, 3};
    EXPECT_EQ_INT(sort_under_test(buf, 3, sizeof *buf, NULL), -1);
  }

  {
    int v = 0;
    EXPECT_EQ_INT(sort_under_test(&v, 1, 0, sort_cmp_int), -1);
  }

  {
    int v = 0;
    EXPECT_EQ_INT(sort_under_test(&v, 0, sizeof v, NULL), -1);
  }

  {
    int buf[2] = {1, 0};
    EXPECT_EQ_INT(sort_under_test(buf, 2, sizeof *buf, NULL), -1);
  }

  {
    int buf[] = {3, 2, 1, 0};
    EXPECT_EQ_INT(sort_under_test(buf, 4, sizeof *buf, sort_cmp_int), 0);
    EXPECT_TRUE(sort_sorted(buf, 4, sizeof *buf, sort_cmp_int));
  }

  {
    int buf[] = {20, 10, 20, 10};
    EXPECT_EQ_INT(sort_under_test(buf, 4, sizeof *buf, sort_cmp_int), 0);
    EXPECT_TRUE(sort_sorted(buf, 4, sizeof *buf, sort_cmp_int));
  }

  {
    int buf[] = {1, 0, 1, 0, 1, 0, 1, 0, 1};
    EXPECT_EQ_INT(sort_under_test(buf, 9, sizeof *buf, sort_cmp_int), 0);
    EXPECT_TRUE(sort_sorted(buf, 9, sizeof *buf, sort_cmp_int));
  }

  {
    int buf[32];
    size_t i;

    for (i = 0; i < 32; i++)
      buf[i] = 12345;
    EXPECT_EQ_INT(sort_under_test(buf, 32, sizeof *buf, sort_cmp_int), 0);
    EXPECT_TRUE(sort_sorted(buf, 32, sizeof *buf, sort_cmp_int));
    for (i = 0; i < 32; i++)
      EXPECT_EQ_INT(buf[i], 12345);
  }

  {
    int buf[20];
    size_t i;

    for (i = 0; i < 20; i++)
      buf[i] = 0;
    EXPECT_EQ_INT(sort_under_test(buf, 20, sizeof *buf, sort_cmp_int), 0);
    EXPECT_TRUE(sort_sorted(buf, 20, sizeof *buf, sort_cmp_int));
    for (i = 0; i < 20; i++)
      EXPECT_EQ_INT(buf[i], 0);
  }

  {
    double buf[18];
    size_t i;

    for (i = 0; i < 18; i++)
      buf[i] = -0.25;
    EXPECT_EQ_INT(sort_under_test(buf, 18, sizeof *buf, sort_cmp_double), 0);
    EXPECT_TRUE(sort_sorted(buf, 18, sizeof *buf, sort_cmp_double));
    for (i = 0; i < 18; i++)
      EXPECT_EQ_DOUBLE(buf[i], -0.25);
  }

  {
    int buf[] = {7, 7, 7, 7, 7, 1, 7, 7, 7, 7, 7, 7, 7};
    EXPECT_EQ_INT(sort_under_test(buf, 13, sizeof *buf, sort_cmp_int), 0);
    EXPECT_TRUE(sort_sorted(buf, 13, sizeof *buf, sort_cmp_int));
    EXPECT_EQ_INT(buf[0], 1);
    EXPECT_EQ_INT(buf[12], 7);
  }

  {
    int buf[] = {2, 2, 2, 2, 9, 2, 2, 2, 2, 9, 2};
    EXPECT_EQ_INT(sort_under_test(buf, 11, sizeof *buf, sort_cmp_int), 0);
    EXPECT_TRUE(sort_sorted(buf, 11, sizeof *buf, sort_cmp_int));
  }

  {
    int buf[] = {
        5, 5, 5, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
    };
    EXPECT_EQ_INT(sort_under_test(buf, 18, sizeof *buf, sort_cmp_int), 0);
    EXPECT_TRUE(sort_sorted(buf, 18, sizeof *buf, sort_cmp_int));
    EXPECT_EQ_INT(buf[0], 0);
  }

  {
    int buf[] = {
        4, 4, 4, 8, 4, 4, 4, 4, 1, 4, 4, 4, 4, 4, 4, 4,
    };
    EXPECT_EQ_INT(sort_under_test(buf, 16, sizeof *buf, sort_cmp_int), 0);
    EXPECT_TRUE(sort_sorted(buf, 16, sizeof *buf, sort_cmp_int));
  }

  {
    int buf[] = {
        3, 3, 100, 3, 3, 3, -50, 3, 3, 3, 3, 3, 3, 3,
    };
    EXPECT_EQ_INT(sort_under_test(buf, 14, sizeof *buf, sort_cmp_int), 0);
    EXPECT_TRUE(sort_sorted(buf, 14, sizeof *buf, sort_cmp_int));
    EXPECT_EQ_INT(buf[0], -50);
    EXPECT_EQ_INT(buf[13], 100);
  }

  {
    int buf[] = {1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1};
    EXPECT_EQ_INT(sort_under_test(buf, 12, sizeof *buf, sort_cmp_int), 0);
    EXPECT_TRUE(sort_sorted(buf, 12, sizeof *buf, sort_cmp_int));
    EXPECT_EQ_INT(buf[0], 1);
    EXPECT_EQ_INT(buf[11], 2);
  }

  {
    sort_massrec buf[] = {
        {10, 'z'}, {10, 'z'}, {10, 'z'}, {2, 'a'},  {10, 'z'}, {10, 'z'},
        {10, 'z'}, {10, 'z'}, {2, 'b'},  {10, 'z'}, {10, 'z'},
    };
    EXPECT_EQ_INT(sort_under_test(buf, 11, sizeof *buf, sort_cmp_massrec), 0);
    EXPECT_TRUE(sort_sorted(buf, 11, sizeof *buf, sort_cmp_massrec));
  }

  {
    sort_big300 buf[2];
    size_t i;
    int k0 = 99;
    int k1 = 3;
    buf[0].k = k0;
    buf[1].k = k1;
    for (i = 0; i < sizeof buf[0].tail; i++) {
      buf[0].tail[i] = (unsigned char)((k0 + (int)i) & 0xFF);
      buf[1].tail[i] = (unsigned char)((k1 + (int)i) & 0xFF);
    }
    EXPECT_EQ_INT(sort_under_test(buf, 2, sizeof *buf, sort_cmp_big300), 0);
    EXPECT_TRUE(sort_sorted(buf, 2, sizeof *buf, sort_cmp_big300));
    EXPECT_EQ_INT(buf[0].k, k1);
    EXPECT_EQ_INT(buf[1].k, k0);
    for (i = 0; i < sizeof buf[0].tail; i++)
      EXPECT_EQ_UCHAR(buf[0].tail[i], (unsigned char)((k1 + (int)i) & 0xFF));
    for (i = 0; i < sizeof buf[1].tail; i++)
      EXPECT_EQ_UCHAR(buf[1].tail[i], (unsigned char)((k0 + (int)i) & 0xFF));
  }
}
