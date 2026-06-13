#include <set.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <utest.h>

struct intg_point {
  int x;
  int y;
};

static int intg_dtor_n;

static void intg_dtor_inc(void *p)
{
  (void)p;
  intg_dtor_n++;
}

static void intg_free_ele(void *p) { free(p); }

static uint32_t intg_hash_int(void *k) { return (uint32_t)(*(int *)k); }

static int intg_cmp_int(void *a, void *b)
{
  int ka = *(int *)a;
  int kb = *(int *)b;
  if (ka < kb)
    return -1;
  if (ka > kb)
    return 1;
  return 0;
}

static uint32_t intg_hash_point(void *k)
{
  struct intg_point *p = (struct intg_point *)k;
  return (uint32_t)(p->x * 1315423911u ^ (uint32_t)p->y);
}

static int intg_cmp_point(void *a, void *b)
{
  struct intg_point *pa = (struct intg_point *)a;
  struct intg_point *pb = (struct intg_point *)b;
  if (pa->x != pb->x)
    return pa->x < pb->x ? -1 : 1;
  if (pa->y != pb->y)
    return pa->y < pb->y ? -1 : 1;
  return 0;
}

static uint32_t intg_hash_str(void *k)
{
  const unsigned char *s = (const unsigned char *)k;
  uint32_t h = 5381;
  unsigned c;
  while ((c = *s++) != 0)
    h = h * 33u + c;
  return h;
}

static int intg_cmp_str(void *a, void *b)
{
  return strcmp((const char *)a, (const char *)b);
}

static uint32_t intg_hash_zero(void *k)
{
  (void)k;
  return 0u;
}

