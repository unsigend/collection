#include <set.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <utest.h>

static uint32_t hash_int(void *k) { return (uint32_t)(*(int *)k); }

static int cmp_int(void *a, void *b)
{
  int ka = *(int *)a;
  int kb = *(int *)b;
  if (ka < kb)
    return -1;
  if (ka > kb)
    return 1;
  return 0;
}

static uint32_t hash_str(void *k)
{
  const unsigned char *s = (const unsigned char *)k;
  uint32_t h = 5381;
  unsigned c;
  while ((c = *s++) != 0)
    h = h * 33u + c;
  return h;
}

static int cmp_str(void *a, void *b)
{
  return strcmp((const char *)a, (const char *)b);
}

static int dtor_n;

static void dtor_inc(void *p)
{
  (void)p;
  dtor_n++;
}

static void free_ele(void *p) { free(p); }

UTEST_CASE(basic)
{
  {
    struct set s;
    struct set_fns fns = {hash_int, cmp_int, NULL};

    EXPECT_EQ_INT(set_init(NULL, &fns), -1);
    EXPECT_EQ_INT(set_init(&s, NULL), -1);
    fns.hash = NULL;
    EXPECT_EQ_INT(set_init(&s, &fns), -1);
    fns.hash = hash_int;
    fns.cmp = NULL;
    EXPECT_EQ_INT(set_init(&s, &fns), -1);
  }

  {
    struct set s;
    struct set_fns fns = {hash_int, cmp_int, NULL};

    EXPECT_EQ_INT(set_init(&s, &fns), 0);
    EXPECT_TRUE(set_empty(&s));
    EXPECT_EQ_UINT(set_size(&s), 0);
    EXPECT_FALSE(set_contains(&s, (void *)1));
    EXPECT_EQ_INT(set_insert(&s, NULL), -1);
    EXPECT_EQ_INT(set_remove(&s, (void *)1), 0);
    set_fini(NULL);
    set_clear(&s);
    set_fini(&s);
  }

  {
    struct set s;
    struct set_fns fns = {hash_int, cmp_int, NULL};
    int k;

    EXPECT_EQ_INT(set_init(&s, &fns), 0);
    k = 11;
    EXPECT_EQ_INT(set_insert(&s, &k), 0);
    EXPECT_FALSE(set_empty(&s));
    EXPECT_EQ_UINT(set_size(&s), 1);
    EXPECT_TRUE(set_contains(&s, &k));
    EXPECT_EQ_INT(set_insert(&s, &k), 0);
    EXPECT_EQ_UINT(set_size(&s), 1);
    set_fini(&s);
  }

  {
    struct set s;
    struct set_fns fns = {hash_int, cmp_int, NULL};
    int k1, k2;

    EXPECT_EQ_INT(set_init(&s, &fns), 0);
    k1 = 1;
    k2 = 2;
    EXPECT_EQ_INT(set_insert(&s, &k1), 0);
    EXPECT_EQ_INT(set_insert(&s, &k2), 0);
    EXPECT_EQ_UINT(set_size(&s), 2);
    EXPECT_TRUE(set_contains(&s, &k1));
    EXPECT_TRUE(set_contains(&s, &k2));
    set_fini(&s);
  }

  {
    struct set s;
    struct set_fns fns = {hash_int, cmp_int, NULL};
    int k;

    EXPECT_EQ_INT(set_init(&s, &fns), 0);
    k = 9;
    EXPECT_EQ_INT(set_insert(&s, &k), 0);
    EXPECT_EQ_INT(set_remove(&s, &k), 0);
    EXPECT_TRUE(set_empty(&s));
    EXPECT_FALSE(set_contains(&s, &k));
    EXPECT_EQ_INT(set_remove(&s, &k), 0);
    set_fini(&s);
  }

  {
    struct set s;
    struct set_fns fns = {hash_int, cmp_int, NULL};
    int k0, k1, k2;

    EXPECT_EQ_INT(set_init(&s, &fns), 0);
    k0 = 0;
    k1 = 1;
    k2 = 2;
    EXPECT_EQ_INT(set_insert(&s, &k0), 0);
    EXPECT_EQ_INT(set_insert(&s, &k1), 0);
    EXPECT_EQ_INT(set_insert(&s, &k2), 0);
    set_clear(&s);
    EXPECT_TRUE(set_empty(&s));
    EXPECT_EQ_UINT(set_size(&s), 0);
    set_fini(&s);
  }

  {
    struct set s;
    struct set_fns fns = {hash_str, cmp_str, free_ele};
    char *ka, *kb;

    EXPECT_EQ_INT(set_init(&s, &fns), 0);
    ka = strdup("alpha");
    EXPECT_NOTNULL(ka);
    kb = strdup("beta");
    if (!kb) {
      free(ka);
    }
    EXPECT_NOTNULL(kb);
    EXPECT_EQ_INT(set_insert(&s, ka), 0);
    EXPECT_EQ_INT(set_insert(&s, kb), 0);
    EXPECT_TRUE(set_contains(&s, ka));
    EXPECT_TRUE(set_contains(&s, kb));
    set_fini(&s);
  }

  {
    struct set s;
    struct set_fns fns = {hash_int, cmp_int, free_ele};
    int idx;
    int *pe;

    EXPECT_EQ_INT(set_init(&s, &fns), 0);
    for (idx = 0; idx < 8; idx++) {
      int ins;

      pe = (int *)malloc(sizeof(int));
      EXPECT_NOTNULL(pe);
      *pe = idx;
      ins = set_insert(&s, pe);
      if (ins != 0) {
        free(pe);
      }
      EXPECT_EQ_INT(ins, 0);
    }
    EXPECT_EQ_UINT(set_size(&s), 8);
    for (idx = 0; idx < 8; idx++) {
      int key = idx;
      EXPECT_TRUE(set_contains(&s, &key));
    }
    set_fini(&s);
  }

  {
    struct set s;
    struct set_fns fns = {hash_int, cmp_int, dtor_inc};
    int k;

    dtor_n = 0;
    EXPECT_EQ_INT(set_init(&s, &fns), 0);
    k = 1;
    EXPECT_EQ_INT(set_insert(&s, &k), 0);
    set_clear(&s);
    EXPECT_EQ_INT(dtor_n, 1);
    set_fini(&s);
    EXPECT_EQ_INT(dtor_n, 1);
  }

  {
    struct set s;
    struct set_fns fns = {hash_int, cmp_int, dtor_inc};
    int k;

    dtor_n = 0;
    EXPECT_EQ_INT(set_init(&s, &fns), 0);
    k = 3;
    EXPECT_EQ_INT(set_insert(&s, &k), 0);
    EXPECT_EQ_INT(set_remove(&s, &k), 0);
    EXPECT_EQ_INT(dtor_n, 1);
    set_fini(&s);
    EXPECT_EQ_INT(dtor_n, 1);
  }

  {
    struct set s;
    struct set_fns fns = {hash_int, cmp_int, dtor_inc};
    int k;

    dtor_n = 0;
    EXPECT_EQ_INT(set_init(&s, &fns), 0);
    k = 7;
    EXPECT_EQ_INT(set_insert(&s, &k), 0);
    set_fini(&s);
    EXPECT_EQ_INT(dtor_n, 1);
  }
}
