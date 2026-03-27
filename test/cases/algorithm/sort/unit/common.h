#ifndef TEST_SORT_COMMON_H
#define TEST_SORT_COMMON_H

#define COL_ALL_SORTS
#include <sort.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <utest.h>

typedef int (*sortfunc)(void *, size_t, size_t, int (*)(void *, void *));
extern sortfunc sort_under_test;

static inline void *sort_at(void *base, size_t idx, size_t sz)
{
  return (char *)base + idx * sz;
}

static inline int sort_sorted(void *base, size_t n, size_t sz,
                              int (*cmp)(void *, void *))
{
  size_t i;

  for (i = 1; i < n; i++) {
    if (cmp(sort_at(base, i - 1, sz), sort_at(base, i, sz)) > 0)
      return 0;
  }
  return 1;
}

static uint64_t sort_rng_u64;
static inline void sort_rng_seed(uint64_t s)
{
  sort_rng_u64 = s ? s : 1;
}

static inline uint32_t sort_rng_u32(void)
{
  sort_rng_u64 = sort_rng_u64 * 2862933555777941757ULL + 3037000493ULL;
  return (uint32_t)(sort_rng_u64 >> 32);
}

static inline int sort_cmp_int(void *a, void *b)
{
  int x = *(const int *)a;
  int y = *(const int *)b;
  return (x > y) - (x < y);
}

static inline int sort_cmp_double(void *a, void *b)
{
  double x = *(const double *)a;
  double y = *(const double *)b;
  return (x > y) - (x < y);
}

static inline int sort_cmp_uint(void *a, void *b)
{
  unsigned x = *(const unsigned *)a;
  unsigned y = *(const unsigned *)b;
  return (x > y) - (x < y);
}

static inline int sort_cmp_char(void *a, void *b)
{
  signed char x = *(const signed char *)a;
  signed char y = *(const signed char *)b;
  return (x > y) - (x < y);
}

static inline int sort_cmp_i64(void *a, void *b)
{
  int64_t x = *(const int64_t *)a;
  int64_t y = *(const int64_t *)b;
  return (x > y) - (x < y);
}

#endif
