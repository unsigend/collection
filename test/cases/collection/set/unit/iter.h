#include <set.h>
#include <stdint.h>
#include <utest.h>

static uint32_t iter_hash_int(void *k) { return (uint32_t)(*(int *)k); }

static int iter_cmp_int(void *a, void *b)
{
  int ka = *(int *)a;
  int kb = *(int *)b;
  if (ka < kb)
    return -1;
  if (ka > kb)
    return 1;
  return 0;
}

UTEST_CASE(iter)
{
  {
    struct set s;
    struct set_iter it;

    EXPECT_EQ_INT(set_iter_init(NULL, &s), -1);
    EXPECT_EQ_INT(set_iter_init(&it, NULL), -1);
  }

  {
    struct set s;
    struct set_iter it;
    struct set_fns fns = {iter_hash_int, iter_cmp_int, NULL};

    EXPECT_EQ_INT(set_init(&s, &fns), 0);
    EXPECT_EQ_INT(set_iter_init(&it, &s), 0);
    EXPECT_NULL(set_iter_get(&it));
    set_iter_inc(&it);
    EXPECT_NULL(set_iter_get(&it));
    set_fini(&s);
  }

  {
    struct set s;
    struct set_iter it;
    struct set_fns fns = {iter_hash_int, iter_cmp_int, NULL};
    int k;

    EXPECT_EQ_INT(set_init(&s, &fns), 0);
    k = 16;
    EXPECT_EQ_INT(set_insert(&s, &k), 0);
    EXPECT_EQ_INT(set_iter_init(&it, &s), 0);
    EXPECT_NOTNULL(set_iter_get(&it));
    EXPECT_EQ_PTR(set_iter_get(&it), &k);
    set_iter_inc(&it);
    EXPECT_NULL(set_iter_get(&it));
    set_fini(&s);
  }

  {
    struct set s;
    struct set_iter it;
    struct set_fns fns = {iter_hash_int, iter_cmp_int, NULL};
    int k0, k1, k2;
    void *ele;
    int sumk;
    size_t n;

    EXPECT_EQ_INT(set_init(&s, &fns), 0);
    k0 = 0;
    k1 = 1;
    k2 = 2;
    EXPECT_EQ_INT(set_insert(&s, &k2), 0);
    EXPECT_EQ_INT(set_insert(&s, &k0), 0);
    EXPECT_EQ_INT(set_insert(&s, &k1), 0);
    EXPECT_EQ_INT(set_iter_init(&it, &s), 0);
    sumk = 0;
    n = 0;
    while ((ele = set_iter_get(&it)) != NULL) {
      sumk += *(int *)ele;
      n++;
      set_iter_inc(&it);
    }
    EXPECT_EQ_UINT(n, 3);
    EXPECT_EQ_INT(sumk, 3);
    set_fini(&s);
  }

  {
    struct set s;
    struct set_iter it;
    struct set_fns fns = {iter_hash_int, iter_cmp_int, NULL};
    int k;

    EXPECT_EQ_INT(set_init(&s, &fns), 0);
    k = 48;
    EXPECT_EQ_INT(set_insert(&s, &k), 0);
    EXPECT_EQ_INT(set_iter_init(&it, &s), 0);
    set_iter_inc(&it);
    EXPECT_NULL(set_iter_get(&it));
    EXPECT_EQ_INT(set_iter_init(&it, &s), 0);
    EXPECT_NOTNULL(set_iter_get(&it));
    EXPECT_EQ_INT(*(int *)set_iter_get(&it), 48);
    set_fini(&s);
  }

  {
    struct set s;
    struct set_iter it;
    struct set_fns fns = {iter_hash_int, iter_cmp_int, NULL};
    int k;

    EXPECT_EQ_INT(set_init(&s, &fns), 0);
    k = 64;
    EXPECT_EQ_INT(set_insert(&s, &k), 0);
    EXPECT_EQ_INT(set_iter_init(&it, &s), 0);
    set_clear(&s);
    EXPECT_EQ_INT(set_iter_init(&it, &s), 0);
    EXPECT_NULL(set_iter_get(&it));
    set_fini(&s);
  }

  {
    EXPECT_NULL(set_iter_get(NULL));
    set_iter_inc(NULL);
  }
}
