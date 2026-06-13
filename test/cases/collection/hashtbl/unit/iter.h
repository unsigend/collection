#include <hashtbl.h>
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
    struct hashtbl ht;
    struct hashtbl_iter it;

    EXPECT_EQ_INT(hashtbl_iter_init(NULL, &ht), -1);
    EXPECT_EQ_INT(hashtbl_iter_init(&it, NULL), -1);
  }

  {
    struct hashtbl ht;
    struct hashtbl_iter it;
    struct hashtbl_fns fns = {iter_hash_int, iter_cmp_int, NULL, NULL};

    EXPECT_EQ_INT(hashtbl_init(&ht, &fns), 0);
    EXPECT_EQ_INT(hashtbl_iter_init(&it, &ht), 0);
    EXPECT_NULL(hashtbl_iter_get(&it));
    hashtbl_iter_inc(&it);
    EXPECT_NULL(hashtbl_iter_get(&it));
    hashtbl_fini(&ht);
  }

  {
    struct hashtbl ht;
    struct hashtbl_iter it;
    struct hashtbl_fns fns = {iter_hash_int, iter_cmp_int, NULL, NULL};
    int k, v;

    EXPECT_EQ_INT(hashtbl_init(&ht, &fns), 0);
    EXPECT_EQ_INT(hashtbl_iter_init(&it, &ht), 0);
    EXPECT_NULL(hashtbl_iter_get(&it));
    k = 16;
    v = 91;
    EXPECT_EQ_INT(hashtbl_insert(&ht, &k, &v), 0);
    EXPECT_NULL(hashtbl_iter_get(&it));
    EXPECT_EQ_INT(hashtbl_iter_init(&it, &ht), 0);
    EXPECT_NOTNULL(hashtbl_iter_get(&it));
    EXPECT_EQ_PTR(hashtbl_iter_get(&it)->key, &k);
    EXPECT_EQ_PTR(hashtbl_iter_get(&it)->val, &v);
    hashtbl_fini(&ht);
  }

  {
    struct hashtbl ht;
    struct hashtbl_iter it;
    struct hashtbl_fns fns = {iter_hash_int, iter_cmp_int, NULL, NULL};
    int k, v;
    struct hashtbl_node *nd;

    EXPECT_EQ_INT(hashtbl_init(&ht, &fns), 0);
    k = 32;
    v = 91;
    EXPECT_EQ_INT(hashtbl_insert(&ht, &k, &v), 0);
    EXPECT_EQ_INT(hashtbl_iter_init(&it, &ht), 0);
    nd = hashtbl_iter_get(&it);
    EXPECT_NOTNULL(nd);
    EXPECT_EQ_PTR(nd->key, &k);
    EXPECT_EQ_PTR(nd->val, &v);
    hashtbl_iter_inc(&it);
    EXPECT_NULL(hashtbl_iter_get(&it));
    hashtbl_iter_inc(&it);
    EXPECT_NULL(hashtbl_iter_get(&it));
    hashtbl_fini(&ht);
  }

  {
    struct hashtbl ht;
    struct hashtbl_iter it;
    struct hashtbl_fns fns = {iter_hash_int, iter_cmp_int, NULL, NULL};
    int k0, k1, k2, v0, v1, v2;
    struct hashtbl_node *nd;
    int sumk;
    size_t n;

    EXPECT_EQ_INT(hashtbl_init(&ht, &fns), 0);
    k0 = 0;
    k1 = 1;
    k2 = 2;
    v0 = 10;
    v1 = 20;
    v2 = 30;
    EXPECT_EQ_INT(hashtbl_insert(&ht, &k2, &v2), 0);
    EXPECT_EQ_INT(hashtbl_insert(&ht, &k0, &v0), 0);
    EXPECT_EQ_INT(hashtbl_insert(&ht, &k1, &v1), 0);
    EXPECT_EQ_INT(hashtbl_iter_init(&it, &ht), 0);
    sumk = 0;
    n = 0;
    while ((nd = hashtbl_iter_get(&it)) != NULL) {
      sumk += *(int *)nd->key;
      n++;
      hashtbl_iter_inc(&it);
    }
    EXPECT_EQ_UINT(n, 3);
    EXPECT_EQ_INT(sumk, 3);
    hashtbl_fini(&ht);
  }

  {
    struct hashtbl ht;
    struct hashtbl_iter it;
    struct hashtbl_fns fns = {iter_hash_int, iter_cmp_int, NULL, NULL};
    int k0, k16, k32, v0, v16, v32;
    struct hashtbl_node *nd;
    int sumk;
    size_t n;

    EXPECT_EQ_INT(hashtbl_init(&ht, &fns), 0);
    k0 = 0;
    k16 = 16;
    k32 = 32;
    v0 = 100;
    v16 = 200;
    v32 = 300;
    EXPECT_EQ_INT(hashtbl_insert(&ht, &k16, &v16), 0);
    EXPECT_EQ_INT(hashtbl_insert(&ht, &k0, &v0), 0);
    EXPECT_EQ_INT(hashtbl_insert(&ht, &k32, &v32), 0);
    EXPECT_EQ_INT(hashtbl_iter_init(&it, &ht), 0);
    sumk = 0;
    n = 0;
    while ((nd = hashtbl_iter_get(&it)) != NULL) {
      sumk += *(int *)nd->key;
      n++;
      hashtbl_iter_inc(&it);
    }
    EXPECT_EQ_UINT(n, 3);
    EXPECT_EQ_INT(sumk, 48);
    hashtbl_fini(&ht);
  }

  {
    struct hashtbl ht;
    struct hashtbl_iter it;
    struct hashtbl_fns fns = {iter_hash_int, iter_cmp_int, NULL, NULL};
    int k, v;
    struct hashtbl_node *nd;

    EXPECT_EQ_INT(hashtbl_init(&ht, &fns), 0);
    k = 48;
    v = 70;
    EXPECT_EQ_INT(hashtbl_insert(&ht, &k, &v), 0);
    EXPECT_EQ_INT(hashtbl_iter_init(&it, &ht), 0);
    hashtbl_iter_inc(&it);
    EXPECT_NULL(hashtbl_iter_get(&it));
    EXPECT_EQ_INT(hashtbl_iter_init(&it, &ht), 0);
    nd = hashtbl_iter_get(&it);
    EXPECT_NOTNULL(nd);
    EXPECT_EQ_INT(*(int *)nd->key, 48);
    hashtbl_fini(&ht);
  }

  {
    struct hashtbl ht;
    struct hashtbl_iter it;
    struct hashtbl_fns fns = {iter_hash_int, iter_cmp_int, NULL, NULL};
    int k, v;

    EXPECT_EQ_INT(hashtbl_init(&ht, &fns), 0);
    k = 64;
    v = 50;
    EXPECT_EQ_INT(hashtbl_insert(&ht, &k, &v), 0);
    EXPECT_EQ_INT(hashtbl_iter_init(&it, &ht), 0);
    hashtbl_clear(&ht);
    EXPECT_EQ_INT(hashtbl_iter_init(&it, &ht), 0);
    EXPECT_NULL(hashtbl_iter_get(&it));
    hashtbl_fini(&ht);
  }

  {
    EXPECT_NULL(hashtbl_iter_get(NULL));
    hashtbl_iter_inc(NULL);
  }
}
