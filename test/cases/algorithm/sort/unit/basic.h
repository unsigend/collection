#include "common.h"
#include <limits.h>

typedef struct {
  int id;
  char tag;
} sort_rec;

static int sort_cmp_rec(void *a, void *b)
{
  const sort_rec *x = (const sort_rec *)a;
  const sort_rec *y = (const sort_rec *)b;
  if (x->id < y->id)
    return -1;
  if (x->id > y->id)
    return 1;
  return (int)x->tag - (int)y->tag;
}

typedef struct {
  int key;
  unsigned char body[512 - sizeof(int)];
} sort_elem512;

static int sort_cmp_elem512(void *a, void *b)
{
  int x = ((const sort_elem512 *)a)->key;
  int y = ((const sort_elem512 *)b)->key;
  return (x > y) - (x < y);
}

UTEST_CASE(basic)
{
  {
    int buf[] = {1, 2, 3, 4, 5};
    EXPECT_EQ_INT(sort_under_test(buf, 5, sizeof *buf, sort_cmp_int), 0);
    EXPECT_TRUE(sort_sorted(buf, 5, sizeof *buf, sort_cmp_int));
  }
  {
    int buf[] = {1, 2, 3, 4};
    EXPECT_EQ_INT(sort_under_test(buf, 4, sizeof *buf, sort_cmp_int), 0);
    EXPECT_TRUE(sort_sorted(buf, 4, sizeof *buf, sort_cmp_int));
  }

  {
    int buf[] = {9, 4, 7, 1};
    EXPECT_EQ_INT(sort_under_test(buf, 4, sizeof *buf, sort_cmp_int), 0);
    EXPECT_TRUE(sort_sorted(buf, 4, sizeof *buf, sort_cmp_int));
  }

  {
    int buf[] = {2, 2, 2, 2};
    EXPECT_EQ_INT(sort_under_test(buf, 4, sizeof *buf, sort_cmp_int), 0);
    EXPECT_TRUE(sort_sorted(buf, 4, sizeof *buf, sort_cmp_int));
  }

  {
    int buf[] = {0, -3, 100, -3};
    EXPECT_EQ_INT(sort_under_test(buf, 4, sizeof *buf, sort_cmp_int), 0);
    EXPECT_TRUE(sort_sorted(buf, 4, sizeof *buf, sort_cmp_int));
  }

  {
    int buf[] = {7};
    EXPECT_EQ_INT(sort_under_test(buf, 1, sizeof *buf, sort_cmp_int), 0);
    EXPECT_EQ_INT(buf[0], 7);
  }

  {
    int buf[] = {2, 1};
    EXPECT_EQ_INT(sort_under_test(buf, 2, sizeof *buf, sort_cmp_int), 0);
    EXPECT_EQ_INT(buf[0], 1);
    EXPECT_EQ_INT(buf[1], 2);
  }

  {
    int buf[] = {INT_MAX, INT_MIN, 0, -1, INT_MAX, INT_MIN};
    EXPECT_EQ_INT(sort_under_test(buf, 6, sizeof *buf, sort_cmp_int), 0);
    EXPECT_TRUE(sort_sorted(buf, 6, sizeof *buf, sort_cmp_int));
  }

  {
    unsigned buf[] = {9u, 1u, 4u, 1u, 4u};
    EXPECT_EQ_INT(sort_under_test(buf, 5, sizeof *buf, sort_cmp_uint), 0);
    EXPECT_TRUE(sort_sorted(buf, 5, sizeof *buf, sort_cmp_uint));
  }

  {
    signed char buf[] = {'z', 'a', 'm', 'a'};
    EXPECT_EQ_INT(sort_under_test(buf, 4, sizeof *buf, sort_cmp_char), 0);
    EXPECT_TRUE(sort_sorted(buf, 4, sizeof *buf, sort_cmp_char));
  }

  {
    int64_t buf[] = {((int64_t)1) << 40, -(((int64_t)1) << 41), 0,
                     ((int64_t)1) << 40};
    EXPECT_EQ_INT(sort_under_test(buf, 4, sizeof *buf, sort_cmp_i64), 0);
    EXPECT_TRUE(sort_sorted(buf, 4, sizeof *buf, sort_cmp_i64));
  }

  {
    int buf[] = {8, 7, 6, 5, 4, 3, 2, 1};
    EXPECT_EQ_INT(sort_under_test(buf, 8, sizeof *buf, sort_cmp_int), 0);
    EXPECT_TRUE(sort_sorted(buf, 8, sizeof *buf, sort_cmp_int));
  }

  {
    int buf[] = {3, 1, 2, 4, 5};
    EXPECT_EQ_INT(sort_under_test(buf, 5, sizeof *buf, sort_cmp_int), 0);
    EXPECT_TRUE(sort_sorted(buf, 5, sizeof *buf, sort_cmp_int));
  }

  {
    int buf[] = {2, 5, 1, 4, 3};
    EXPECT_EQ_INT(sort_under_test(buf, 5, sizeof *buf, sort_cmp_int), 0);
    EXPECT_TRUE(sort_sorted(buf, 5, sizeof *buf, sort_cmp_int));
  }

  {
    double buf[] = {1.25, 0.5, 2.0, 0.5};
    EXPECT_EQ_INT(sort_under_test(buf, 4, sizeof *buf, sort_cmp_double), 0);
    EXPECT_TRUE(sort_sorted(buf, 4, sizeof *buf, sort_cmp_double));
  }

  {
    double buf[] = {-1e9, 0.0, -0.0, 1e-30, 1e9};
    EXPECT_EQ_INT(sort_under_test(buf, 5, sizeof *buf, sort_cmp_double), 0);
    EXPECT_TRUE(sort_sorted(buf, 5, sizeof *buf, sort_cmp_double));
  }

  {
    sort_rec buf[] = {{2, 'b'}, {1, 'a'}, {2, 'a'}};
    EXPECT_EQ_INT(sort_under_test(buf, 3, sizeof *buf, sort_cmp_rec), 0);
    EXPECT_TRUE(sort_sorted(buf, 3, sizeof *buf, sort_cmp_rec));
  }

  {
    sort_rec buf[] = {{100, 'z'}, {100, 'a'}, {50, 'm'}, {50, 'n'}, {200, 'a'}};
    EXPECT_EQ_INT(sort_under_test(buf, 5, sizeof *buf, sort_cmp_rec), 0);
    EXPECT_TRUE(sort_sorted(buf, 5, sizeof *buf, sort_cmp_rec));
  }

  {
    sort_elem512 buf[3];
    size_t i;
    int k;

    k = 30;
    for (i = 0; i < sizeof buf[0].body; i++)
      buf[0].body[i] = (unsigned char)((k + (int)i) & 0xFF);
    buf[0].key = k;
    k = 10;
    for (i = 0; i < sizeof buf[1].body; i++)
      buf[1].body[i] = (unsigned char)((k + (int)i) & 0xFF);
    buf[1].key = k;
    k = 20;
    for (i = 0; i < sizeof buf[2].body; i++)
      buf[2].body[i] = (unsigned char)((k + (int)i) & 0xFF);
    buf[2].key = k;
    EXPECT_EQ_INT(sort_under_test(buf, 3, sizeof *buf, sort_cmp_elem512), 0);
    EXPECT_TRUE(sort_sorted(buf, 3, sizeof *buf, sort_cmp_elem512));
    for (i = 0; i < 3; i++) {
      size_t j;
      k = buf[i].key;
      for (j = 0; j < sizeof buf[i].body; j++)
        EXPECT_EQ_UCHAR(buf[i].body[j], (unsigned char)((k + (int)j) & 0xFF));
    }
  }

  {
    sort_elem512 buf[8];
    size_t i;
    int keys[] = {5, 2, 8, 1, 9, 3, 7, 4};
    for (i = 0; i < 8; i++) {
      size_t j;
      int kk = keys[i];
      buf[i].key = kk;
      for (j = 0; j < sizeof buf[i].body; j++)
        buf[i].body[j] = (unsigned char)((kk + (int)j) & 0xFF);
    }
    EXPECT_EQ_INT(sort_under_test(buf, 8, sizeof *buf, sort_cmp_elem512), 0);
    EXPECT_TRUE(sort_sorted(buf, 8, sizeof *buf, sort_cmp_elem512));
    for (i = 0; i < 8; i++) {
      size_t j;
      int kk = buf[i].key;
      for (j = 0; j < sizeof buf[i].body; j++)
        EXPECT_EQ_UCHAR(buf[i].body[j], (unsigned char)((kk + (int)j) & 0xFF));
    }
  }

  {
    int buf[] = {0, 1, 2, 9, 3, 4};
    EXPECT_EQ_INT(sort_under_test(buf, 6, sizeof *buf, sort_cmp_int), 0);
    EXPECT_TRUE(sort_sorted(buf, 6, sizeof *buf, sort_cmp_int));
  }

  {
    int buf[] = {5, 4, 3, 10, 11, 12};
    EXPECT_EQ_INT(sort_under_test(buf, 6, sizeof *buf, sort_cmp_int), 0);
    EXPECT_TRUE(sort_sorted(buf, 6, sizeof *buf, sort_cmp_int));
  }

  {
    int buf[] = {0, 1, 2, 3, 4, 100, 6, 5, 7, 8, 9};
    EXPECT_EQ_INT(sort_under_test(buf, 11, sizeof *buf, sort_cmp_int), 0);
    EXPECT_TRUE(sort_sorted(buf, 11, sizeof *buf, sort_cmp_int));
  }

  {
    int buf[] = {9, 8, 7, 6, 5, 0, 1, 2, 3, 4};
    EXPECT_EQ_INT(sort_under_test(buf, 10, sizeof *buf, sort_cmp_int), 0);
    EXPECT_TRUE(sort_sorted(buf, 10, sizeof *buf, sort_cmp_int));
  }

  {
    int buf[] = {20, 21, 22, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    EXPECT_EQ_INT(sort_under_test(buf, 13, sizeof *buf, sort_cmp_int), 0);
    EXPECT_TRUE(sort_sorted(buf, 13, sizeof *buf, sort_cmp_int));
  }

  {
    int buf[] = {0, 1, 2, 3, 4, 5, 6, 7, 9, 8};
    EXPECT_EQ_INT(sort_under_test(buf, 10, sizeof *buf, sort_cmp_int), 0);
    EXPECT_TRUE(sort_sorted(buf, 10, sizeof *buf, sort_cmp_int));
  }

  {
    int buf[] = {0, 2, 1, 3, 4, 5, 7, 6, 8};
    EXPECT_EQ_INT(sort_under_test(buf, 9, sizeof *buf, sort_cmp_int), 0);
    EXPECT_TRUE(sort_sorted(buf, 9, sizeof *buf, sort_cmp_int));
  }

  {
    int buf[] = {3, 3, 1, 2, 4, 4, 5, 5};
    EXPECT_EQ_INT(sort_under_test(buf, 8, sizeof *buf, sort_cmp_int), 0);
    EXPECT_TRUE(sort_sorted(buf, 8, sizeof *buf, sort_cmp_int));
  }

  {
    int buf[] = {4, 2, 6, 1, 5, 3, 0};
    EXPECT_EQ_INT(sort_under_test(buf, 7, sizeof *buf, sort_cmp_int), 0);
    EXPECT_TRUE(sort_sorted(buf, 7, sizeof *buf, sort_cmp_int));
  }

  {
    int buf[] = {1, 3, 2, 5, 4, 7, 6, 9, 8, 10, 11};
    EXPECT_EQ_INT(sort_under_test(buf, 11, sizeof *buf, sort_cmp_int), 0);
    EXPECT_TRUE(sort_sorted(buf, 11, sizeof *buf, sort_cmp_int));
  }

  {
    int buf[] = {0, 10, 20, 30, 40, 5, 15, 25, 35, 45, 50, 55};
    EXPECT_EQ_INT(sort_under_test(buf, 12, sizeof *buf, sort_cmp_int), 0);
    EXPECT_TRUE(sort_sorted(buf, 12, sizeof *buf, sort_cmp_int));
  }
}
