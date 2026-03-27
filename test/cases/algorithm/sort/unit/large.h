#include "common.h"

UTEST_CASE(large)
{
  {
    size_t n = 50;
    int *buf;
    size_t i;

    buf = malloc(n * sizeof *buf);
    EXPECT_NOTNULL(buf);
    sort_rng_seed(0xC0FFEEu);
    for (i = 0; i < n; i++)
      buf[i] = (int)(sort_rng_u32() % 2000001) - 1000000;
    EXPECT_EQ_INT(sort_under_test(buf, n, sizeof *buf, sort_cmp_int), 0);
    EXPECT_TRUE(sort_sorted(buf, n, sizeof *buf, sort_cmp_int));
    free(buf);
  }

  {
    size_t n = 100;
    int *buf;
    size_t i;

    buf = malloc(n * sizeof *buf);
    EXPECT_NOTNULL(buf);
    sort_rng_seed(0xABCllu);
    for (i = 0; i < n; i++)
      buf[i] = (int)(sort_rng_u32() % 2000001) - 1000000;
    EXPECT_EQ_INT(sort_under_test(buf, n, sizeof *buf, sort_cmp_int), 0);
    EXPECT_TRUE(sort_sorted(buf, n, sizeof *buf, sort_cmp_int));
    free(buf);
  }

  {
    size_t n = 300;
    int *buf;
    size_t i;

    buf = malloc(n * sizeof *buf);
    EXPECT_NOTNULL(buf);
    sort_rng_seed(0x10002ull);
    for (i = 0; i < n; i++)
      buf[i] = (int)(sort_rng_u32() % 2000001) - 1000000;
    EXPECT_EQ_INT(sort_under_test(buf, n, sizeof *buf, sort_cmp_int), 0);
    EXPECT_TRUE(sort_sorted(buf, n, sizeof *buf, sort_cmp_int));
    free(buf);
  }

  {
    size_t n = 500;
    int *buf;
    size_t i;

    buf = malloc(n * sizeof *buf);
    EXPECT_NOTNULL(buf);
    sort_rng_seed(0xDEADBEEFCAFEull);
    for (i = 0; i < n; i++)
      buf[i] = (int)(sort_rng_u32() % 2000001) - 1000000;
    EXPECT_EQ_INT(sort_under_test(buf, n, sizeof *buf, sort_cmp_int), 0);
    EXPECT_TRUE(sort_sorted(buf, n, sizeof *buf, sort_cmp_int));
    free(buf);
  }

  {
    size_t n = 500;
    int *buf;
    size_t i;

    buf = malloc(n * sizeof *buf);
    EXPECT_NOTNULL(buf);
    for (i = 0; i < n; i++)
      buf[i] = (int)(n - 1 - i);
    EXPECT_EQ_INT(sort_under_test(buf, n, sizeof *buf, sort_cmp_int), 0);
    EXPECT_TRUE(sort_sorted(buf, n, sizeof *buf, sort_cmp_int));
    for (i = 0; i < n; i++)
      EXPECT_EQ_INT(buf[i], (int)i);
    free(buf);
  }

  {
    size_t n = 500;
    int *buf;
    size_t i;

    buf = malloc(n * sizeof *buf);
    EXPECT_NOTNULL(buf);
    for (i = 0; i < n; i++)
      buf[i] = (int)i;
    EXPECT_EQ_INT(sort_under_test(buf, n, sizeof *buf, sort_cmp_int), 0);
    EXPECT_TRUE(sort_sorted(buf, n, sizeof *buf, sort_cmp_int));
    for (i = 0; i < n; i++)
      EXPECT_EQ_INT(buf[i], (int)i);
    free(buf);
  }

  {
    size_t n = 400;
    int *buf;
    size_t i;

    buf = malloc(n * sizeof *buf);
    EXPECT_NOTNULL(buf);
    sort_rng_seed(0xD00Du);
    for (i = 0; i < n; i++)
      buf[i] = (int)(sort_rng_u32() % 7);
    EXPECT_EQ_INT(sort_under_test(buf, n, sizeof *buf, sort_cmp_int), 0);
    EXPECT_TRUE(sort_sorted(buf, n, sizeof *buf, sort_cmp_int));
    free(buf);
  }

  {
    size_t n = 256;
    unsigned *buf;
    size_t i;

    buf = malloc(n * sizeof *buf);
    EXPECT_NOTNULL(buf);
    sort_rng_seed(0x600Du);
    for (i = 0; i < n; i++)
      buf[i] = sort_rng_u32();
    EXPECT_EQ_INT(sort_under_test(buf, n, sizeof *buf, sort_cmp_uint), 0);
    EXPECT_TRUE(sort_sorted(buf, n, sizeof *buf, sort_cmp_uint));
    free(buf);
  }
}
