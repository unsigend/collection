#include <hashtbl.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
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

static int edg_key_dtor_n;
static int edg_val_dtor_n;

static void edg_key_dtor_inc(void *p)
{
  (void)p;
  edg_key_dtor_n++;
}

static void edg_val_dtor_inc(void *p)
{
  (void)p;
  edg_val_dtor_n++;
}

static void edg_free_key(void *p)
{
  free(p);
}

UTEST_CASE(edge)
{
  {
    int k, v;

    k = 0;
    v = 0;
    EXPECT_EQ_DOUBLE((double)hashtbl_loadfactor(NULL), 0.0);
    EXPECT_NULL(hashtbl_find(NULL, &k));
    EXPECT_NULL(hashtbl_findnode(NULL, &k));
    EXPECT_EQ_INT(hashtbl_remove(NULL, &k, NULL), -1);
    EXPECT_EQ_INT(hashtbl_insert(NULL, &k, &v), -1);
    EXPECT_EQ_INT(hashtbl_update(NULL, &k, &v, NULL), -1);
    EXPECT_EQ_INT(hashtbl_setthreshold(NULL, 0.5f, NULL), -1);
    hashtbl_clear(NULL);
  }

  {
    struct hashtbl ht;
    struct hashtbl_fns fns = {edg_hash_int, edg_cmp_int, NULL, NULL};
    int k, v;
    void *p;

    EXPECT_EQ_INT(hashtbl_init(&ht, &fns), 0);
    k = 1;
    v = 1;
    EXPECT_NULL(hashtbl_find(&ht, &k));
    EXPECT_NULL(hashtbl_findnode(&ht, &k));
    EXPECT_EQ_INT(hashtbl_remove(&ht, &k, NULL), -1);
    EXPECT_NULL(hashtbl_find(&ht, NULL));
    EXPECT_NULL(hashtbl_findnode(&ht, NULL));
    EXPECT_EQ_INT(hashtbl_remove(&ht, NULL, NULL), -1);
    EXPECT_EQ_INT(hashtbl_update(&ht, NULL, &v, NULL), -1);
    EXPECT_EQ_INT(hashtbl_update(&ht, &k, NULL, NULL), -1);
    p = &v;
    EXPECT_EQ_INT(hashtbl_update(&ht, &k, p, &p), -1);
    EXPECT_EQ_INT(hashtbl_insert(&ht, &k, &v), 0);
    EXPECT_EQ_INT(hashtbl_update(&ht, &k, NULL, NULL), -1);
    hashtbl_fini(&ht);
  }

  {
    struct hashtbl ht;
    struct hashtbl_fns fns = {edg_hash_int, edg_cmp_int, NULL, NULL};

    EXPECT_EQ_INT(hashtbl_init(&ht, &fns), 0);
    hashtbl_clear(&ht);
    hashtbl_clear(&ht);
    EXPECT_TRUE(hashtbl_empty(&ht));
    hashtbl_fini(&ht);
  }

  {
    struct hashtbl ht;
    struct hashtbl_fns fns = {edg_hash_int, edg_cmp_int, NULL, NULL};
    int k, v;
    void *oldv;

    EXPECT_EQ_INT(hashtbl_init(&ht, &fns), 0);
    k = 9;
    v = 90;
    EXPECT_EQ_INT(hashtbl_insert(&ht, &k, &v), 0);
    EXPECT_EQ_INT(hashtbl_update(&ht, &k, &v, &oldv), 0);
    EXPECT_NOTNULL(oldv);
    EXPECT_EQ_PTR(oldv, &v);
    EXPECT_EQ_INT(hashtbl_update(&ht, &k, &v, NULL), 0);
    hashtbl_fini(&ht);
  }

  {
    struct hashtbl ht;
    struct hashtbl_fns fns = {edg_hash_int, edg_cmp_int, NULL, edg_val_dtor_inc};
    int k, v0, v1;
    void *oldv;

    edg_val_dtor_n = 0;
    EXPECT_EQ_INT(hashtbl_init(&ht, &fns), 0);
    k = 2;
    v0 = 20;
    v1 = 21;
    EXPECT_EQ_INT(hashtbl_insert(&ht, &k, &v0), 0);
    EXPECT_EQ_INT(hashtbl_update(&ht, &k, &v1, &oldv), 0);
    EXPECT_NOTNULL(oldv);
    EXPECT_EQ_PTR(oldv, &v0);
    EXPECT_EQ_INT(edg_val_dtor_n, 0);
    hashtbl_fini(&ht);
    EXPECT_EQ_INT(edg_val_dtor_n, 1);
  }

  {
    struct hashtbl ht;
    struct hashtbl_fns fns = {edg_hash_int, edg_cmp_int, NULL, NULL};
    int k;
    void *oldv;

    EXPECT_EQ_INT(hashtbl_init(&ht, &fns), 0);
    k = 3;
    EXPECT_EQ_INT(hashtbl_insert(&ht, &k, NULL), 0);
    EXPECT_NOTNULL(hashtbl_findnode(&ht, &k));
    EXPECT_NULL(hashtbl_find(&ht, &k));
    oldv = (void *)1;
    EXPECT_EQ_INT(hashtbl_update(&ht, &k, &k, &oldv), 0);
    EXPECT_NULL(oldv);
    EXPECT_EQ_PTR(hashtbl_find(&ht, &k), &k);
    hashtbl_fini(&ht);
  }

  {
    struct hashtbl ht;
    struct hashtbl_fns fns = {edg_hash_int, edg_cmp_int, NULL, NULL};
    int k;
    void *outv;

    EXPECT_EQ_INT(hashtbl_init(&ht, &fns), 0);
    k = 4;
    EXPECT_EQ_INT(hashtbl_insert(&ht, &k, NULL), 0);
    outv = (void *)1;
    EXPECT_EQ_INT(hashtbl_remove(&ht, &k, &outv), 0);
    EXPECT_NULL(outv);
    hashtbl_fini(&ht);
  }

  {
    struct hashtbl ht;
    struct hashtbl_fns fns = {edg_hash_zero, edg_cmp_int, NULL, NULL};
    int keys[12];
    int vals[12];
    int idx;
    int q;

    EXPECT_EQ_INT(hashtbl_init(&ht, &fns), 0);
    for (idx = 0; idx < 12; idx++) {
      keys[idx] = idx;
      vals[idx] = idx * 100;
      EXPECT_EQ_INT(hashtbl_insert(&ht, &keys[idx], &vals[idx]), 0);
      EXPECT_LE_DOUBLE((double)hashtbl_loadfactor(&ht), (double)hashtbl_threshold(&ht));
    }
    EXPECT_EQ_UINT(hashtbl_size(&ht), 12);
    for (idx = 0; idx < 12; idx++) {
      q = idx;
      {
        void *fv = hashtbl_find(&ht, &q);

        EXPECT_NOTNULL(fv);
        EXPECT_EQ_INT(*(int *)fv, idx * 100);
      }
    }
    EXPECT_EQ_INT(hashtbl_remove(&ht, &keys[6], NULL), 0);
    EXPECT_EQ_UINT(hashtbl_size(&ht), 11);
    q = 6;
    EXPECT_NULL(hashtbl_findnode(&ht, &q));
    EXPECT_EQ_INT(hashtbl_remove(&ht, &keys[0], NULL), 0);
    EXPECT_EQ_INT(hashtbl_remove(&ht, &keys[11], NULL), 0);
    EXPECT_EQ_UINT(hashtbl_size(&ht), 9);
    hashtbl_fini(&ht);
  }

  {
    struct hashtbl ht;
    struct hashtbl_fns fns = {edg_hash_zero, edg_cmp_int, NULL, NULL};
    int a, b, c, v;

    EXPECT_EQ_INT(hashtbl_init(&ht, &fns), 0);
    a = 1;
    b = 2;
    c = 3;
    v = 7;
    EXPECT_EQ_INT(hashtbl_insert(&ht, &a, &v), 0);
    EXPECT_EQ_INT(hashtbl_insert(&ht, &b, &v), 0);
    EXPECT_EQ_INT(hashtbl_insert(&ht, &c, &v), 0);
    EXPECT_EQ_INT(hashtbl_update(&ht, &b, &v, NULL), 0);
    EXPECT_EQ_PTR(hashtbl_find(&ht, &b), &v);
    EXPECT_EQ_INT(hashtbl_remove(&ht, &a, NULL), 0);
    EXPECT_NOTNULL(hashtbl_findnode(&ht, &c));
    EXPECT_NOTNULL(hashtbl_findnode(&ht, &b));
    hashtbl_fini(&ht);
  }

  {
    struct hashtbl ht;
    struct hashtbl_fns fns = {edg_hash_int, edg_cmp_int, NULL, NULL};
    int k1, k2, v;

    EXPECT_EQ_INT(hashtbl_init(&ht, &fns), 0);
    k1 = 42;
    k2 = 42;
    v = 1;
    EXPECT_EQ_INT(hashtbl_insert(&ht, &k1, &v), 0);
    EXPECT_EQ_INT(hashtbl_insert(&ht, &k2, &v), -1);
    EXPECT_EQ_UINT(hashtbl_size(&ht), 1);
    EXPECT_EQ_PTR(hashtbl_find(&ht, &k1), &v);
    EXPECT_EQ_PTR(hashtbl_find(&ht, &k2), &v);
    hashtbl_fini(&ht);
  }

  {
    struct hashtbl ht;
    struct hashtbl_fns fns = {edg_hash_int, edg_cmp_int, edg_free_key, NULL};
    int *pk;
    int v;
    int klookup;

    EXPECT_EQ_INT(hashtbl_init(&ht, &fns), 0);
    pk = (int *)malloc(sizeof(int));
    EXPECT_NOTNULL(pk);
    *pk = 100;
    v = 1;
    EXPECT_EQ_INT(hashtbl_insert(&ht, pk, &v), 0);
    klookup = 100;
    EXPECT_EQ_PTR(hashtbl_find(&ht, &klookup), &v);
    hashtbl_fini(&ht);
  }

  {
    struct hashtbl ht;
    struct hashtbl_fns fns = {edg_hash_int, edg_cmp_int, NULL, NULL};
    float old_a, old_b;

    EXPECT_EQ_INT(hashtbl_init(&ht, &fns), 0);
    EXPECT_EQ_INT(hashtbl_setthreshold(&ht, 0.25f, &old_a), 0);
    EXPECT_GT_DOUBLE((double)old_a, 0.0);
    EXPECT_EQ_INT(hashtbl_setthreshold(&ht, 0.9f, &old_b), 0);
    EXPECT_GE_DOUBLE((double)old_b, 0.24);
    EXPECT_LE_DOUBLE((double)old_b, 0.26);
    EXPECT_LE_DOUBLE((double)hashtbl_loadfactor(&ht), (double)hashtbl_threshold(&ht));
    hashtbl_fini(&ht);
  }

  {
    struct hashtbl ht;
    struct hashtbl_fns fns = {edg_hash_int, edg_cmp_int, edg_key_dtor_inc, edg_val_dtor_inc};
    int k, v;

    edg_key_dtor_n = 0;
    edg_val_dtor_n = 0;
    EXPECT_EQ_INT(hashtbl_init(&ht, &fns), 0);
    k = 8;
    v = 80;
    EXPECT_EQ_INT(hashtbl_insert(&ht, &k, &v), 0);
    EXPECT_EQ_INT(hashtbl_remove(&ht, &k, NULL), 0);
    EXPECT_EQ_INT(edg_key_dtor_n, 1);
    EXPECT_EQ_INT(edg_val_dtor_n, 1);
    hashtbl_fini(&ht);
  }

  {
    struct hashtbl ht;
    struct hashtbl_fns fns = {edg_hash_int, edg_cmp_int, edg_key_dtor_inc, NULL};
    int k, v;
    void *outv;

    edg_key_dtor_n = 0;
    EXPECT_EQ_INT(hashtbl_init(&ht, &fns), 0);
    k = 5;
    v = 50;
    EXPECT_EQ_INT(hashtbl_insert(&ht, &k, &v), 0);
    EXPECT_EQ_INT(hashtbl_remove(&ht, &k, &outv), 0);
    EXPECT_NOTNULL(outv);
    EXPECT_EQ_PTR(outv, &v);
    EXPECT_EQ_INT(edg_key_dtor_n, 1);
    hashtbl_fini(&ht);
  }

  {
    struct hashtbl ht;
    struct hashtbl_fns fns = {edg_hash_int, edg_cmp_int, NULL, edg_val_dtor_inc};
    int k, v;
    void *outv;

    edg_val_dtor_n = 0;
    EXPECT_EQ_INT(hashtbl_init(&ht, &fns), 0);
    k = 6;
    v = 60;
    EXPECT_EQ_INT(hashtbl_insert(&ht, &k, &v), 0);
    EXPECT_EQ_INT(hashtbl_remove(&ht, &k, &outv), 0);
    EXPECT_NOTNULL(outv);
    EXPECT_EQ_PTR(outv, &v);
    EXPECT_EQ_INT(edg_val_dtor_n, 0);
    hashtbl_fini(&ht);
  }

  {
    struct hashtbl ht;
    struct hashtbl_fns fns = {edg_hash_int, edg_cmp_int, NULL, NULL};
    int k, v;
    int step;

    EXPECT_EQ_INT(hashtbl_init(&ht, &fns), 0);
    EXPECT_EQ_INT(hashtbl_setthreshold(&ht, 0.5f, NULL), 0);
    for (step = 0; step < 64; step++) {
      k = step + 1000;
      v = step;
      EXPECT_EQ_INT(hashtbl_insert(&ht, &k, &v), 0);
      EXPECT_LE_DOUBLE((double)hashtbl_loadfactor(&ht), (double)hashtbl_threshold(&ht));
    }
    hashtbl_fini(&ht);
  }

  {
    struct hashtbl ht;
    struct hashtbl_fns fns = {edg_hash_int, edg_cmp_int, NULL, NULL};
    int k1, k2, km;
    int v;

    EXPECT_EQ_INT(hashtbl_init(&ht, &fns), 0);
    k1 = 11;
    k2 = 22;
    v = 0;
    EXPECT_EQ_INT(hashtbl_insert(&ht, &k1, &v), 0);
    EXPECT_EQ_INT(hashtbl_insert(&ht, &k2, &v), 0);
    km = 9999;
    EXPECT_EQ_INT(hashtbl_update(&ht, &km, &v, NULL), -1);
    EXPECT_EQ_INT(hashtbl_remove(&ht, &km, NULL), -1);
    EXPECT_NULL(hashtbl_find(&ht, &km));
    EXPECT_NULL(hashtbl_findnode(&ht, &km));
    EXPECT_EQ_UINT(hashtbl_size(&ht), 2);
    hashtbl_fini(&ht);
  }

  {
    struct hashtbl ht;
    struct hashtbl_fns fns = {edg_hash_zero, edg_cmp_int, NULL, NULL};
    int k0, k1;
    int v0, v1;
    struct hashtbl_node *n0;
    struct hashtbl_node *n1;

    EXPECT_EQ_INT(hashtbl_init(&ht, &fns), 0);
    k0 = 0;
    k1 = 1;
    v0 = 10;
    v1 = 11;
    EXPECT_EQ_INT(hashtbl_insert(&ht, &k0, &v0), 0);
    EXPECT_EQ_INT(hashtbl_insert(&ht, &k1, &v1), 0);
    n1 = hashtbl_findnode(&ht, &k1);
    EXPECT_NOTNULL(n1);
    EXPECT_NOTNULL(n1->next);
    EXPECT_EQ_PTR(n1->next->key, &k0);
    n0 = hashtbl_findnode(&ht, &k0);
    EXPECT_NOTNULL(n0);
    EXPECT_NULL(n0->next);
    hashtbl_fini(&ht);
  }
}
