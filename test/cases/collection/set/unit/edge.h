#include <set.h>
#include <stdint.h>
#include <stdlib.h>
#include <utest.h>

static uint32_t edg_hash_int(void *k)
{
  return (uint32_t)(*(int *)k);
}

static int edg_cmp_int(void *a, void *b)
{
  int ka = *(int *)a;
  int kb = *(int *)b;
  if (ka < kb)
    return -1;
  if (ka > kb)
    return 1;
  return 0;
}

static uint32_t edg_hash_zero(void *k)
{
  (void)k;
  return 0u;
}

static int edg_dtor_n;

static void edg_dtor_inc(void *p)
{
  (void)p;
  edg_dtor_n++;
}

static void edg_free_ele(void *p)
{
  free(p);
}

UTEST_CASE(edge)
{
  {
    int k;

    k = 0;
    EXPECT_FALSE(set_contains(NULL, &k));
    EXPECT_EQ_INT(set_insert(NULL, &k), -1);
    EXPECT_EQ_INT(set_remove(NULL, &k), -1);
    set_clear(NULL);
  }

  {
    struct set s;
    struct set_fns fns = {edg_hash_int, edg_cmp_int, NULL};
    int k;

    EXPECT_EQ_INT(set_init(&s, &fns), 0);
    EXPECT_FALSE(set_contains(&s, NULL));
    EXPECT_EQ_INT(set_insert(&s, NULL), -1);
    EXPECT_EQ_INT(set_remove(&s, NULL), -1);
    k = 1;
    EXPECT_FALSE(set_contains(&s, &k));
    EXPECT_EQ_INT(set_remove(&s, &k), 0);
    set_fini(&s);
  }

  {
    struct set s;
    struct set_fns fns = {edg_hash_int, edg_cmp_int, NULL};

    EXPECT_EQ_INT(set_init(&s, &fns), 0);
    set_clear(&s);
    set_clear(&s);
    EXPECT_TRUE(set_empty(&s));
    set_fini(&s);
  }

  {
    struct set s;
    struct set_fns fns = {edg_hash_int, edg_cmp_int, NULL};
    int k1, k2;

    EXPECT_EQ_INT(set_init(&s, &fns), 0);
    k1 = 42;
    k2 = 42;
    EXPECT_EQ_INT(set_insert(&s, &k1), 0);
    EXPECT_EQ_INT(set_insert(&s, &k2), 0);
    EXPECT_EQ_UINT(set_size(&s), 1);
    EXPECT_TRUE(set_contains(&s, &k1));
    EXPECT_TRUE(set_contains(&s, &k2));
    set_fini(&s);
  }

  {
    struct set s;
    struct set_fns fns = {edg_hash_zero, edg_cmp_int, NULL};
    int keys[12];
    int idx;
    int q;

    EXPECT_EQ_INT(set_init(&s, &fns), 0);
    for (idx = 0; idx < 12; idx++) {
      keys[idx] = idx;
      EXPECT_EQ_INT(set_insert(&s, &keys[idx]), 0);
    }
    EXPECT_EQ_UINT(set_size(&s), 12);
    for (idx = 0; idx < 12; idx++) {
      q = idx;
      EXPECT_TRUE(set_contains(&s, &q));
    }
    EXPECT_EQ_INT(set_remove(&s, &keys[6]), 0);
    EXPECT_EQ_UINT(set_size(&s), 11);
    q = 6;
    EXPECT_FALSE(set_contains(&s, &q));
    EXPECT_EQ_INT(set_remove(&s, &keys[0]), 0);
    EXPECT_EQ_INT(set_remove(&s, &keys[11]), 0);
    EXPECT_EQ_UINT(set_size(&s), 9);
    set_fini(&s);
  }

  {
    struct set s;
    struct set_fns fns = {edg_hash_int, edg_cmp_int, edg_free_ele};
    int *pe;
    int klookup;

    EXPECT_EQ_INT(set_init(&s, &fns), 0);
    pe = (int *)malloc(sizeof(int));
    EXPECT_NOTNULL(pe);
    *pe = 100;
    EXPECT_EQ_INT(set_insert(&s, pe), 0);
    klookup = 100;
    EXPECT_TRUE(set_contains(&s, &klookup));
    EXPECT_EQ_INT(set_remove(&s, &klookup), 0);
    EXPECT_FALSE(set_contains(&s, &klookup));
    set_fini(&s);
  }

  {
    struct set s;
    struct set_fns fns = {edg_hash_int, edg_cmp_int, NULL};
    int k;

    EXPECT_EQ_INT(set_init(&s, &fns), 0);
    k = 5;
    EXPECT_EQ_INT(set_insert(&s, &k), 0);
    EXPECT_EQ_INT(set_remove(&s, &k), 0);
    EXPECT_FALSE(set_contains(&s, &k));
    EXPECT_EQ_INT(set_insert(&s, &k), 0);
    EXPECT_TRUE(set_contains(&s, &k));
    set_fini(&s);
  }

  {
    struct set s;
    struct set_fns fns = {edg_hash_int, edg_cmp_int, edg_dtor_inc};
    int k;

    edg_dtor_n = 0;
    EXPECT_EQ_INT(set_init(&s, &fns), 0);
    k = 8;
    EXPECT_EQ_INT(set_insert(&s, &k), 0);
    EXPECT_EQ_INT(set_remove(&s, &k), 0);
    EXPECT_EQ_INT(edg_dtor_n, 1);
    set_fini(&s);
    EXPECT_EQ_INT(edg_dtor_n, 1);
  }

  {
    struct set s;
    struct set_fns fns = {edg_hash_int, edg_cmp_int, NULL};
    int keys[64];
    int idx;

    EXPECT_EQ_INT(set_init(&s, &fns), 0);
    for (idx = 0; idx < 64; idx++) {
      keys[idx] = idx + 1000;
      EXPECT_EQ_INT(set_insert(&s, &keys[idx]), 0);
    }
    EXPECT_EQ_UINT(set_size(&s), 64);
    set_fini(&s);
  }

  {
    struct set s;
    struct set_fns fns = {edg_hash_int, edg_cmp_int, NULL};
    int k1, k2, km;

    EXPECT_EQ_INT(set_init(&s, &fns), 0);
    k1 = 11;
    k2 = 22;
    EXPECT_EQ_INT(set_insert(&s, &k1), 0);
    EXPECT_EQ_INT(set_insert(&s, &k2), 0);
    km = 9999;
    EXPECT_FALSE(set_contains(&s, &km));
    EXPECT_EQ_INT(set_remove(&s, &km), 0);
    EXPECT_EQ_UINT(set_size(&s), 2);
    set_fini(&s);
  }
}