UTEST_CASE(integration)
{
  {
    struct set s;
    struct set_fns fns = {intg_hash_int, intg_cmp_int, NULL};
    int keys[40];
    int idx;
    int q;

    EXPECT_EQ_INT(set_init(&s, &fns), 0);
    for (idx = 0; idx < 40; idx++) {
      keys[idx] = idx * 3 + 1;
      EXPECT_EQ_INT(set_insert(&s, &keys[idx]), 0);
    }
    EXPECT_EQ_UINT(set_size(&s), 40);
    for (idx = 0; idx < 40; idx++) {
      q = idx * 3 + 1;
      EXPECT_TRUE(set_contains(&s, &q));
    }
    EXPECT_EQ_INT(set_remove(&s, &keys[0]), 0);
    EXPECT_EQ_INT(set_remove(&s, &keys[39]), 0);
    EXPECT_EQ_UINT(set_size(&s), 38);
    q = 1;
    EXPECT_FALSE(set_contains(&s, &q));
    q = 118;
    EXPECT_FALSE(set_contains(&s, &q));
    set_clear(&s);
    EXPECT_TRUE(set_empty(&s));
    keys[0] = 999;
    EXPECT_EQ_INT(set_insert(&s, &keys[0]), 0);
    EXPECT_EQ_UINT(set_size(&s), 1);
    set_fini(&s);
  }

  {
    struct set s;
    struct set_fns fns = {intg_hash_int, intg_cmp_int, intg_free_ele};
    int idx;
    int *pe;
    int q;

    EXPECT_EQ_INT(set_init(&s, &fns), 0);
    for (idx = 0; idx < 24; idx++) {
      int ins;

      pe = (int *)malloc(sizeof(int));
      EXPECT_NOTNULL(pe);
      *pe = idx + 500;
      ins = set_insert(&s, pe);
      if (ins != 0) {
        free(pe);
      }
      EXPECT_EQ_INT(ins, 0);
    }
    for (idx = 0; idx < 24; idx++) {
      q = idx + 500;
      EXPECT_TRUE(set_contains(&s, &q));
    }
    q = 502;
    EXPECT_EQ_INT(set_remove(&s, &q), 0);
    q = 515;
    EXPECT_EQ_INT(set_remove(&s, &q), 0);
    EXPECT_EQ_UINT(set_size(&s), 22);
    q = 502;
    EXPECT_FALSE(set_contains(&s, &q));
    set_fini(&s);
  }

  {
    struct set s;
    struct set_fns fns = {intg_hash_point, intg_cmp_point, NULL};
    struct intg_point pk[6];
    struct intg_point lq;
    int idx;

    EXPECT_EQ_INT(set_init(&s, &fns), 0);
    for (idx = 0; idx < 6; idx++) {
      pk[idx].x = idx;
      pk[idx].y = idx * 10;
      EXPECT_EQ_INT(set_insert(&s, &pk[idx]), 0);
    }
    lq.x = 3;
    lq.y = 30;
    EXPECT_TRUE(set_contains(&s, &lq));
    lq.x = 1;
    lq.y = 10;
    EXPECT_EQ_INT(set_remove(&s, &lq), 0);
    lq.x = 1;
    lq.y = 10;
    EXPECT_FALSE(set_contains(&s, &lq));
    lq.x = 5;
    lq.y = 50;
    EXPECT_TRUE(set_contains(&s, &lq));
    set_fini(&s);
  }

  {
    struct set s;
    struct set_fns fns = {intg_hash_str, intg_cmp_str, intg_free_ele};
    char *ks[5];
    int idx;
    char *kdup;

    EXPECT_EQ_INT(set_init(&s, &fns), 0);
    for (idx = 0; idx < 5; idx++) {
      kdup = (char *)malloc(16);
      EXPECT_NOTNULL(kdup);
      memset(kdup, 0, 16);
      kdup[0] = (char)('a' + idx);
      ks[idx] = kdup;
      EXPECT_EQ_INT(set_insert(&s, ks[idx]), 0);
    }
    kdup = strdup("c");
    EXPECT_NOTNULL(kdup);
    EXPECT_TRUE(set_contains(&s, kdup));
    free(kdup);
    kdup = strdup("e");
    EXPECT_NOTNULL(kdup);
    EXPECT_EQ_INT(set_remove(&s, kdup), 0);
    free(kdup);
    EXPECT_EQ_UINT(set_size(&s), 4);
    set_fini(&s);
  }

  {
    struct set s;
    struct set_fns fns = {intg_hash_zero, intg_cmp_int, NULL};
    int keys[20];
    int idx;
    int q;

    EXPECT_EQ_INT(set_init(&s, &fns), 0);
    for (idx = 0; idx < 20; idx++) {
      keys[idx] = idx;
      EXPECT_EQ_INT(set_insert(&s, &keys[idx]), 0);
    }
    for (idx = 0; idx < 20; idx += 2)
      EXPECT_EQ_INT(set_remove(&s, &keys[idx]), 0);
    EXPECT_EQ_UINT(set_size(&s), 10);
    for (idx = 1; idx < 20; idx += 2) {
      q = idx;
      EXPECT_TRUE(set_contains(&s, &q));
    }
    for (idx = 0; idx < 20; idx += 2) {
      q = idx;
      EXPECT_FALSE(set_contains(&s, &q));
    }
    EXPECT_EQ_INT(set_insert(&s, &keys[0]), 0);
    EXPECT_EQ_INT(set_insert(&s, &keys[2]), 0);
    EXPECT_EQ_UINT(set_size(&s), 12);
    set_fini(&s);
  }

  {
    struct set s;
    struct set_fns fns = {intg_hash_int, intg_cmp_int, intg_dtor_inc};
    int k;

    intg_dtor_n = 0;
    EXPECT_EQ_INT(set_init(&s, &fns), 0);
    k = 7;
    EXPECT_EQ_INT(set_insert(&s, &k), 0);
    k = 8;
    EXPECT_EQ_INT(set_insert(&s, &k), 0);
    set_clear(&s);
    EXPECT_EQ_INT(intg_dtor_n, 2);
    k = 1;
    EXPECT_EQ_INT(set_insert(&s, &k), 0);
    EXPECT_EQ_UINT(set_size(&s), 1);
    set_fini(&s);
    EXPECT_EQ_INT(intg_dtor_n, 3);
  }
}
