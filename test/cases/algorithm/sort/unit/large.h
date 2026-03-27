#include "common.h"

UTEST_CASE(large)
{
  {
    size_t n = 50;
    int buf[50];
    size_t i;

    sort_rng_seed(0xC0FFEEu);
    for (i = 0; i < n; i++)
      buf[i] = (int)(sort_rng_u32() % 2000001) - 1000000;
    EXPECT_EQ_INT(sort_under_test(buf, n, sizeof *buf, sort_cmp_int), 0);
    EXPECT_TRUE(sort_sorted(buf, n, sizeof *buf, sort_cmp_int));
  }

  {
    size_t n = 100;
    int buf[100];
    size_t i;

    sort_rng_seed(0xABCllu);
    for (i = 0; i < n; i++)
      buf[i] = (int)(sort_rng_u32() % 2000001) - 1000000;
    EXPECT_EQ_INT(sort_under_test(buf, n, sizeof *buf, sort_cmp_int), 0);
    EXPECT_TRUE(sort_sorted(buf, n, sizeof *buf, sort_cmp_int));
  }

  {
    size_t n = 300;
    int buf[300];
    size_t i;

    sort_rng_seed(0x10002ull);
    for (i = 0; i < n; i++)
      buf[i] = (int)(sort_rng_u32() % 2000001) - 1000000;
    EXPECT_EQ_INT(sort_under_test(buf, n, sizeof *buf, sort_cmp_int), 0);
    EXPECT_TRUE(sort_sorted(buf, n, sizeof *buf, sort_cmp_int));
  }

  {
    size_t n = 500;
    int buf[500];
    size_t i;

    sort_rng_seed(0xDEADBEEFCAFEull);
    for (i = 0; i < n; i++)
      buf[i] = (int)(sort_rng_u32() % 2000001) - 1000000;
    EXPECT_EQ_INT(sort_under_test(buf, n, sizeof *buf, sort_cmp_int), 0);
    EXPECT_TRUE(sort_sorted(buf, n, sizeof *buf, sort_cmp_int));
  }

  {
    size_t n = 500;
    int buf[500];
    size_t i;

    for (i = 0; i < n; i++)
      buf[i] = (int)(n - 1 - i);
    EXPECT_EQ_INT(sort_under_test(buf, n, sizeof *buf, sort_cmp_int), 0);
    EXPECT_TRUE(sort_sorted(buf, n, sizeof *buf, sort_cmp_int));
    for (i = 0; i < n; i++)
      EXPECT_EQ_INT(buf[i], (int)i);
  }

  {
    size_t n = 500;
    int buf[500];
    size_t i;

    for (i = 0; i < n; i++)
      buf[i] = (int)i;
    EXPECT_EQ_INT(sort_under_test(buf, n, sizeof *buf, sort_cmp_int), 0);
    EXPECT_TRUE(sort_sorted(buf, n, sizeof *buf, sort_cmp_int));
    for (i = 0; i < n; i++)
      EXPECT_EQ_INT(buf[i], (int)i);
  }

  {
    size_t n = 400;
    int buf[400];
    size_t i;

    sort_rng_seed(0xD00Du);
    for (i = 0; i < n; i++)
      buf[i] = (int)(sort_rng_u32() % 7);
    EXPECT_EQ_INT(sort_under_test(buf, n, sizeof *buf, sort_cmp_int), 0);
    EXPECT_TRUE(sort_sorted(buf, n, sizeof *buf, sort_cmp_int));
  }

  {
    size_t n = 256;
    unsigned buf[256];
    size_t i;

    sort_rng_seed(0x600Du);
    for (i = 0; i < n; i++)
      buf[i] = sort_rng_u32();
    EXPECT_EQ_INT(sort_under_test(buf, n, sizeof *buf, sort_cmp_uint), 0);
    EXPECT_TRUE(sort_sorted(buf, n, sizeof *buf, sort_cmp_uint));
  }

  {
    size_t n = 500;
    int buf[500];
    size_t i;

    for (i = 0; i < 250; i++)
      buf[i] = (int)(250 + i);
    for (i = 0; i < 250; i++)
      buf[250 + i] = (int)i;
    EXPECT_EQ_INT(sort_under_test(buf, n, sizeof *buf, sort_cmp_int), 0);
    EXPECT_TRUE(sort_sorted(buf, n, sizeof *buf, sort_cmp_int));
    for (i = 0; i < n; i++)
      EXPECT_EQ_INT(buf[i], (int)i);
  }

  {
    size_t n = 500;
    int buf[500];
    size_t i;

    for (i = 0; i < n; i++)
      buf[i] = (int)i;
    buf[240] = 242;
    buf[241] = 241;
    buf[242] = 240;
    EXPECT_EQ_INT(sort_under_test(buf, n, sizeof *buf, sort_cmp_int), 0);
    EXPECT_TRUE(sort_sorted(buf, n, sizeof *buf, sort_cmp_int));
    for (i = 0; i < n; i++)
      EXPECT_EQ_INT(buf[i], (int)i);
  }

  {
    size_t n = 500;
    int buf[500];
    size_t i;
    size_t j;
    int t;

    for (i = 0; i < 380; i++)
      buf[i] = (int)i;
    for (i = 0; i < 120; i++)
      buf[380 + i] = (int)(380 + i);
    sort_rng_seed(0x51CED00Du);
    for (i = 119; i > 0; i--) {
      j = (size_t)(sort_rng_u32() % (uint32_t)(i + 1));
      t = buf[380 + i];
      buf[380 + i] = buf[380 + j];
      buf[380 + j] = t;
    }
    EXPECT_EQ_INT(sort_under_test(buf, n, sizeof *buf, sort_cmp_int), 0);
    EXPECT_TRUE(sort_sorted(buf, n, sizeof *buf, sort_cmp_int));
    for (i = 0; i < n; i++)
      EXPECT_EQ_INT(buf[i], (int)i);
  }

  {
    size_t n = 499;
    int buf[499];
    size_t i;

    for (i = 0; i < 249; i++)
      buf[i] = (int)(250 + i);
    for (i = 0; i < 250; i++)
      buf[249 + i] = (int)i;
    EXPECT_EQ_INT(sort_under_test(buf, n, sizeof *buf, sort_cmp_int), 0);
    EXPECT_TRUE(sort_sorted(buf, n, sizeof *buf, sort_cmp_int));
    for (i = 0; i < n; i++)
      EXPECT_EQ_INT(buf[i], (int)i);
  }

  {
    size_t n = 501;
    int buf[501];
    size_t i;

    for (i = 0; i < 251; i++)
      buf[i] = (int)(250 + i);
    for (i = 0; i < 250; i++)
      buf[251 + i] = (int)i;
    EXPECT_EQ_INT(sort_under_test(buf, n, sizeof *buf, sort_cmp_int), 0);
    EXPECT_TRUE(sort_sorted(buf, n, sizeof *buf, sort_cmp_int));
    for (i = 0; i < n; i++)
      EXPECT_EQ_INT(buf[i], (int)i);
  }

  {
    size_t n = 63;
    int buf[63];
    size_t i;

    for (i = 0; i < 40; i++)
      buf[i] = (int)i;
    for (i = 40; i < n; i++)
      buf[i] = (int)(90 - (i - 40));
    EXPECT_EQ_INT(sort_under_test(buf, n, sizeof *buf, sort_cmp_int), 0);
    EXPECT_TRUE(sort_sorted(buf, n, sizeof *buf, sort_cmp_int));
  }

  {
    size_t n = 64;
    int buf[64];
    size_t i;

    for (i = 0; i < 32; i++)
      buf[i] = (int)(32 + i);
    for (i = 0; i < 32; i++)
      buf[32 + i] = (int)i;
    EXPECT_EQ_INT(sort_under_test(buf, n, sizeof *buf, sort_cmp_int), 0);
    EXPECT_TRUE(sort_sorted(buf, n, sizeof *buf, sort_cmp_int));
    for (i = 0; i < n; i++)
      EXPECT_EQ_INT(buf[i], (int)i);
  }

  {
    size_t n = 502;
    int buf[502];
    size_t i;

    for (i = 0; i < n; i++)
      buf[i] = (int)i;
    buf[0] = 400;
    buf[400] = 0;
    EXPECT_EQ_INT(sort_under_test(buf, n, sizeof *buf, sort_cmp_int), 0);
    EXPECT_TRUE(sort_sorted(buf, n, sizeof *buf, sort_cmp_int));
    for (i = 0; i < n; i++)
      EXPECT_EQ_INT(buf[i], (int)i);
  }

  {
    size_t n = 128;
    int buf[128];
    size_t i;
    int s;

    for (i = 0; i < n; i++)
      buf[i] = (int)i;
    for (i = 0; i + 1 < n; i += 2) {
      s = buf[i];
      buf[i] = buf[i + 1];
      buf[i + 1] = s;
    }
    EXPECT_EQ_INT(sort_under_test(buf, n, sizeof *buf, sort_cmp_int), 0);
    EXPECT_TRUE(sort_sorted(buf, n, sizeof *buf, sort_cmp_int));
    for (i = 0; i < n; i++)
      EXPECT_EQ_INT(buf[i], (int)i);
  }

  {
    size_t n = 127;
    int buf[127];
    size_t i;
    int s;

    for (i = 0; i < n; i++)
      buf[i] = (int)i;
    for (i = 0; i + 1 < n; i += 2) {
      s = buf[i];
      buf[i] = buf[i + 1];
      buf[i + 1] = s;
    }
    EXPECT_EQ_INT(sort_under_test(buf, n, sizeof *buf, sort_cmp_int), 0);
    EXPECT_TRUE(sort_sorted(buf, n, sizeof *buf, sort_cmp_int));
    for (i = 0; i < n; i++)
      EXPECT_EQ_INT(buf[i], (int)i);
  }

  {
    size_t n = 500;
    int buf[500];
    size_t i;
    int s;

    for (i = 0; i < n; i++)
      buf[i] = (int)i;
    for (i = 0; i + 1 < n; i += 2) {
      s = buf[i];
      buf[i] = buf[i + 1];
      buf[i + 1] = s;
    }
    EXPECT_EQ_INT(sort_under_test(buf, n, sizeof *buf, sort_cmp_int), 0);
    EXPECT_TRUE(sort_sorted(buf, n, sizeof *buf, sort_cmp_int));
    for (i = 0; i < n; i++)
      EXPECT_EQ_INT(buf[i], (int)i);
  }

  {
    size_t n = 48;
    int buf[48];
    size_t i;

    sort_rng_seed(0xF1EEu);
    for (i = 0; i < n / 2; i++)
      buf[i] = (int)(sort_rng_u32() % 50000);
    for (; i < n; i++)
      buf[i] = (int)(60000 + sort_rng_u32() % 50000);
    EXPECT_EQ_INT(sort_under_test(buf, n, sizeof *buf, sort_cmp_int), 0);
    EXPECT_TRUE(sort_sorted(buf, n, sizeof *buf, sort_cmp_int));
  }
}
